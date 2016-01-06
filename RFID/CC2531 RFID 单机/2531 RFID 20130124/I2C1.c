#include "include.h"

void Init_I2C()
{
	P0DIR |= 0x03;
	SDA = 1;
	SCL = 1;
}

void Delay_1u(unsigned int microSecs) 
{
	while(microSecs--)
	{
		/* 32 NOPs == 1 usecs */
		asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
		asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
		asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
		asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
		asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
		asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
		asm("nop"); asm("nop");
	}
}

void WriteSDA1(void)//SDA ���1   
{
	P0DIR |= 0x02;
	SDA = 1;
}

void WriteSDA0(void)//SDA ���0    
{
	P0DIR |= 0x02;
	SDA = 0;
}

void WriteSCL1(void)//SCL ���1    
{
	P0DIR |= 0x01;
	SCL = 1;
}
    
void WriteSCL0(void)//SCL ���0    
{
	P0DIR |= 0x01;
	SCL = 0;
}

void ReadSDA(void)//��������SDA��ӦIO��DIR���Խ�������    
{
	P0DIR &= 0xFD;
}

/*����I2C���ߵĺ�������SCLΪ�ߵ�ƽʱʹSDA����һ��������*/       
void I2C_Start(void)
{
    WriteSDA1();
    WriteSCL1();
    Delay_1u(50);
    WriteSDA0();
    Delay_1u(50);
    WriteSCL0();
    Delay_1u(25);
}

/*��ֹI2C���ߣ���SCLΪ�ߵ�ƽʱʹSDA����һ��������*/
void I2C_Stop(void)
{
    WriteSCL0();
	WriteSDA0();
	Delay_1u(25);
    WriteSCL1();
    Delay_1u(50);
    WriteSDA1();
	Delay_1u(100);
}

/*����0����SCLΪ�ߵ�ƽʱʹSDA�ź�Ϊ��*/
void SEND_0(void)
{
    WriteSDA0();
    WriteSCL0();
    Delay_1u(25);
    WriteSCL1();
    Delay_1u(50);
	WriteSCL0();
    Delay_1u(25);
}

/*����1����SCLΪ�ߵ�ƽʱʹSDA�ź�Ϊ��*/
void SEND_0_bit8(void)
{
    WriteSDA0();
    WriteSCL0();
    Delay_1u(25);
    WriteSCL1();
    Delay_1u(50);
	WriteSCL0();
	ReadSDA();
    Delay_1u(25);
}

/*����1����SCLΪ�ߵ�ƽʱʹSDA�ź�Ϊ��*/
void SEND_1(void)
{
    WriteSDA1();
    WriteSCL0();
    Delay_1u(25);
    WriteSCL1();
    Delay_1u(50);
	WriteSCL0();
    Delay_1u(25);
}

/*����1����SCLΪ�ߵ�ƽʱʹSDA�ź�Ϊ��*/
void SEND_1_bit8(void)
{
    WriteSDA1();
    WriteSCL0();
    Delay_1u(25);
    WriteSCL1();
    Delay_1u(50);
	WriteSCL0();
	ReadSDA();
    Delay_1u(25);
}

/*������һ���ֽں�����豸��Ӧ���ź�*/    
char Check_Acknowledge(void)
{
    ReadSDA();
    WriteSCL0();
    Delay_1u(25);
    WriteSCL1();
    Delay_1u(25);   
    F0=SDA;
    Delay_1u(25);
    WriteSCL0();
	WriteSDA0();
    Delay_1u(25);
	Delay_1u(20);//�鿴����ʱʹ�ã����ڸ����ηֶΣ����鿴����ʱ�����԰Ѵ˾�ע����
    if(F0==1)
	{
        return FALSE;
	}
	else
	{
    	return TRUE;
	}
}

void Write_Acknowledge_1(void)
{
    WriteSDA1();
    WriteSCL0();
    Delay_1u(25);
    WriteSCL1();
    Delay_1u(50);
	WriteSCL0();
    Delay_1u(25);
}

/*��I2C����дһ���ֽ�*/
void WriteI2CByte(char b)
{
    char temp = 0;
	char i;
	temp = b;
	for(i=0;i<7;i++)
	{
		if((b<<i)&0x80)
		{
			SEND_1();
		}
		else
		{
			SEND_0();
		}
	}
	if((temp & 0x01) != 0)
	{
		SEND_1_bit8();		
	}
	else
	{
		SEND_0_bit8();
	}
}

/*��I2C���߶�һ���ֽ�*/
char ReadI2CByte(void)
{
    char b=0,i;
	ReadSDA();
    for(i=0;i<8;i++)
    {   
		WriteSCL0();
		Delay_1u(25);
		WriteSCL1();
		Delay_1u(25);   
		F0=SDA;   //�Ĵ����е�һλ,���ڴ洢SDA�е�һλ����
		Delay_1u(25);
		WriteSCL0();
		Delay_1u(25);
		if(F0==1)
		{
			b=b<<1;
			b=b|0x01;
		}
		else
		{
			b=b<<1;
		}
    }
    return b; 
}

/////////////////////////////////////////////////////////////////////
//��    �ܣ���RC522�Ĵ���
//����˵����reg[IN]:�Ĵ�����ַ
//��    �أ�������ֵ
/////////////////////////////////////////////////////////////////////
unsigned char ReadRawRC(char reg)
{
    unsigned char ucResult=0;
	I2C_Start();
	
	WriteI2CByte(MFRC522_WRITE_ADDRESS);
	while(Check_Acknowledge() == FALSE);
	
	WriteI2CByte(reg);
	while(Check_Acknowledge() == FALSE);
	
	I2C_Start();
	
	WriteI2CByte(MFRC522_READ_ADDRESS);
	while(Check_Acknowledge() == FALSE);
	
	ucResult = ReadI2CByte();
	Write_Acknowledge_1();
	
	I2C_Stop();
	return ucResult;  
}

/////////////////////////////////////////////////////////////////////
//��    �ܣ�дRC632�Ĵ���
//����˵����reg[IN]:�Ĵ�����ַ
//          data[IN]:д���ֵ
/////////////////////////////////////////////////////////////////////
void WriteRawRC(char reg, char data)
{
	I2C_Start();
        
	WriteI2CByte(MFRC522_WRITE_ADDRESS);
	while(Check_Acknowledge() == FALSE);
        
	WriteI2CByte(reg);
	while(Check_Acknowledge() == FALSE);
	
	WriteI2CByte(data);
	while(Check_Acknowledge() == FALSE);
	
	I2C_Stop();
}
