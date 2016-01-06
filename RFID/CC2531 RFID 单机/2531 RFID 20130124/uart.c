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
*�������� ����ʼ������1										
*��ڲ��� ����												
*�� �� ֵ ����							
*˵    �� ��38400-8-n-1						
****************************************************************/
void initUART(void)
{

    CLKCONCMD &= ~0x40;              //����
    while(!(SLEEPSTA & 0x40));      //�ȴ������ȶ�
    CLKCONCMD &= ~0x47;             //TICHSPD128��Ƶ��CLKSPD����Ƶ
    SLEEPCMD |= 0x04; 		 //�رղ��õ�RC����

    PERCFG = 0x01;				//λ��1 P0��
    P1SEL |= 0x30;				//P0��������

    U0CSR |= 0x80;				//UART��ʽ
    U0GCR |= 8;				//baud_e
    U0BAUD |= 59;				//��������Ϊ9600
    UTX0IF = 1;

    U0CSR |= 0X40;				//�������
    IEN0 |= 0x84;				//�����жϣ������ж�
}

/****************************************************************
*�������� �����ڷ����ַ�������					
*��ڲ��� : data:����									
*			len :���ݳ���							
*�� �� ֵ ����											
*˵    �� ��				
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
*�������� �����ڽ���һ���ַ�					
*��ڲ��� : ��						
*�� �� ֵ ����				
*˵    �� ��������ɺ�򿪽���				
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
