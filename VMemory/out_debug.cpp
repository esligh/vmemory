#include "stdafx.h"
#include "out_debug.h"
/*********************调试文件********************/
//打印当前PM中某一数据块
void print_block_PM(ulong startpos,ulong size)
{
	printf("\n**************************************************\n\n");
	printf("print_block_PM:\t print a special block from PM.\n");
	if(size<0)
		size = 0;
	else if(size >PM_SIZE-startpos)
		size = PM_SIZE-startpos;
	printf("Mem_View Debug output: startpos = %x,size =%x\n\n\t",startpos,size);
	for(int i=0;i<16;++i)
		printf("%4x",i);
	printf("\n\n");	
	ulong end=startpos+size;
	int count = 0;
	printf("%x:\t",startpos);
	for(ulong i=startpos;i<end;++i){
		printf("%4x",PM[i]);
		count++;
		if(count%16 == 0){
			printf("\n");
			if(startpos+count < end)
				printf("%x:\t",startpos+count);
		}
	}
	printf("\n**************************************************\n");
}

//打印某一个物理页面的数据
void print_page_PM(ulong panum)
{
	printf("\n**************************************************\n\n");
	printf("print_page_PM:\t print a special page from PM.\n");
	if(panum>=PM_PAGE_SIZE)
		panum = PM_PAGE_SIZE;
	printf("Mem_View Debug output: pagenum=%x\n\n\t",panum);
	for(int i=0;i<16;++i)
		printf("%4x",i);
	printf("\n\n");
	ulong startpos = panum*PAGE_SIZE;
	ulong end = startpos+PAGE_SIZE;
	int count = 0;
	printf("%x:\t",startpos);
	for( ;startpos<end;){
		printf("%4x",PM[startpos++]);
		if(startpos%16 == 0){
			printf("\n");
			if(startpos<end)
				printf("%x:\t",startpos);
		}
	}
	printf("\n**************************************************\n");
}

//打印页表条目 n 条
void print_PTE(ulong startpos,ulong n)
{
	printf("\n***************************************************\n");
	printf("print_PTE: print the PTE ITEM.\n");
	printf("\tVALID\tVPN\tPPN\tSTATUS\n");
	if(n+startpos>PAGE_ITEM_SIZE)
		n = PAGE_ITEM_SIZE-startpos;
	for(ulong i=startpos;i<n;++i){
		printf("%x\t",i);
		printf("%x\t%x\t%x\t",VM_Pte[i].valid_,VM_Pte[i].vpn_,VM_Pte[i].ppn_);
		show_binary(VM_Pte[i].status_,2);
	}
	printf("\n***************************************************\n");
}
//打印字节的二进制码
/*
	value ：要显示的值
	n		:显示二进制的位数
*/
void show_binary(byte val,int n)
{
	int limit = 8-n;
	for(int i=limit+1;i<=limit+n;i++)
	{
		byte temp=0;
		__asm{
					mov ecx,i
					xor dh,dh
					mov ah,val
					shl	ah,cl
					jc		NEXT		//CF 标记位为1
					mov	dl,0
					jmp	LAB
			NEXT:
					mov dl,1
			LAB:
					mov temp,dl
		}
		printf("%d",temp);
	}
	printf("\n");
}

/*显示L1高速缓冲n个条目 共十六组*/

void print_cache(ulong strartpos,ulong n)
{
	if(n>CACHE_GROUP_SIZE-strartpos)
		n=CACHE_GROUP_SIZE-strartpos;
	printf("\n*************************************************\n");
	printf("print_cache: print the L1-Cache ITEM.\n");
	printf("G_ID\tTAG\tVALID\tBLOCK\n");
	for(ulong i=strartpos;i<strartpos+n;++i){
		printf("%x:\t",L1_Cache[i].gindex_);
		printf("%x\t%d\t",L1_Cache[i].gflag_,L1_Cache[i].valid_);
		show_cache_block(i);
		printf("\n");
	}
	printf("\n*************************************************\n");
}
/*打印L1 行数据块*/
void show_cache_block(ulong index_)
{
	for(int i = 0;i<ROW_BLOCK_SIZE;++i)
	{
		printf("%x\t",L1_Cache[index_].block[i]);
	}
}