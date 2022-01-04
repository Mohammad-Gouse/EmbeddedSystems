#include<reg51.h>
#include<stdio.h>

int sine[]={0,0,5,10,5,15,20,25,30,35,40,45,45,40,35,30,25,20,15,10,5,0,0};

void main()
{
	int i;
	P1=0x00;
	while(1)
	{
		for(i=0;i<23;i++)
		{
			P1=sine[i];
		}
	}
}