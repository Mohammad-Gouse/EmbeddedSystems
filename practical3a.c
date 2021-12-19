#include<reg51.h>
void delay(int time);
void main()
{
	P1 = 00000000;
	while(1)
	{
		P1++;
		delay(100);
	}
}

void delay(int time)
{
	int i, j;
	for(i = 0; i <= time; i++)
	{
		for(j = 0; j <= 23; j++)
		{
		}
	}
		
}