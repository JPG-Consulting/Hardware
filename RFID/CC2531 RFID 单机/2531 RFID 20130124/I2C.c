#include "include.h"

void Init_I2C()
{
	P0DIR |= 0x03;
	SDA = 1;
	SCL = 1;
}

void Delay_1u(unsigned int microSecs) {
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
    Delay_1u(5);
    WriteSDA0();
    Delay_1u(5);
    WriteSCL0();
    Delay_1u(5);
}

/*��ֹI2C���ߣ���SCLΪ�ߵ�ƽʱʹSDA����һ��������*/
void I2C_Stop(void)
{
    WriteSDA0();
    Delay_1u(5);
    WriteSCL1();
    Delay_1u(5);
    WriteSDA1();
}

/*����0����SCLΪ�ߵ�ƽʱʹSDA�ź�Ϊ��*/
void SEND_0(void)
{
    WriteSDA0();
    WriteSCL1();
    Delay_1u(5);
    WriteSCL0();
    Delay_1u(5);
}

/*����1����SCLΪ�ߵ�ƽʱʹSDA�ź�Ϊ��*/
void SEND_1(void)
{
    WriteSDA1();
    WriteSCL1();
    Delay_1u(5);
    WriteSCL0();
    Delay_1u(5);
}

/*������һ���ֽں�����豸��Ӧ���ź�*/    
char Check_Acknowledge(void)
{
    WriteSDA1();
    WriteSCL1();
    Delay_1u(5);
	ReadSDA();
    F0=SDA;
    Delay_1u(5);
    WriteSCL0();
    Delay_1u(5);
    if(F0==1)
        return FALSE;
    return TRUE;
}

void Write_Acknowledge_1(void)
{
	WriteSDA1();   
	Delay_1u(5);
	WriteSCL1();   
	Delay_1u(5);
	WriteSCL0();   
	Delay_1u(5);
}

/*��I2C����дһ���ֽ�*/
void WriteI2CByte(char b)
{
    char i;
    for(i=0;i<8;i++)
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
}

/*��I2C���߶�һ���ֽ�*/
char ReadI2CByte(void)
{
    char b=0,i;
    WriteSDA1();

    for(i=0;i<8;i++)
    {   
        WriteSCL0();
        Delay_1u(5);
        WriteSCL1(); 
        Delay_1u(5);

        ReadSDA();
        F0=SDA;//�Ĵ����е�һλ,���ڴ洢SDA�е�һλ����

    if(F0==1)
        {
          b=b<<1;
          b=b|0x01;
        }
        else
          b=b<<1;
    }
    WriteSCL0();
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

