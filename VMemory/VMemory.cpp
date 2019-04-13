// VMemory.cpp : �������̨Ӧ�ó������ڵ㡣
//
//�������ϵͳģ�� VM_mini  Version 0.0.1-By Li
/*
΢������洢ϵͳ
*/

/*
��������:
���ڸ����ĵ�ַVA�������ܹ���ȡ�����ڴ���һ���ֽ�
������裺
�洢�����ֽ�Ѱַ
�洢������ʱ���1�ֽڵ�
�����ַ14λ��(n=14)
�����ַ12λ��(m=12)
ҳ���С64�ֽ�(p=64)
TLB����·�����ģ��ܹ���16����Ŀ
L1 d-cache������Ѱַ��ֱ��ӳ��ģ��д�СΪ4�ֽڣ��ܹ�16��
*/


#include "stdafx.h"

#include "type_def.h"
#include "system_.h"
#include "out_debug.h"
#include "util.h"

int _tmain(int argc, _TCHAR* argv[])
{
	
	byte data[64*2+10]; //����ҳ�� ģ����� �߼���ַ�ռ�Ϊ0~137
	for(int i=0;i<64*2+10;++i)
		data[i]= i+3;
	_Init_();
	_map_inVm_(data,64*2+10);

	//DMA����ӳ�����
	_DMA(data,0,64); //ӳ���һ������ҳ���ڴ� ��64ҳ��ʼ����
	_DMA(data+64,1,74); //ӳ��ڶ�ҳ
	_DMA(data+2*64,2,80,10);//

	//������Ϣ��ʾ����
	print_page_PM(64);
	print_page_PM(74);
	print_page_PM(80);
	print_PTE(32,42);
	//�������ݵ����ٻ���������
	load_data_from_pm(64*64+3); //�����ַ643
	load_data_from_pm(64*64+6);
	print_cache(0,10);
	//�˵��˵�ַ�������
	printf("\n*************�˵��˵�ַ����******************\n");
	printf("%x\n",_load(_MMU(5)));
	printf("%x\n",_load(_MMU(7)));
	printf("%x\n",_load(_MMU(10)));

	print_cache(0,10);
	printf("%x\n",_load(_MMU(8)));
	printf("%x\n",_load(_MMU(9)));
	/*
	ҳ���û�����:
		�������ָ��320��
		�涨ÿ��ҳ����10��ָ��
		��32��ҳ��
		�涨��������ҳ��Ϊ4ҳ	��PM_LOAD_BASE ��ʼ���� ��PM_LOAD_BASE+3Ϊֹ
		LRU �û��㷨
	*/
//	printf("*********ҳ���û�����**************\n");
	
	getchar();
	return 0;
}

