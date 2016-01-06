#include "include.h"

uint counter=0;

void init_time3(void)
{
	T3CCTL0 |= 0x04; //���óɱȽ�ģʽ�� 
	T3CC0   = 0xFA;  //250 * 4 = 1000us������ʱ1ms
	//T3CCTL1 = 0x00; 
	//T3CC1   = 0x00; 
	EA = 1;      
	T3IE = 1;
	T3CTL = 0xDE; //ʱ��16MHz��64��Ƶ����ʱ��0.25MHz����ʱ��counter��1������4us��ʹ�ܶ�ʱ����ʹ���жϡ����counter��ģʽ��modulo mode��
}

#pragma vector = T3_VECTOR
__interrupt void T3_ISR(void)
{
	IRCON &= 0xFD;  //���OVFIF
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
