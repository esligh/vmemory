// TestSuit.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../VMemory/LinkList.cpp"

void visit(int n)
{
	printf("%d ",n);
}
int _tmain(int argc, _TCHAR* argv[])
{
	LinkList L;
	InitList(L);
	for(int i=1;i<=10;i++)
	{
		ListInsert(L,i,i);
	}
	ListTraverse(L,visit);
	ElemType e;
	ListDelete(L,1,e);
	ListTraverse(L,visit);
	ListDelete(L,9,e);
	ListTraverse(L,visit);
	ListDelete(L,9,e);
	ListTraverse(L,visit);

	printf("len=%d\n",ListLength(L));
	printf("isempty=%d\n",ListEmpty(L));
	ElemType s=0;
	GetElem(L,11,s);
	printf("%d\n",s);
	return 0;
}

