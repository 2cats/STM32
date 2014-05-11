#include "tube.h"
//x:[0,3]
void EnableTubex(u8 D0,u8 D1,u8 D2,u8 D3)
{
	u16 data=0;
	data|=(D0<<8);
	data|=(D0<<9);
	data|=(D0<<10);
	data|=(D0<<11);
	In_595(data);
	Out_595();
}