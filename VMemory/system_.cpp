#include "stdafx.h"
#include "struct_def.h"
#include "system_.h"
#include "util.h"
#include "LinkList.h"
#define  _DEBUG_VIEW

//VM���ֽṹ

byte			PM[PM_SIZE] ; //�����ڴ�: ���Խṹ,һά�ֽڵ�ַ�ռ�
PM_MONITOR		PM_Monitor[PM_PAGE_SIZE];

LinkList		PM_IdlepageList;	//����ҳ��������
LinkList		PM_NonIdlepageList; //����ҳ�ǿ�������

TLB_ITEM		Tlb_Item[TLB_ITEM_SIZE]; //TLB��
CACHE_ITEM		L1_Cache[CACHE_GROUP_SIZE] ; // L1 ����
PTE			VM_Pte[PAGE_ITEM_SIZE];	//ҳ��

/*VM�ĳ�ʼ�� */
void _Init_()
{
	for(int i=0;i<PM_SIZE;++i)
		PM[i]= -1; // ��ʼ���ڴ�Ϊ0xff
	/*
	for(int i=0;i<PM_PAGE_SIZE;++i){
		if(i<PM_LOAD_BASE)				//���������ڴ�ǰ10��ҳ��
			PM_Monitor[i].pm_isUsed_ = true;
		else
			PM_Monitor[i].pm_isUsed_ = false;
		PM_Monitor[i].vm_pagenum = -1;  //-1 �����ʼ����ҳ����Ч
	}
	*/
	PM_MONITOR pm;
	InitList(PM_NonIdlepageList);	//��ʼ���ǿ�������
	InitList(PM_IdlepageList);		//��ʼ����������
	for(int i=PM_LOAD_BASE;i<PM_PAGE_SIZE;++i){ //��PM_LOAD_BASE��ʼ������ҳ��ӵ���������
		memset(&pm,0,sizeof(pm));
	//	pm.pm_isUsed_ =false;
		pm.vm_pagenum=-1;
		pm.pm_pagenum=i;	//��Ǹ�����ҳ

		ListInsert(PM_IdlepageList,i+1,pm); //��ʼ��ӵ���������
	}
	
	for(int i = 0;i<PAGE_ITEM_SIZE;++i){ //��ʼ��ҳ��
		VM_Pte[i].ppn_= -1; //��ʼ��Ϊ1*
		VM_Pte[i].vpn_ = -1; //��ʼ��Ϊ1*
		VM_Pte[i].valid_ = 0;
		VM_Pte[i].status_ = 0;  //δ����״̬
	}
	for(int i=0;i<CACHE_GROUP_SIZE;++i){ //��ʼ�����ٻ���
		L1_Cache[i].gindex_ = i;
		L1_Cache[i].valid_ = 0;
	}
}
//��ѯ�ڴ���ָ��λ�õ��ֽ�
byte pm_search(ulong pos)
{
	assert(pos>=0 && pos< PM_SIZE);
	return PM[pos]; 
}

//ӳ��������ݿ鵽����洢�� (���̼����״�ӳ��)
void _map_inVm_(byte bytestream[],ulong streamlen)
{
	//ע: ҳ���СΪ64�ֽ� 256��ҳ�� ǰ30��ҳ�汣��
	//ҳ���31��ʼӳ��
	assert(streamlen>=0);
	ulong page_num = 0;
	if((streamlen%PAGE_SIZE) == 0) //����ҳ����
		page_num = streamlen/PAGE_SIZE; 
	else
		page_num = streamlen/PAGE_SIZE +1;
	if(page_num>VM_LOAD_TAIL_PAGE-VM_LOAD_BASE_PAGE) //���ڽ��̵�ַ����ռ�����ݴ�������
		page_num = VM_LOAD_TAIL_PAGE-VM_LOAD_BASE_PAGE;

	ulong base_load = VM_LOAD_BASE_PAGE ; //���صĻ�ҳ������ 20��ʼ
	for(ulong i = 0;i<page_num;++i){
		VM_Pte[base_load+i].vpn_ = i; 
		VM_Pte[base_load+i].status_ =0x1; //�ѻ���
	}
}

//������洢����ĳ��ҳ����ص��ڴ���
/*
	pa��ӳ�������
	vm_panum: ��Ҫӳ�������ҳ��
	pm_panum:��Ҫӳ�������ҳ��
	size: ���ݴ�С,Ĭ��Ϊҳ���С
*/

