#include "include.h"

/////////////////////////////////////////////////////////////////////
//��    �ܣ���֤��Ƭ����
//����˵��: auth_mode[IN]: ������֤ģʽ
//                 0x60 = ��֤A��Կ
//                 0x61 = ��֤B��Կ 
//          addr[IN]�����ַ
//          pKey[IN]������
//          pSnr[IN]����Ƭ���кţ�4�ֽ�
//��    ��: �ɹ�����MI_OK
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
	//���Status2Reg��bit3Ϊ1��������ɹ�ִ������У�顣���bit3Ϊ0��Ӧ���ش���
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }

#ifdef DEBUG    
	//������֤ ��RC522��FIFOд�����ݣ������ݽ�ͨ��RC522���͸���Ƭ
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
	
	//������֤ ��RC522����Ĵ���д����
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
