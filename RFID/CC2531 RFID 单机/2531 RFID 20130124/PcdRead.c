#include "include.h"

/////////////////////////////////////////////////////////////////////
//功    能：读取M1卡一块数据
//参数说明: addr[IN]：块地址
//          pData[OUT]：读出的数据，16字节
//返    回: 成功返回MI_OK
///////////////////////////////////////////////////////////////////// 
char PcdRead(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
#ifdef DEBUG
	// 读卡片数据 向RC522的FIFO写入数据，该数据将通过RC522发送给卡片
	SendBuffer[A_MsgLen] = 0x08;
	SendBuffer[A_MsgID_0] = 0xAA;
	SendBuffer[A_MsgID_1] = 0x00;
	SendBuffer[A_MsgType] = 0xAA;

	SendBuffer[9] = 0x02;
	SendBuffer[10] = 0x09;
	SendBuffer[11] = 0x04;
	SendBuffer[12] = PICC_READ;
	SendBuffer[13] = addr;
    for(i=0;i<2;i++)
    {
      SendBuffer[i+14] = ucComMF522Buf[i+2];
    }
	SendBuffer[16] = XOR_verify(SendBuffer,16);

	UartTX_Send_String(SendBuffer, 17);
	//读卡片数据 向RC522命令寄存器写命令
	SendBuffer[A_MsgLen] = 0x04;
	SendBuffer[A_MsgID_0] = 0xAA;
	SendBuffer[A_MsgID_1] = 0x00;
	SendBuffer[A_MsgType] = 0xAA;
	SendBuffer[9] = 0x01;
	SendBuffer[10] = 0x01;
	SendBuffer[11] = PCD_TRANSCEIVE;
	SendBuffer[12] = XOR_verify(SendBuffer,12);

	UartTX_Send_String(SendBuffer, 13);
#endif   
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
 //   {   memcpy(pData, ucComMF522Buf, 16);   }
    {
        for (i=0; i<16; i++)
        {    *(pData+i) = ucComMF522Buf[i];   }
    }
    else
    {   status = MI_ERR;   }
#ifdef DEBUG    
	//读卡片数据 从RC522的FIFO读数据，该数据为卡片发送给RC522的数据
	SendBuffer[A_MsgLen] = 0x16;
	SendBuffer[A_MsgID_0] =  0xAA;
	SendBuffer[A_MsgID_1] =  0x00;
	SendBuffer[A_MsgType] =  0xAA;

	SendBuffer[9] = 0x03;
	SendBuffer[10] = 0x09;
	SendBuffer[11] = 0x12;
    for(i=0;i<18;i++)
    {
      SendBuffer[i+12] = ucComMF522Buf[i];
    }
	SendBuffer[30] = XOR_verify(SendBuffer,30);

	UartTX_Send_String(SendBuffer, 31);
#endif
    return status;
}
