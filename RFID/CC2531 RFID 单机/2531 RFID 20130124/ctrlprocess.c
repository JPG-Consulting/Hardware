#include "include.h"

extern uchar result1;
uchar uart_test[4] = {0x55, 0xAA, 0x55, 0xAA};

void delay_ms(uint tms)
{
  uint ii;
  while(tms)
  {
    tms--;
    for(ii=0;ii<400;ii++)
    {
      asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    }
  }
}

void Init_IO_AND_LED(void)
{
	P0DIR = 0x80;     	//0为输入（默认），1为输出，设置P0_7为输出，P0_7管脚控制蜂鸣器
	CLR_BEEP;
	P1DIR |= 0x42;       //设置P1_1和P1_6为输出，P1_1管脚控制RFID读卡器上的指示灯。P1_6管脚控制MFRC522的复位管脚。
}
//告警蜂鸣器响3次
void Warn(void)
{
  uchar ii;
  for(ii=0;ii<3;ii++)
  {
    SET_BEEP;
    delay_ms(120);
    CLR_BEEP;
    delay_ms(120);
  }
}

void Pass(void)
{
  SET_BEEP;
  delay_ms(700);
  CLR_BEEP;
}

void InitRc522(void)
{
  PcdReset();
  PcdAntennaOff();  
  PcdAntennaOn();
  M522PcdConfigISOType( 'A' );
}

void InitAll(void)
{
  Init_I2C();
  Init_IO_AND_LED();
  InitRc522();
  EA=0;
  init_time3();
  initUART();
  EA=1; //re-enable interrupts  
  bWarn=0;
  bPass=0;
  SysTime=0;
  KeyTime=0;
  WaitTimes=0;
  KeyNum=0;
  KuaiN=0;
  oprationcard=0;
  uart_count=0;
  uart_comp=0;
  bSendID=0;
  Pass();
}

