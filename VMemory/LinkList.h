#ifndef  _VM_DATA_STRUCT_LINKLIST_H__
#define  _VM_DATA_STRUCT_LINKLIST_H__
#include "struct_def.h"
typedef PM_MONITOR			ElemType;
typedef int					Status;
#define  ERROR				0
#define  OK					1

typedef struct _LNode
{
	ElemType data;
	_LNode* next;
}LNode;
typedef LNode* LinkList;

//all operator
void InitList(LinkList &L);
void ClearList(LinkList  &L);
bool ListEmpty(LinkList L);
int ListLength(LinkList L);
Status GetElem(LinkList L,int i,ElemType &e);
int LocateElem(LinkList L,ElemType e,Status(*compare)(ElemType,ElemType));

Status ListInsert(LinkList &L,int i,ElemType e);
Status ListDelete(LinkList &L,int i ,ElemType &e);
void ListTraverse(LinkList L,void(*vi)(ElemType));

//Status PriorElem(LinkList L,ElemType cur_e,ElemType &pre_e);
//Status NextElem(LinkList L,ElemType cur_e,ElemType& pre_e);

#endif