#include "include.h"

/////////////////////////////////////////////////////////////////////
//��    �ܣ����Ƭ��������״̬
//��    ��: �ɹ�����MI_OK
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
	// ����Ƭ ��RC522��FIFOд�����ݣ������ݽ�ͨ��RC522���͸���Ƭ
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
	//����Ƭ ��RC522����Ĵ���д����
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
