#include "include.h"

extern uchar result1;

//////////////////////////////////////////////////////////////////////
//设置RC522的工作方式 
//////////////////////////////////////////////////////////////////////
char M522PcdConfigISOType(unsigned char type)
{
	if (type == 'A')                     //ISO14443_A
	{ 
		ClearBitMask(Status2Reg,0x08);
		WriteRawRC(ModeReg,0x3D);//3F
		WriteRawRC(RxSelReg,0x86);//84
		WriteRawRC(RFCfgReg,0x7F);   //4F
		WriteRawRC(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
		WriteRawRC(TReloadRegH,0);
		WriteRawRC(TModeReg,0x8D);
		WriteRawRC(TPrescalerReg,0x3E);
		
		result1 = ReadRawRC(ModeReg);
		result1 = ReadRawRC(RxSelReg);
		result1 = ReadRawRC(RFCfgReg);
		result1 = ReadRawRC(TReloadRegL);
		result1 = ReadRawRC(TReloadRegH);
		result1 = ReadRawRC(TModeReg);
		result1 = ReadRawRC(TPrescalerReg);
		
		delay_ns(1000);
		PcdAntennaOn();
	}
	else{ return -1; }
	
	return MI_OK;
}
