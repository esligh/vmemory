#ifndef	_VM_SYSTEM_HEAD_H__
#define  _VM_SYSTEM_HEAD_H__

void _Init_(); // ϵͳ��ʼ��
byte pm_search(ulong pos); //��ѯ�ڴ�ָ��λ�õ�ֵ
void _map_inVm_(byte bytestream[],ulong streamlen);
bool _DMA(byte pa[],ulong vm_panum,ulong pm_panum,ulong size =PAGE_SIZE);
int get_index_from_pte(ulong vm_panum);
void load_data_from_pm(ulong pm_addr);
ulong _MMU(ulong vm_addr);
byte _load(ulong pm_addr);

#endif