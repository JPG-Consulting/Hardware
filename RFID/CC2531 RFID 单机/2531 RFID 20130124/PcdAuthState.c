#include "include.h"

/////////////////////////////////////////////////////////////////////
//功    能：验证卡片密码
//参数说明: auth_mode[IN]: 密码验证模式
//                 0x60 = 验证A密钥
//                 0x61 = 验证B密钥 
//          addr[IN]：块地址
//          pKey[IN]：密码
//          pSnr[IN]：卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////               
char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr)
{
    char status;
    unsigned int unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<4; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
 //   memcpy(&ucComMF522Buf[2], pKey, 6); 
 //   memcpy(&ucComMF522Buf[8], pSnr, 4); 
    
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
	//如果Status2Reg的bit3为1，则表明成功执行密码校验。如果bit3为0，应返回错误。
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }

#ifdef DEBUG    
	//密码认证 向RC522的FIFO写入数据，该数据将通过RC522发送给卡片
	SendBuffer[A_MsgLen] = 0x10;
	SendBuffer[A_MsgID_0] = 0xA9;
	SendBuffer[A_MsgID_1] = 0x00;
	SendBuffer[A_MsgType] = 0xA9;

	SendBuffer[9] = 0x02;
	SendBuffer[10] = 0x09;
	SendBuffer[11] = 0x0C;
	SendBuffer[12] = auth_mode;
	SendBuffer[13] = addr;
    for (i=0; i<6; i++)
    {    SendBuffer[i+14] = *(pKey+i);   }
    for (i=0; i<4; i++)
    {    SendBuffer[i+20] = *(pSnr+i);   }
	SendBuffer[24] = XOR_verify(SendBuffer,24);
	
	UartTX_Send_String(SendBuffer, 25);
	
	//密码认证 向RC522命令寄存器写命令
	SendBuffer[A_MsgLen] = 0x04;
	SendBuffer[A_MsgID_0] = 0xA9;
	SendBuffer[A_MsgID_1] = 0x00;
	SendBuffer[A_MsgType] = 0xA9;
	SendBuffer[9] = 0x01;
	SendBuffer[10] = 0x01;
	SendBuffer[11] = PCD_AUTHENT;
	SendBuffer[12] = XOR_verify(SendBuffer,12);

	UartTX_Send_String(SendBuffer, 13);
#endif
    return status;
}
