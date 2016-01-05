#include <ioCC2530.h>

#define uint unsigned int
#define uchar unsigned char
//定义控制灯的端口
#define RLED P0_5	//定义LED1为P0_5口控制
#define YLED P2_0	//定义LED2为P2_0口控制
#define relay P0_7
#define K1 P1_1
#define K2 P1_0

#define ON 0
#define OFF 1


//函数声明
void Delay(uint);		//延时函数
void Initial(void);		//初始化P口
void InitKey(void);
uchar KeyScan(void);

uchar Keyvalue = 0 ;

/****************************
//延时
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
//按键初始化
*****************************************/
void InitKey(void)
{
	P1SEL &= ~0X03;
	P1DIR &= ~0X03; //按键在P12 P13
	P1INP |= 0x03; //上、下拉
}

/****************************************************************
*函数功能 ：初始化串口1										
*入口参数 ：无												
*返 回 值 ：无							
*说    明 ：57600-8-n-1						
****************************************************************/
void initUARTtest(void)
{

    CLKCONCMD &= ~0x40;              //晶振
    while(!(SLEEPSTA & 0x40));      //等待晶振稳定
    CLKCONCMD &= ~0x47;             //TICHSPD128分频，CLKSPD不分频
    SLEEPCMD |= 0x04; 		        //关闭不用的RC振荡器

    PERCFG = 0x01;				//位置1 P0口
    P1SEL = 0x30;				//P0用作串口

    U0CSR |= 0x80;				//UART方式
    U0GCR |= 8;				//baud_e
    U0BAUD |= 59;				//波特率设为57600
    UTX0IF = 1;

    U0CSR |= 0X40;				//允许接收
    IEN0 |= 0x84;				//开总中断，接收中断
}

/****************************
//初始化程序
*****************************/
void Initial(void)
{
	P0DIR |= 0x20; //P0_5定义为输出
    P2DIR |= 0x01; //P2_0定义为输出
	RLED = OFF;
	YLED = OFF;	//LED
}

/*****************************************
//读键值
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

void write_relay_1(void)//继电器控制管脚 输出1    
{
	relay = 1;
}

void write_relay_0(void)//继电器控制管脚 输出0    
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
//主函数
***************************/
void main(void)
{
	
    Initial();		//调用初始化函数
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
                  RLED = ON;        //亮灯
                  YLED = OFF;
                  write_relay_1();
                  UartTX_Send_String(false_result,3);
                }
                else
                {
                  YLED = ON;        //亮灯
                  RLED = OFF; 
                  write_relay_0();
                  UartTX_Send_String(true_result,3);
                }
                
            }
           Delay(4000); 
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
 	URX0IF = 0;				//清中断标志
	temp = U0DBUF;
 }
