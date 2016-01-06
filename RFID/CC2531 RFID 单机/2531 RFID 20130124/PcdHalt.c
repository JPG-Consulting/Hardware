#include "include.h"

/////////////////////////////////////////////////////////////////////
//功    能：命令卡片进入休眠状态
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdHalt(void)
{
    char status;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
	unsigned char i = 0;

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
#ifdef DEBUG
	// 挂起卡片 向RC522的FIFO写入数据，该数据将通过RC522发送给卡片
	SendBuffer[A_MsgLen] = 0x06;
	SendBuffer[A_MsgID_0] = 0xAC;
	SendBuffer[A_MsgID_1] = 0x00;
	SendBuffer[A_MsgType] = 0xAC;

	SendBuffer[9] = 0x02;
	SendBuffer[10] = 0x09;
	SendBuffer[11] = 0x02;
	SendBuffer[12] = PICC_HALT;
	SendBuffer[13] = 0x00;
	SendBuffer[14] = XOR_verify(SendBuffer,14);
	
	UartTX_Send_String(SendBuffer, 15);
	//挂起卡片 向RC522命令寄存器写命令
	SendBuffer[A_MsgLen] = 0x04;
	SendBuffer[A_MsgID_0] = 0xAC;
	SendBuffer[A_MsgID_1] = 0x00;
	SendBuffer[A_MsgType] = 0xAC;
	SendBuffer[9] = 0x01;
	SendBuffer[10] = 0x01;
	SendBuffer[11] = PCD_TRANSCEIVE;
	SendBuffer[12] = XOR_verify(SendBuffer,12);

	UartTX_Send_String(SendBuffer, 13);
#endif
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    return MI_OK;
}
