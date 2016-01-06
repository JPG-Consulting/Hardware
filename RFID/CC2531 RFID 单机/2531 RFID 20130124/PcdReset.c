#include "include.h"

uchar result1 = 0;
/////////////////////////////////////////////////////////////////////
//��    �ܣ���λRC522
//��    ��: �ɹ�����MI_OK
/////////////////////////////////////////////////////////////////////
char PcdReset(void)
{
	//PORTD|=(1<<RC522RST);
	SET_RC522RST;
    delay_ns(1);
	//PORTD&=~(1<<RC522RST);
	CLR_RC522RST;
    delay_ns(1);
	//PORTD|=(1<<RC522RST);
	SET_RC522RST;
    delay_ns(1);
    WriteRawRC(CommandReg,PCD_RESETPHASE);
    delay_ns(1);
    
    WriteRawRC(ModeReg,0x3D);            //��Mifare��ͨѶ��CRC��ʼֵ0x6363

    WriteRawRC(TReloadRegL,30); 

    WriteRawRC(TReloadRegH,0);

    WriteRawRC(TModeReg,0x8D);
	
    WriteRawRC(TPrescalerReg,0x3E);

	WriteRawRC(TxAutoReg,0x40);//����Ҫ
	
    return MI_OK;
}
