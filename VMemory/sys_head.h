#ifndef  _VM_SYS_HEAD_H__
#define _VM_SYS_HEAD_H__

#include "type_def.h"
#include "struct_def.h"

extern byte				PM[PM_SIZE] ; //物理内存:	线性结构,一维字节地址空间
//extern PM_MONITOR		PM_Monitor[PM_PAGE_SIZE];
extern TLB_ITEM			Tlb_Item[TLB_ITEM_SIZE]; //TLB表
extern CACHE_ITEM		L1_Cache[CACHE_GROUP_SIZE] ; // L1 缓存
extern PTE						VM_Pte[PAGE_ITEM_SIZE];	//页表

#endif