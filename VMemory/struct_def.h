#ifndef _VM_STRUCT_DEF_H__
#define _VM_STRUCT_DEF_H__
#include "type_def.h"


/*页表结构*/
typedef struct Tag_PTE
{	
	uInt valid_:VALID_LEN;			/*有效位 1*/
	uInt vpn_:VPN_LEN;				/*虚拟页号10*/
	uInt ppn_:PPN_LEN;				/*物理页号6*/
	uInt status_:PAGE_STATUS_LEN;	/*状态位 2，三种状态未分配00，未缓存01，已缓存11*/
}PTE,*PPTE;

/*cache结构 直接映射*/
typedef struct Tag_CATHE_ITEM
{
	byte gindex_:ITEM_INDEX_LEN;	/*组索引4*/
	byte gflag_:ITEM_TAG_LEN;		/*标记8*/
	byte valid_:VALID_LEN;			/*有效位1*/
	byte block[ROW_BLOCK_SIZE];		/*行大小4*/
}CACHE_ITEM,PCACHE_ITEM;

/*TLB结构*/
typedef struct Tag_TLB_ITEM
{
	byte tlbt_index_:TLBT_LEN ;		/*标记位 6*/
	byte tlbi_index_:TLBI_LEN ;		/*组索引 2*/
	byte ppn_:PPN_LEN ;				/*物理页号 6*/
	byte valid_:VALID_LEN;			/*有效位 1*/
}TLB_ITEM,*PTLB_ITEM;


/*物理页面管理结构*/
typedef struct _PM_Monitor_
{
	ulong processId;		/*进程ID*/
	uInt  pm_pagenum;		/*当前物理页面*/ 
	ulong vm_pagenum;		/*对应的虚拟页号*/	
	//bool pm_isUsed_;		/*占用标记*/
}PM_MONITOR;

#endif