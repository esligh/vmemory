#ifndef _VM_TYPE_DEF_H__
#define  _VM_TYPE_DEF_H__
/*基本类型及常量定义*/
typedef unsigned char	byte; 
typedef unsigned char* pbyte;
typedef unsigned long	ulong;
typedef unsigned short	ushort;
typedef unsigned int	uInt;

/*
	物理页面数 = 物理内存大小/页面大小
		256		= 256*64 / 64    
	物理页面256 可以用8位来表示
*/
#define  PM_PAGE_SIZE	256	//
#define  PM_SIZE	256*64  // 物理内存大小4k(字节数)
#define  PAGE_SIZE	64	// 页面大小64字节


#define  VPN_LEN		10			//虚拟页号
#define	 PPN_LEN		8			//物理页号
#define  VPO_LEN		6			//虚拟页偏移
#define  PPO_LEN		6			//物理页偏移 ==虚拟页偏移
#define	VALID_LEN		1			//有效位
/*
	虚拟地址位长 =  VPN(虚拟页号 10位)  + VPO(虚拟页偏移 6位)
	物理地址位长 = PPN(物理页号 8位) + PPO(物理页偏移 6位)

	虚拟空间大小 =  2^16 = 64KB 
*/
#define VM_ADDR_LEN  16  //虚拟地址位长
#define  PM_ADDR_LEN 14  //物理地址位长


/*
	高速缓存结构说明：
		物理寻址 、直接映射、 行大小4字节，总共16组
	缓存标记 CT(6位) 
	缓存组索引CI(4位)
	缓存偏移 CO(2位)
	标记位
	四字节块 (4字节，按偏移量来访问)
*/
#define  CACHE_GROUP_SIZE		16  // 16组 
#define ITEM_INDEX_LEN			4	//组索引
#define ITEM_TAG_LEN			8	//标记

#define ITEM_OFFSET_LEN			2	//偏移位长
#define ROW_BLOCK_SIZE			4	//行大小
/*
	页表说明：
		PTE(页表条目) = VPN(虚拟页号10)+PPN(物理页号6)+有效位(2)
		页表大小 =  PTE大小* (虚拟地址空间大小(2^16B)/页面大小(64B))	
*/
#define	 PAGE_ITEM_SIZE		1024	//2^10 PTE条目数
#define  PAGE_STATUS_LEN	2		//虚拟页状态位

/*
Vmini 进程存储器映像
虚拟页面 1024页
0~31		未用	32
32~255		数据区 224
256~511		堆区 向上增长 256
512~767		栈区 向下增长 256
768~1023	内核虚拟存储器 256	
*/

//虚拟存储器共256页
#define  VM_LOAD_BASE_PAGE		32		//虚拟内存起始加载页
#define VM_LOAD_TAIL_PAGE		255		//加载终止页
#define	 PM_LOAD_BASE			64		//物理内存中开始加载的页面索引


/* TLB翻译后备缓冲区
四组 16个条目 四路组相联
TBL结构：TLBT 、TLBI 、有效位
TBLT+TLBI = VPN 虚拟页号位长
*/
#define  TLBT_LEN   6			// 组标记位长
#define  TLBI_LEN    2			//组索引位长 
#define  TLB_ITEM_SIZE 16  //TLB条目数

#endif
