#include<reg51.h>
void delay(unsigned int ms)
{
	unsigned int i, j;
	for(i = 0; i<ms; i++)
	{
		for(j = 0; j<=1275; j++)
		{
		}
	}
}

void main(void)
{
	char number[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
	int i, j;
	P2 = 0x00;
	P3 = 0x00;
	while(1)
	{
		for(i = 0; i<=9; i++)
		{
			P2 = number[i];
			for(j = 0; j<=9; j++)
			{
				P3 = number[j];
				delay(50);
			}
		}
	}
}