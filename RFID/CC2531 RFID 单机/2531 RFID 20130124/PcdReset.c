#include "include.h"

uchar result1 = 0;
/////////////////////////////////////////////////////////////////////
//功    能：复位RC522
//返    回: 成功返回MI_OK
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
    
    WriteRawRC(ModeReg,0x3D);            //和Mifare卡通讯，CRC初始值0x6363

    WriteRawRC(TReloadRegL,30); 

    WriteRawRC(TReloadRegH,0);

    WriteRawRC(TModeReg,0x8D);
	
    WriteRawRC(TPrescalerReg,0x3E);

	WriteRawRC(TxAutoReg,0x40);//必须要
	
    return MI_OK;
}