bool _DMA(byte pa[PAGE_SIZE],ulong vm_panum,ulong pm_panum,ulong size/*PAGE_SIZE*/) 
{
	assert(pm_panum>=PM_LOAD_BASE);		//pm_panum 10~64
//	if(PM_Monitor[pm_panum].pm_isUsed_)	//������ҳ��ռ��
//		return false;
	int index = get_index_from_pte(vm_panum);//��ȡ����ҳ��ҳ����PTE������
	if(index == -1)
		return false;
	for(ulong i = 0;i<size;++i){
		PM[pm_panum*PAGE_SIZE+i] = pa[i];
	}
//	PM_Monitor[pm_panum].pm_isUsed_ = true;	//���ռ��
	PM_Monitor[pm_panum].vm_pagenum = vm_panum;

	VM_Pte[index].valid_ = 0x1;		//�����Ч
	VM_Pte[index].ppn_ = pm_panum; //��������ҳ��
	VM_Pte[index].status_ = 0x3;		//11 �ѷ���
	return true;
}

//��������ҳ���PTE����ֵ
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
���������ݵ�L1-cache:
	���������ַ pm_addr
	����ָ����һ�����ݵ�L1	���ĸ��ֽ�
pm_addr	�����ַ �����˼��ص�����������ƫ��

ע�������ַ14λ����ַ��ΧΪ 0~128*128-1
		��˲���pm_addr �ĵ�14λ��Ч
*/
void load_data_from_pm(ulong pm_addr)
{
	assert(pm_addr <PM_SIZE);		//����Խ��
	ulong mask = -1;	//λ����
	mask<<=ITEM_OFFSET_LEN;
	pm_addr &= mask; 
	ulong gindex_= _extract(pm_addr,ITEM_OFFSET_LEN,ITEM_INDEX_LEN);
	//��������
	pbyte det = &L1_Cache[gindex_].block[0];
	pbyte src = &PM[pm_addr];
	for(ulong i =0;i<ROW_BLOCK_SIZE;i+=2)		//���ֽ���Ϊż��
	{
		*(det+i)=*(src+i);
		*(det+i+1)=*(src+i+1);
	}
	//��ȡ���
	pm_addr>>=PPO_LEN; //ȥ��PPOƫ��λ ͬPPN����ҳ��
	L1_Cache[gindex_].gflag_ = pm_addr;
	//�޸���Чλ
	L1_Cache[gindex_].valid_ = 1;
}


/*
	MMU��ַ���룺
	���ݸ����������ַ������������ַ����
*/
ulong _MMU(ulong vm_addr)
{
	assert(vm_addr < pow_base2(VM_ADDR_LEN)); //Խ��
	ulong search_id = vm_addr>>VPO_LEN;
	int index_ = get_index_from_pte(search_id);
	if(index_ == -1)
		return -1; 
	byte ppn = 0;
	if(VM_Pte[index_].status_ == 0x03){  //�ѷ���
		ppn = VM_Pte[index_].ppn_;
	}
	else if(VM_Pte[index_].status_ == 0x01){ //�ѻ���
	}
	else{ //δ����
	}
	ulong pm_addr = (ulong)ppn; 
	pm_addr<<=VPO_LEN; 
	byte tsm = (byte)vm_addr ;  //��ȡ��8λ ����λVPO��Ч
	pm_addr |= tsm;
	return pm_addr;
}

/*
	���������ַ�������� ��L1�����ڴ��м��� 
*/
byte _load(ulong pm_addr)
{
	assert(pm_addr < pow_base2(PM_ADDR_LEN)); //Խ��
	byte co  = _extract(pm_addr,0,ITEM_OFFSET_LEN);  //��ȡƫ��
	byte ci = _extract(pm_addr,ITEM_OFFSET_LEN,ITEM_INDEX_LEN); //��ȡ������
	byte ct = _extract(pm_addr,ITEM_OFFSET_LEN+ITEM_INDEX_LEN,
								ITEM_TAG_LEN);  // ��ȡ���
	byte  retdata = -1;
	if(L1_Cache[ci].valid_ == 0x1) { //��Ч
		if(L1_Cache[ci].gflag_ == ct){ //�����ͬ
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
			retdata = PM[pm_addr];		//������в�ͬ
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
