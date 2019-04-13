// VMemory.cpp : 定义控制台应用程序的入口点。
//
//虚拟操作系统模块 VM_mini  Version 0.0.1-By Li
/*
微型虚拟存储系统
*/

/*
问题需求:
对于给定的地址VA，可以能够存取给定内存中一个字节
问题假设：
存储器按字节寻址
存储器访问时针对1字节的
虚拟地址14位长(n=14)
物理地址12位长(m=12)
页面大小64字节(p=64)
TLB是四路组联的，总共有16个条目
L1 d-cache是物理寻址、直接映射的，行大小为4字节，总共16组
*/


#include "stdafx.h"

#include "type_def.h"
#include "system_.h"
#include "out_debug.h"
#include "util.h"

int _tmain(int argc, _TCHAR* argv[])
{
	
	byte data[64*2+10]; //两个页面 模拟进程 逻辑地址空间为0~137
	for(int i=0;i<64*2+10;++i)
		data[i]= i+3;
	_Init_();
	_map_inVm_(data,64*2+10);

	//DMA加载映射测试
	_DMA(data,0,64); //映射第一个虚拟页到内存 从64页开始加载
	_DMA(data+64,1,74); //映射第二页
	_DMA(data+2*64,2,80,10);//

	//调试信息显示测试
	print_page_PM(64);
	print_page_PM(74);
	print_page_PM(80);
	print_PTE(32,42);
	//加载数据到高速缓冲区测试
	load_data_from_pm(64*64+3); //物理地址643
	load_data_from_pm(64*64+6);
	print_cache(0,10);
	//端到端地址翻译测试
	printf("\n*************端到端地址翻译******************\n");
	printf("%x\n",_load(_MMU(5)));
	printf("%x\n",_load(_MMU(7)));
	printf("%x\n",_load(_MMU(10)));

	print_cache(0,10);
	printf("%x\n",_load(_MMU(8)));
	printf("%x\n",_load(_MMU(9)));
	/*
	页面置换测试:
		构造随机指令320条
		规定每个页面存放10条指令
		共32个页面
		规定可用物理页面为4页	从PM_LOAD_BASE 开始加载 到PM_LOAD_BASE+3为止
		LRU 置换算法
	*/
//	printf("*********页面置换测试**************\n");
	
	getchar();
	return 0;
}

