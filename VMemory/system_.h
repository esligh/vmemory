#ifndef	_VM_SYSTEM_HEAD_H__
#define  _VM_SYSTEM_HEAD_H__

void _Init_(); // 系统初始化
byte pm_search(ulong pos); //查询内存指定位置的值
void _map_inVm_(byte bytestream[],ulong streamlen);
bool _DMA(byte pa[],ulong vm_panum,ulong pm_panum,ulong size =PAGE_SIZE);
int get_index_from_pte(ulong vm_panum);
void load_data_from_pm(ulong pm_addr);
ulong _MMU(ulong vm_addr);
byte _load(ulong pm_addr);

#endif