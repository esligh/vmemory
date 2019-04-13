#include "stdafx.h"
#include "struct_def.h"
#include "system_.h"
#include "util.h"
#include "LinkList.h"
#define  _DEBUG_VIEW

//VM部分结构

byte			PM[PM_SIZE] ; //物理内存: 线性结构,一维字节地址空间
PM_MONITOR		PM_Monitor[PM_PAGE_SIZE];

LinkList		PM_IdlepageList;	//物理页空闲链表
LinkList		PM_NonIdlepageList; //物理页非空闲链表

TLB_ITEM		Tlb_Item[TLB_ITEM_SIZE]; //TLB表
CACHE_ITEM		L1_Cache[CACHE_GROUP_SIZE] ; // L1 缓存
PTE			VM_Pte[PAGE_ITEM_SIZE];	//页表

/*VM的初始化 */
void _Init_()
{
	for(int i=0;i<PM_SIZE;++i)
		PM[i]= -1; // 初始化内存为0xff
	/*
	for(int i=0;i<PM_PAGE_SIZE;++i){
		if(i<PM_LOAD_BASE)				//保留物理内存前10个页面
			PM_Monitor[i].pm_isUsed_ = true;
		else
			PM_Monitor[i].pm_isUsed_ = false;
		PM_Monitor[i].vm_pagenum = -1;  //-1 标记起始虚拟页号无效
	}
	*/
	PM_MONITOR pm;
	InitList(PM_NonIdlepageList);	//初始化非空闲链表
	InitList(PM_IdlepageList);		//初始化空闲链表
	for(int i=PM_LOAD_BASE;i<PM_PAGE_SIZE;++i){ //从PM_LOAD_BASE开始将物理页添加到空闲链表
		memset(&pm,0,sizeof(pm));
	//	pm.pm_isUsed_ =false;
		pm.vm_pagenum=-1;
		pm.pm_pagenum=i;	//标记该物理页

		ListInsert(PM_IdlepageList,i+1,pm); //初始添加到空闲链表
	}
	
	for(int i = 0;i<PAGE_ITEM_SIZE;++i){ //初始化页表
		VM_Pte[i].ppn_= -1; //初始化为1*
		VM_Pte[i].vpn_ = -1; //初始化为1*
		VM_Pte[i].valid_ = 0;
		VM_Pte[i].status_ = 0;  //未分配状态
	}
	for(int i=0;i<CACHE_GROUP_SIZE;++i){ //初始化高速缓存
		L1_Cache[i].gindex_ = i;
		L1_Cache[i].valid_ = 0;
	}
}
//查询内存中指定位置的字节
byte pm_search(ulong pos)
{
	assert(pos>=0 && pos< PM_SIZE);
	return PM[pos]; 
}

//映射进程数据块到虚拟存储器 (进程加载首次映射)
void _map_inVm_(byte bytestream[],ulong streamlen)
{
	//注: 页面大小为64字节 256个页面 前30个页面保留
	//页面从31开始映射
	assert(streamlen>=0);
	ulong page_num = 0;
	if((streamlen%PAGE_SIZE) == 0) //计算页面数
		page_num = streamlen/PAGE_SIZE; 
	else
		page_num = streamlen/PAGE_SIZE +1;
	if(page_num>VM_LOAD_TAIL_PAGE-VM_LOAD_BASE_PAGE) //大于进程地址虚拟空间的数据代码区域
		page_num = VM_LOAD_TAIL_PAGE-VM_LOAD_BASE_PAGE;

	ulong base_load = VM_LOAD_BASE_PAGE ; //加载的基页面索引 20开始
	for(ulong i = 0;i<page_num;++i){
		VM_Pte[base_load+i].vpn_ = i; 
		VM_Pte[base_load+i].status_ =0x1; //已缓存
	}
}

//将虚拟存储器中某个页面加载到内存中
/*
	pa：映射的数据
	vm_panum: 需要映射的虚拟页号
	pm_panum:需要映射的物理页号
	size: 数据大小,默认为页面大小
*/

