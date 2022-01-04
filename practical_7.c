#include<reg51.h>
sbit red = P2^0;
sbit yellow = P2^1;
sbit green = P2^2;
void delay(int time);
void main()
{
	red = yellow = green = 0;
	while(1)
	{
		red = 1;
		delay(1000);
		red = 0;

		yellow = 1;
		delay(200);
		yellow = 0;

		green = 1;
		delay(1000);
		green = 0;
		
		yellow = 1;
		delay(200);
		yellow = 0;
	}
}

void delay(int time)
{
	int i, j;
	for(i = 0; i < time; i++)
	{
		for(j = 0; j < 1000; j++)
		{
		}
	}
}