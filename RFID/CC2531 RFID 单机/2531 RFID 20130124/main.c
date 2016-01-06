#include "include.h"

uchar PassWd[6] = {0x00};				
uchar Read_Data[16] = {0x00};
uchar WriteData[16] = {0x00};
uchar RechargeData[4] = {0x00,0x00,0x00,0x00};
uchar ConsumeData[4] = {0x00,0x00,0x00,0x00};
uchar RevBuffer[35] = {0x00};
uchar SendBuffer[35] = {0x00,DevType,0x00,0x00,0x00,0x00,0x00,0x00};
uchar MLastSelectedSnr[4] = {0x00};

uchar uart_count = 0;
uchar uart_comp = 0;
uint KeyNum = 0;
uint KuaiN = 0; 
uchar bWarn = 0;
uchar bPass = 0;
uchar KeyTime = 0;
uchar WaitTimes = 0;

//uchar SysTime = 0;
long int SysTime = 0;

uchar oprationcard = 0;
uchar bSendID = 0;
uchar temp = 0;
extern uchar result1;
void main(void)
{
	InitAll();
	while(1)
	{  
		if(bWarn == 1)
		{
			bWarn = 0;
			Warn();
		}
		if(bPass == 1)
		{
			bPass = 0;
			Pass();
		}
		if(uart_comp == 1)
		{
			ctrl_uart();
			uart_comp = 0;
		}
		if(SysTime >= 40)
		{   
			SysTime = 0;
			YLED = !YLED;
			ctrlprocess();
		}
	}  
}