bool _DMA(byte pa[PAGE_SIZE],ulong vm_panum,ulong pm_panum,ulong size/*PAGE_SIZE*/) 
{
	assert(pm_panum>=PM_LOAD_BASE);		//pm_panum 10~64
//	if(PM_Monitor[pm_panum].pm_isUsed_)	//该物理页被占用
//		return false;
	int index = get_index_from_pte(vm_panum);//获取虚拟页在页表中PTE的索引
	if(index == -1)
		return false;
	for(ulong i = 0;i<size;++i){
		PM[pm_panum*PAGE_SIZE+i] = pa[i];
	}
//	PM_Monitor[pm_panum].pm_isUsed_ = true;	//标记占用
	PM_Monitor[pm_panum].vm_pagenum = vm_panum;

	VM_Pte[index].valid_ = 0x1;		//标记有效
	VM_Pte[index].ppn_ = pm_panum; //关联物理页号
	VM_Pte[index].status_ = 0x3;		//11 已分配
	return true;
}

//返回虚拟页面的PTE索引值
int get_index_from_pte(ulong vm_panum)
{
	int index=-1;
	for(int i=0;i<PAGE_ITEM_SIZE;++i)
	{
		if(VM_Pte[i].vpn_ == vm_panum)
		{
			index = i;
			break;
		}
	}
	return index;
}

/*
加载行数据到L1-cache:
	给定物理地址 pm_addr
	加载指定的一行数据到L1	共四个字节
pm_addr	物理地址 决定了加载的组索引和组偏移

注：物理地址14位，地址范围为 0~128*128-1
		因此参数pm_addr 的低14位有效
*/
void load_data_from_pm(ulong pm_addr)
{
	assert(pm_addr <PM_SIZE);		//访问越界
	ulong mask = -1;	//位掩码
	mask<<=ITEM_OFFSET_LEN;
	pm_addr &= mask; 
	ulong gindex_= _extract(pm_addr,ITEM_OFFSET_LEN,ITEM_INDEX_LEN);
	//加载数据
	pbyte det = &L1_Cache[gindex_].block[0];
	pbyte src = &PM[pm_addr];
	for(ulong i =0;i<ROW_BLOCK_SIZE;i+=2)		//行字节数为偶数
	{
		*(det+i)=*(src+i);
		*(det+i+1)=*(src+i+1);
	}
	//提取标记
	pm_addr>>=PPO_LEN; //去掉PPO偏移位 同PPN物理页号
	L1_Cache[gindex_].gflag_ = pm_addr;
	//修改有效位
	L1_Cache[gindex_].valid_ = 1;
}


/*
	MMU地址翻译：
	根据给定的虚拟地址，构造出物理地址返回
*/
ulong _MMU(ulong vm_addr)
{
	assert(vm_addr < pow_base2(VM_ADDR_LEN)); //越界
	ulong search_id = vm_addr>>VPO_LEN;
	int index_ = get_index_from_pte(search_id);
	if(index_ == -1)
		return -1; 
	byte ppn = 0;
	if(VM_Pte[index_].status_ == 0x03){  //已分配
		ppn = VM_Pte[index_].ppn_;
	}
	else if(VM_Pte[index_].status_ == 0x01){ //已缓存
	}
	else{ //未分配
	}
	ulong pm_addr = (ulong)ppn; 
	pm_addr<<=VPO_LEN; 
	byte tsm = (byte)vm_addr ;  //提取低8位 低六位VPO有效
	pm_addr |= tsm;
	return pm_addr;
}

/*
	根据物理地址加载数据 从L1或者内存中加载 
*/
byte _load(ulong pm_addr)
{
	assert(pm_addr < pow_base2(PM_ADDR_LEN)); //越界
	byte co  = _extract(pm_addr,0,ITEM_OFFSET_LEN);  //提取偏移
	byte ci = _extract(pm_addr,ITEM_OFFSET_LEN,ITEM_INDEX_LEN); //提取组索引
	byte ct = _extract(pm_addr,ITEM_OFFSET_LEN+ITEM_INDEX_LEN,
								ITEM_TAG_LEN);  // 提取标记
	byte  retdata = -1;
	if(L1_Cache[ci].valid_ == 0x1) { //有效
		if(L1_Cache[ci].gflag_ == ct){ //标记相同
			retdata = L1_Cache[ci].block[co];
#ifdef	_DEBUG_VIEW
			printf("catch it from cache.\n");
#endif
		}
		else{  
#ifdef	_DEBUG_VIEW
			printf("catch it from memory.\n");
#endif
			load_data_from_pm(pm_addr);	
			retdata = PM[pm_addr];		//设计略有不同
		}
	}
	else{
#ifdef	_DEBUG_VIEW
		printf("catch it from memory.\n");
#endif
		load_data_from_pm(pm_addr);
		retdata = PM[pm_addr];			
	}
	return retdata;
}
