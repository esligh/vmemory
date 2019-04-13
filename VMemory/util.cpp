#include "stdafx.h"
#include "type_def.h"
/*
	计算2为底的n 次幂函数
*/
ulong pow_base2(unsigned n)
{
	ulong result =1;
	for(unsigned i=0;i<n;++i)
		result<<=1;
	return result;
}
/*
	提取指定位
	从start开始提取，提取n位作为byte值返回
*/
byte _extract(ulong value,byte start,byte n)
{
		value>>=start;
		ulong mask = -1;		//掩码
		mask<<=n;
		value &=(~mask);
		return (byte)value;
}