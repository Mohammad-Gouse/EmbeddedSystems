#include<reg51.h>
void main()
{
	P2= 0x00;
	while(1)
	{
		do
		{
			P2 += 0x05;
		}
		while(P2 < 0xFF);
		do
		{
			P2 -= 0x05;
		}
		while(P2 > 0x00);
	}
}