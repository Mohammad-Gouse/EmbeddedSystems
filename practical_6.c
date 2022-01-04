#include<reg51.h>
void delay()
{
	int i, j;
	for(i = 0; i <= 100; i++)
	{
		for(j = 0; j < 100; j++)
		{
		}
	}
}

void main()
{
	while(1)
	{
		P2 = 0x09;
		delay();
		P2 = 0x03;
		delay();
		P2 = 0x06;
		delay();
		P2 = 0x0C;
		delay();
	}
}