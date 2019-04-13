#ifndef _VM_STRUCT_DEF_H__
#define _VM_STRUCT_DEF_H__
#include "type_def.h"


/*ҳ��ṹ*/
typedef struct Tag_PTE
{	
	uInt valid_:VALID_LEN;			/*��Чλ 1*/
	uInt vpn_:VPN_LEN;				/*����ҳ��10*/
	uInt ppn_:PPN_LEN;				/*����ҳ��6*/
	uInt status_:PAGE_STATUS_LEN;	/*״̬λ 2������״̬δ����00��δ����01���ѻ���11*/
}PTE,*PPTE;

/*cache�ṹ ֱ��ӳ��*/
typedef struct Tag_CATHE_ITEM
{
	byte gindex_:ITEM_INDEX_LEN;	/*������4*/
	byte gflag_:ITEM_TAG_LEN;		/*���8*/
	byte valid_:VALID_LEN;			/*��Чλ1*/
	byte block[ROW_BLOCK_SIZE];		/*�д�С4*/
}CACHE_ITEM,PCACHE_ITEM;

/*TLB�ṹ*/
typedef struct Tag_TLB_ITEM
{
	byte tlbt_index_:TLBT_LEN ;		/*���λ 6*/
	byte tlbi_index_:TLBI_LEN ;		/*������ 2*/
	byte ppn_:PPN_LEN ;				/*����ҳ�� 6*/
	byte valid_:VALID_LEN;			/*��Чλ 1*/
}TLB_ITEM,*PTLB_ITEM;


/*����ҳ�����ṹ*/
typedef struct _PM_Monitor_
{
	ulong processId;		/*����ID*/
	uInt  pm_pagenum;		/*��ǰ����ҳ��*/ 
	ulong vm_pagenum;		/*��Ӧ������ҳ��*/	
	//bool pm_isUsed_;		/*ռ�ñ��*/
}PM_MONITOR;

#endif