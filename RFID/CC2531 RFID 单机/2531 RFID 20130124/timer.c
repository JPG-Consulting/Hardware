#include "include.h"

uint counter=0;

void init_time3(void)
{
	T3CCTL0 |= 0x04; //配置成比较模式， 
	T3CC0   = 0xFA;  //250 * 4 = 1000us，即定时1ms
	//T3CCTL1 = 0x00; 
	//T3CC1   = 0x00; 
	EA = 1;      
	T3IE = 1;
	T3CTL = 0xDE; //时钟16MHz，64分频，定时器0.25MHz，定时器counter增1，步进4us。使能定时器。使能中断。清除counter。模式：modulo mode。
}

#pragma vector = T3_VECTOR
__interrupt void T3_ISR(void)
{
	IRCON &= 0xFD;  //清除OVFIF
	T3IE = 0;

	//WaitTimes++;
	SysTime++;
	/*if(WaitTimes>=10)
	{
		WaitTimes=10;
		uart_count=0;
		uart_comp=0;
	}*/
	if(KeyTime)
	{
		KeyTime--;
	}
	T3IE = 1;
}