void ctrlprocess(void)
{
	unsigned char ii;
	char status;
	
	PcdReset();
	
	status=PcdRequest(PICC_REQALL,&RevBuffer[0]);//寻天线区内未进入休眠状态的卡，返回卡片类型 2字节
	
	if(status!=MI_OK)
	{
		return;
	}
	status=PcdAnticoll(&RevBuffer[2]);//防冲撞，返回卡的序列号 4字节
	if(status!=MI_OK)
	{
		return;
	}
	memcpy(MLastSelectedSnr,&RevBuffer[2],4); 
	status=PcdSelect(MLastSelectedSnr);//选卡
	if(status!=MI_OK)
	{
		return;
	}
  if(oprationcard != 0)
  {
   if(oprationcard==READCARD)//读卡片数据
  {
	oprationcard=0;	
    status=PcdAuthState(PICC_AUTHENT1A,KuaiN,PassWd,MLastSelectedSnr);//
    if(status!=MI_OK)
    {
      bWarn=1;
      return;
    }
    status=PcdRead(KuaiN,Read_Data);
    if(status!=MI_OK)
    {
      bWarn=1;
      return;
    }
		SendBuffer[A_MsgLen] = 0x13;
		SendBuffer[A_MsgID_0] = 0xA1;
		SendBuffer[A_MsgID_1] = 0x00;
		SendBuffer[A_MsgType] = 0xA1;
	
		SendBuffer[9] = KuaiN;
		SendBuffer[10] = 0x10;
		for(ii=0;ii<16;ii++)
	    {
	      SendBuffer[ii + 11] = Read_Data[ii];
	    }
		SendBuffer[27] = XOR_verify(SendBuffer,27);

		UartTX_Send_String(SendBuffer, 28);
    bPass=1;
    PcdHalt();
  }
  else if(oprationcard==WRITECARD)//写卡
  {
     oprationcard=0;
    status=PcdAuthState(PICC_AUTHENT1A,KuaiN,PassWd,MLastSelectedSnr);//
    if(status!=MI_OK)
    {
      bWarn=1;
      return;
    }
    status=PcdWrite(KuaiN,&WriteData[0]);
    if(status!=MI_OK)
    {
      bWarn=1;
      return;
    }	
		SendBuffer[A_MsgLen] = 0x03;
		SendBuffer[A_MsgID_0] = 0xA2;
		SendBuffer[A_MsgID_1] = 0x00;
		SendBuffer[A_MsgType] = 0xA2;
	
		SendBuffer[9] = KuaiN;
		SendBuffer[10] = 0x00;
		SendBuffer[11] = XOR_verify(SendBuffer,11);

		UartTX_Send_String(SendBuffer, 12);
    bPass=1;
    PcdHalt();	
  } 
  else if(oprationcard==SENDID)//发送卡号
  {
     oprationcard=0;
		//发送卡号
		SendBuffer[A_MsgLen] = 0x06;
		SendBuffer[A_MsgID_0] = 0xA0;
		SendBuffer[A_MsgID_1] = 0x00;
		SendBuffer[A_MsgType] = 0xA0;
	
		SendBuffer[9] = 0x04;
		for(ii=0;ii<4;ii++)
	    {
	      SendBuffer[ii + 10] = MLastSelectedSnr[ii];
	    }
		SendBuffer[14] = XOR_verify(SendBuffer,14);

		UartTX_Send_String(SendBuffer, 15);
    bPass=1;

  }
  else if(oprationcard==CONSUME)//消费
  {
   	oprationcard=0;
    status=PcdAuthState(PICC_AUTHENT1A,KuaiN,PassWd,MLastSelectedSnr);
    if(status!=MI_OK)
    {
	      bWarn=1;
	      return;
	}	 
	status = PcdValue(PICC_DECREMENT, KuaiN, ConsumeData);
    if(status!=MI_OK)
    {
      bWarn=1;
      return;
    }

	status=PcdRead(KuaiN,Read_Data);
    if(status!=MI_OK)
    {
      bWarn=1;
      return;
    }
		 
	SendBuffer[A_MsgLen] = 0x07;
	SendBuffer[A_MsgID_0] = 0xA3;
	SendBuffer[A_MsgID_1] = 0x00;
	SendBuffer[A_MsgType] = 0xA3;

	SendBuffer[9] = KuaiN;
	SendBuffer[10] = 0x04;
	for(ii=0;ii<4;ii++)
    {
      SendBuffer[ii + 11] = Read_Data[ii];
    }
	SendBuffer[15] = XOR_verify(SendBuffer,15);		

	UartTX_Send_String(SendBuffer, 16);
	bPass=1;
    PcdHalt();
	
  }
	  else if(oprationcard==ADDMONEY)//充值
	  {
		oprationcard=0;	
	    status=PcdAuthState(PICC_AUTHENT1A,KuaiN,PassWd,MLastSelectedSnr);
	    if(status!=MI_OK)
	    {
	      return;
	    }
		status = PcdValue(PICC_INCREMENT, KuaiN, RechargeData);
	    if(status!=MI_OK)
	    {
	      bWarn=1;
          return;
        }
		status=PcdRead(KuaiN,Read_Data);
		if(status!=MI_OK)
		{
		  bWarn=1;
		  return;
		}
		SendBuffer[A_MsgLen] = 0x07;
		SendBuffer[A_MsgID_0] = 0xA4;
		SendBuffer[A_MsgID_1] = 0x00;
		SendBuffer[A_MsgType] = 0xA4;
	
		SendBuffer[9] = KuaiN;
		SendBuffer[10] = 0x04;
		for(ii=0;ii<4;ii++)
	    {
	      SendBuffer[ii + 11] = Read_Data[ii];
	    }
		SendBuffer[15] = XOR_verify(SendBuffer,15);		

		UartTX_Send_String(SendBuffer, 16);		
	    bPass=1;
	    PcdHalt();

	  }
	  	//发送跟踪解析帧
#ifdef DEBUG
		//寻卡 向RC522的FIFO写入数据，该数据将通过RC522发送给卡片
		SendBuffer[A_MsgLen] = 0x05;
		SendBuffer[A_MsgID_0] = 0xA6;
		SendBuffer[A_MsgID_1] = 0x00;
		SendBuffer[A_MsgType] = 0xA6;
	
		SendBuffer[9] = 0x02;
		SendBuffer[10] = 0x09;
		SendBuffer[11] = 0x01;
		SendBuffer[12] = 0x52;
		SendBuffer[13] = XOR_verify(SendBuffer,13);
	
		UartTX_Send_String(SendBuffer, 14);
		//寻卡 向RC522命令寄存器写命令
		SendBuffer[A_MsgLen] = 0x04;
		SendBuffer[A_MsgID_0] = 0xA6;
		SendBuffer[A_MsgID_1] = 0x00;
		SendBuffer[A_MsgType] = 0xA6;
		SendBuffer[9] = 0x01;
		SendBuffer[10] = 0x01;
		SendBuffer[11] = PCD_TRANSCEIVE;
		SendBuffer[12] = XOR_verify(SendBuffer,12);

		UartTX_Send_String(SendBuffer, 13);
		//寻卡 从RC522的FIFO读数据，该数据为卡片发送给RC522的数据
		SendBuffer[A_MsgLen] = 0x06;
		SendBuffer[A_MsgID_0] =  0xA6;
		SendBuffer[A_MsgID_1] =  0x00;
		SendBuffer[A_MsgType] =  0xA6;
	
		SendBuffer[9] = 0x03;
		SendBuffer[10] = 0x09;
		SendBuffer[11] = 0x02;
		SendBuffer[12] = RevBuffer[0];
		SendBuffer[13] = RevBuffer[1];
		SendBuffer[14] = XOR_verify(SendBuffer,14); 

		UartTX_Send_String(SendBuffer, 15);
		//防冲撞 向RC522的FIFO写入数据，该数据将通过RC522发送给卡片
		SendBuffer[A_MsgLen] = 0x06;
		SendBuffer[A_MsgID_0] =  0xA7;
		SendBuffer[A_MsgID_1] =  0x00;
		SendBuffer[A_MsgType] =  0xA7;
	
		SendBuffer[9] = 0x02;
		SendBuffer[10] = 0x09;
		SendBuffer[11] = 0x02;
		SendBuffer[12] = PICC_ANTICOLL1;
		SendBuffer[13] = 0x20;
		SendBuffer[14] = XOR_verify(SendBuffer,14);

		UartTX_Send_String(SendBuffer, 15);
		//防冲撞 向RC522命令寄存器写命令
		SendBuffer[A_MsgLen] = 0x04;
		SendBuffer[A_MsgID_0] = 0xA7;
		SendBuffer[A_MsgID_1] = 0x00;
		SendBuffer[A_MsgType] = 0xA7;
		SendBuffer[9] = 0x01;
		SendBuffer[10] = 0x01;
		SendBuffer[11] = PCD_TRANSCEIVE;
		SendBuffer[12] = XOR_verify(SendBuffer,12);

		UartTX_Send_String(SendBuffer, 13);
		//防冲撞 从RC522的FIFO读数据，该数据为卡片发送给RC522的数据
		SendBuffer[A_MsgLen] = 0x08;
		SendBuffer[A_MsgID_0] =  0xA7;
		SendBuffer[A_MsgID_1] =  0x00;
		SendBuffer[A_MsgType] =  0xA7;
	
		SendBuffer[9] = 0x03;
		SendBuffer[10] = 0x09;
		SendBuffer[11] = 0x04;
		for(ii=0;ii<4;ii++)
	    {
	      SendBuffer[ii + 12] = MLastSelectedSnr[ii];
	    }
		SendBuffer[16] = XOR_verify(SendBuffer,16);

		UartTX_Send_String(SendBuffer, 17);
		//选卡 向RC522的FIFO写入数据，该数据将通过RC522发送给卡片
		SendBuffer[A_MsgLen] = 0x0D;
		SendBuffer[A_MsgID_0] =  0xA8;
		SendBuffer[A_MsgID_1] =  0x00;
		SendBuffer[A_MsgType] =  0xA8;
	
		SendBuffer[9] = 0x02;
		SendBuffer[10] = 0x09;
		SendBuffer[11] = 0x09;
		SendBuffer[12] = PICC_ANTICOLL1;
		SendBuffer[13] = 0x70;
		SendBuffer[18] = 0;
	    for (ii=0; ii<4; ii++)
	    {
	    	SendBuffer[ii+14] = *(MLastSelectedSnr+ii);
	    	SendBuffer[18]  ^= *(MLastSelectedSnr+ii);
	    }
		
	//pengcuiyue???
	//    CalulateCRC(&SendBuffer[12],7,&SendBuffer[19]);
		SendBuffer[21] = XOR_verify(SendBuffer,21);
	
		UartTX_Send_String(SendBuffer, 22);
		//选卡 向RC522命令寄存器写命令
		SendBuffer[A_MsgLen] = 0x04;
		SendBuffer[A_MsgID_0] = 0xA8;
		SendBuffer[A_MsgID_1] = 0x00;
		SendBuffer[A_MsgType] = 0xA8;
		SendBuffer[9] = 0x01;
		SendBuffer[10] = 0x01;
		SendBuffer[11] = PCD_TRANSCEIVE;
		SendBuffer[12] = XOR_verify(SendBuffer,12);

		UartTX_Send_String(SendBuffer, 13);
#endif
    }   					
}

