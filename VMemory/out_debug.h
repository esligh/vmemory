#ifndef	VM_OUT_DEBUG_MSG_H__
#define VM_OUT_DEBUG_MSG_H__
#include "sys_head.h"

void print_block_PM(ulong startpos,ulong size);
void print_page_PM(ulong panum);
void print_PTE(ulong startpos,ulong n);
void show_binary(byte val,int n);
void print_cache(ulong strartpos,ulong n);
void show_cache_block(ulong index_);

#endif