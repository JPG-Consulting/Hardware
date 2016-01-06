#include "include.h"

/////////////////////////////////////////////////////////////////////
//功    能：扣款和充值
//参数说明: dd_mode[IN]：命令字
//               0xC0 = 扣款
//               0xC1 = 充值
//          addr[IN]：钱包地址
//          pValue[IN]：4字节增(减)值，低位在前
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////                 
char PcdValue(unsigned char dd_mode,unsigned char addr,unsigned char *pValue)
{
    char status;
    unsigned int  unLen;
	unsigned char i = 0;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    //unsigned char i;
	
    ucComMF522Buf[0] = dd_mode;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);

    #ifdef DEBUG
	// 扣款或充值 向RC522的FIFO写入数据，该数据将通过RC522发送给卡片
	SendBuffer[A_MsgLen] = 0x08;
	SendBuffer[A_MsgID_0] = dd_mode;
	SendBuffer[A_MsgID_1] = 0x00;
	SendBuffer[A_MsgType] = dd_mode;

	SendBuffer[9] = 0x02;
	SendBuffer[10] = 0x09;
	SendBuffer[11] = 0x04;
    for(i=0;i<4;i++)
    {
      SendBuffer[i+12] = ucComMF522Buf[i];
    }
	SendBuffer[16] = XOR_verify(SendBuffer,16);

	UartTX_Send_String(SendBuffer, 17);
	// 扣款或充值 向RC522命令寄存器写命令
	SendBuffer[A_MsgLen] = 0x04;
	SendBuffer[A_MsgID_0] = dd_mode;
	SendBuffer[A_MsgID_1] = 0x00;
	SendBuffer[A_MsgType] = dd_mode;
	SendBuffer[9] = 0x01;
	SendBuffer[10] = 0x01;
	SendBuffer[11] = PCD_TRANSCEIVE;
	SendBuffer[12] = XOR_verify(SendBuffer,12);
	
	UartTX_Send_String(SendBuffer, 13);
    #endif
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        memcpy(ucComMF522Buf, pValue, 4);
        //for (i=0; i<16; i++)
        //{    ucComMF522Buf[i] = *(pValue+i);   }
        CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
        unLen = 0;

	    #ifdef DEBUG
		// 扣款或充值 向RC522的FIFO写入数据，该数据将通过RC522发送给卡片
		SendBuffer[A_MsgLen] = 0x0A;
		SendBuffer[A_MsgID_0] = dd_mode;
		SendBuffer[A_MsgID_1] = 0x00;
		SendBuffer[A_MsgType] = dd_mode;
	
		SendBuffer[9] = 0x02;
		SendBuffer[10] = 0x09;
		SendBuffer[11] = 0x06;
	    for(i=0;i<6;i++)
	    {
	      SendBuffer[i+12] = ucComMF522Buf[i];
	    }
		SendBuffer[18] = XOR_verify(SendBuffer,18);

		UartTX_Send_String(SendBuffer, 19);	
		// 扣款或充值 向RC522命令寄存器写命令
		SendBuffer[A_MsgLen] = 0x04;
		SendBuffer[A_MsgID_0] = dd_mode;
		SendBuffer[A_MsgID_1] = 0x00;
		SendBuffer[A_MsgType] = dd_mode;
		SendBuffer[9] = 0x01;
		SendBuffer[10] = 0x01;
		SendBuffer[11] = PCD_TRANSCEIVE;
		SendBuffer[12] = XOR_verify(SendBuffer,12);

		UartTX_Send_String(SendBuffer, 13);
	    #endif

        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
		if (status != MI_ERR)
        {    status = MI_OK;    }
    }
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = PICC_TRANSFER;
        ucComMF522Buf[1] = addr;
        CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);

		#ifdef DEBUG 
		// 扣款或充值 向RC522的FIFO写入数据，该数据将通过RC522发送给卡片
		SendBuffer[A_MsgLen] = 0x08;
		SendBuffer[A_MsgID_0] = dd_mode;
		SendBuffer[A_MsgID_1] = 0x00;
		SendBuffer[A_MsgType] = dd_mode;
	
		SendBuffer[9] = 0x02;
		SendBuffer[10] = 0x09;
		SendBuffer[11] = 0x04;
	    for(i=0;i<4;i++)
	    {
	      SendBuffer[i+12] = ucComMF522Buf[i];
	    }
		SendBuffer[16] = XOR_verify(SendBuffer,16);

		UartTX_Send_String(SendBuffer, 17);	
		// 扣款或充值 向RC522命令寄存器写命令
		SendBuffer[A_MsgLen] = 0x04;
		SendBuffer[A_MsgID_0] = dd_mode;
		SendBuffer[A_MsgID_1] = 0x00;
		SendBuffer[A_MsgType] = dd_mode;
		SendBuffer[9] = 0x01;
		SendBuffer[10] = 0x01;
		SendBuffer[11] = PCD_TRANSCEIVE;
		SendBuffer[12] = XOR_verify(SendBuffer,12);

		UartTX_Send_String(SendBuffer, 13);
	    #endif
		  
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    return status;
}