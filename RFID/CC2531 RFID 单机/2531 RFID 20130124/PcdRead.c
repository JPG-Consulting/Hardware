#include "include.h"

/////////////////////////////////////////////////////////////////////
//��    �ܣ���ȡM1��һ������
//����˵��: addr[IN]�����ַ
//          pData[OUT]�����������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK
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
	// ����Ƭ���� ��RC522��FIFOд�����ݣ������ݽ�ͨ��RC522���͸���Ƭ
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
	//����Ƭ���� ��RC522����Ĵ���д����
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
	//����Ƭ���� ��RC522��FIFO�����ݣ�������Ϊ��Ƭ���͸�RC522������
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
