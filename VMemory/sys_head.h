#ifndef  _VM_SYS_HEAD_H__
#define _VM_SYS_HEAD_H__

#include "type_def.h"
#include "struct_def.h"

extern byte				PM[PM_SIZE] ; //�����ڴ�:	���Խṹ,һά�ֽڵ�ַ�ռ�
//extern PM_MONITOR		PM_Monitor[PM_PAGE_SIZE];
extern TLB_ITEM			Tlb_Item[TLB_ITEM_SIZE]; //TLB��
extern CACHE_ITEM		L1_Cache[CACHE_GROUP_SIZE] ; // L1 ����
extern PTE						VM_Pte[PAGE_ITEM_SIZE];	//ҳ��

#endif