#include<reg51.h>
int i = 0;

void timer_ISR(void)interrupt 1
{
	i++;
	if(i == 10)
	{
		i=0;
		P1++;
	}
}

void main(void)
{
	TMOD = 0x01;
	ET0 = 1;
	TR0 = 1;
	EA = 1;
	while(1);
}