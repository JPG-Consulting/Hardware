#include <ioCC2530.h>

#define uint unsigned int
#define uchar unsigned char
//������ƵƵĶ˿�
#define RLED P0_5	//����LED1ΪP0_5�ڿ���
#define YLED P2_0	//����LED2ΪP2_0�ڿ���
#define relay P0_7
#define K1 P1_1
#define K2 P1_0

#define ON 0
#define OFF 1


//��������
void Delay(uint);		//��ʱ����
void Initial(void);		//��ʼ��P��
void InitKey(void);
uchar KeyScan(void);

uchar Keyvalue = 0 ;

/****************************
//��ʱ
*****************************/
void Delay(uint n)
{
	uint tt;
	for(tt = 0;tt < n;tt++);
	for(tt = 0;tt < n;tt++);
	for(tt = 0;tt < n;tt++);
	for(tt = 0;tt < n;tt++);
	for(tt = 0;tt < n;tt++);
}

/*****************************************
//������ʼ��
*****************************************/
void InitKey(void)
{
	P1SEL &= ~0X03;
	P1DIR &= ~0X03; //������P12 P13
	P1INP |= 0x03; //�ϡ�����
}

/****************************************************************
*�������� ����ʼ������1										
*��ڲ��� ����												
*�� �� ֵ ����							
*˵    �� ��57600-8-n-1						
****************************************************************/
void initUARTtest(void)
{

    CLKCONCMD &= ~0x40;              //����
    while(!(SLEEPSTA & 0x40));      //�ȴ������ȶ�
    CLKCONCMD &= ~0x47;             //TICHSPD128��Ƶ��CLKSPD����Ƶ
    SLEEPCMD |= 0x04; 		        //�رղ��õ�RC����

    PERCFG = 0x01;				//λ��1 P0��
    P1SEL = 0x30;				//P0��������

    U0CSR |= 0x80;				//UART��ʽ
    U0GCR |= 8;				//baud_e
    U0BAUD |= 59;				//��������Ϊ57600
    UTX0IF = 1;

    U0CSR |= 0X40;				//�������
    IEN0 |= 0x84;				//�����жϣ������ж�
}

/****************************
//��ʼ������
*****************************/
void Initial(void)
{
	P0DIR |= 0x20; //P0_5����Ϊ���
    P2DIR |= 0x01; //P2_0����Ϊ���
	RLED = OFF;
	YLED = OFF;	//LED
}

/*****************************************
//����ֵ
*****************************************/
uchar KeyScan(void)
{
	if(K1 == 0)
	{
		Delay(100);
		if(K1 == 0)
		{
			  while(!K1);
			  return(1);
		}
	};

	if(K2 == 0)
	{
		Delay(100);
		if(K2 == 0)
		{
			  while(!K2);
			  return(2);
		}
	};
	return(0);
}

void UartTX_Send_String(uchar *Data, int len)
{
  int j;
  for(j=0;j<len;j++)
  {
    U0DBUF = *Data++;
    while(UTX0IF == 0);
    UTX0IF = 0;
  }
}

void write_relay_1(void)//�̵������ƹܽ� ���1    
{
	relay = 1;
}

void write_relay_0(void)//�̵������ƹܽ� ���0    
{
    relay = 0;
}

uchar RTflag = 1;
uchar temp;
uint  datanumber = 0;
uint  stringlen;
uchar true_result[3] = "1\n";
uchar false_result[3] = "0\n";

/***************************
//������
***************************/
void main(void)
{
	
    Initial();		//���ó�ʼ������
    InitKey();
    initUARTtest();
    Delay(4000);
    Delay(4000);
    
    P0DIR |= 0x80;
    YLED = ON;  

    U0CSR |= 0x40;
    while(1)
	{
            if(temp != 0)
            {
                if(temp=='0')
                {
                  RLED = ON;        //����
                  YLED = OFF;
                  write_relay_1();
                  UartTX_Send_String(false_result,3);
                }
                else
                {
                  YLED = ON;        //����
                  RLED = OFF; 
                  write_relay_0();
                  UartTX_Send_String(true_result,3);
                }
                
            }
           Delay(4000); 
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
 	URX0IF = 0;				//���жϱ�־
	temp = U0DBUF;
 }
