#include "stdafx.h"
#include "type_def.h"
/*
	����2Ϊ�׵�n ���ݺ���
*/
ulong pow_base2(unsigned n)
{
	ulong result =1;
	for(unsigned i=0;i<n;++i)
		result<<=1;
	return result;
}
/*
	��ȡָ��λ
	��start��ʼ��ȡ����ȡnλ��Ϊbyteֵ����
*/
byte _extract(ulong value,byte start,byte n)
{
		value>>=start;
		ulong mask = -1;		//����
		mask<<=n;
		value &=(~mask);
		return (byte)value;
}