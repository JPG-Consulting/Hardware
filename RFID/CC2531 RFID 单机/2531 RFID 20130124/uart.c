#include "include.h"

extern uchar RevBuffer[35];

//UART0 initialize
// desired baud rate: 9600
// actual: baud rate:9600 (0.0%)
// char size: 8 bit
// parity: Disabled
void initUART(void);
void UartTX_Send_String(uchar *Data,int len);

/****************************************************************
*函数功能 ：初始化串口1										
*入口参数 ：无												
*返 回 值 ：无							
*说    明 ：38400-8-n-1						
****************************************************************/
void initUART(void)
{

    CLKCONCMD &= ~0x40;              //晶振
    while(!(SLEEPSTA & 0x40));      //等待晶振稳定
    CLKCONCMD &= ~0x47;             //TICHSPD128分频，CLKSPD不分频
    SLEEPCMD |= 0x04; 		 //关闭不用的RC振荡器

    PERCFG = 0x01;				//位置1 P0口
    P1SEL |= 0x30;				//P0用作串口

    U0CSR |= 0x80;				//UART方式
    U0GCR |= 8;				//baud_e
    U0BAUD |= 59;				//波特率设为9600
    UTX0IF = 1;

    U0CSR |= 0X40;				//允许接收
    IEN0 |= 0x84;				//开总中断，接收中断
}

/****************************************************************
*函数功能 ：串口发送字符串函数					
*入口参数 : data:数据									
*			len :数据长度							
*返 回 值 ：无											
*说    明 ：				
****************************************************************/
void UartTX_Send_String(uchar *Data,int len)
{
  int j;
  for(j=0;j<len;j++)
  {
    U0DBUF = *Data++;
    while(UTX0IF == 0);
    UTX0IF = 0;
  }
}

/****************************************************************
*函数功能 ：串口接收一个字符					
*入口参数 : 无						
*返 回 值 ：无				
*说    明 ：接收完成后打开接收				
****************************************************************/
#pragma vector = URX0_VECTOR
__interrupt void UART0_ISR(void)
{	
	uchar R_Char;
	if(URX0IF > 0);
	{
		URX0IF = 0;
		WaitTimes=0;
		R_Char = U0DBUF;
		RevBuffer[uart_count]=R_Char;
		uart_count++;
		if(uart_count == (RevBuffer[0] + 9))
		{
			uart_comp=1;
			uart_count = 0;
		} 
	} 	
}