void ctrl_uart(void)
{
	uchar ii = 0;
	if((RevBuffer[0] == 0x00) && (RevBuffer[2] == 0x00) && (RevBuffer[3] == 0x01))
	{
		SendBuffer[A_MsgLen] = 0x00;
		SendBuffer[A_DevType] = 0x01;
		SendBuffer[A_MsgID_0] = 0x00;
		SendBuffer[A_MsgID_1] = 0x10;
		SendBuffer[8] = XOR_verify(SendBuffer,8);
	
		UartTX_Send_String(SendBuffer, 9);
	}
	if((RevBuffer[0] != 0x00) && (RevBuffer[3] == RevBuffer[8]))
	{
	    //UartTX_Send_String(uart_test, 4);  //调试用
		switch(RevBuffer[8])
		{
			case 0xa0:
				oprationcard=SENDID;
				break;
			case 0xa1://读数据
				oprationcard=READCARD;
				for(ii=0;ii<6;ii++)
				{
					PassWd[ii]=RevBuffer[ii+9];
				} 
		  		KuaiN=RevBuffer[15];
		  		break;
			case 0xa2://写数据
				oprationcard=WRITECARD;
				for(ii=0;ii<6;ii++)
				{
					PassWd[ii]=RevBuffer[ii+9];
				} 
		  		KuaiN=RevBuffer[15];
				for(ii=0;ii<16;ii++)
				{
					WriteData[ii]=RevBuffer[ii+17];
				}
				break;  
			case 0xa3:          //消费
				oprationcard=CONSUME;
				for(ii=0;ii<6;ii++)
				{
					PassWd[ii]=RevBuffer[ii+9];
				} 
				KuaiN=RevBuffer[15];
				for(ii=0;ii<4;ii++)
				{
					ConsumeData[ii]=RevBuffer[ii+17];
				} 		
				break;
			case 0xa4:          //充值
				oprationcard=ADDMONEY;
				for(ii=0;ii<6;ii++)
				{
					PassWd[ii]=RevBuffer[ii+9];
				} 
				KuaiN=RevBuffer[15];
				for(ii=0;ii<4;ii++)
				{
					RechargeData[ii]=RevBuffer[ii+17];
				}
				break;
			default:
				break;                     
		}
	}
	for(ii = 0; ii < 35; ii++)
	{
		RevBuffer[ii] = 0;
	}
	uart_comp=0;
	uart_count=0;
}
