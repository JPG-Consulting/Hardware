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

void WriteSDA1(void)//SDA 输出1   
{
     P0DIR |= 0x02;
     SDA = 1;
}

void WriteSDA0(void)//SDA 输出0    
{
     P0DIR |= 0x02;
     SDA = 0;
}

void WriteSCL1(void)//SCL 输出1    
{
         P0DIR |= 0x01;
         SCL = 1;
}
    
void WriteSCL0(void)//SCL 输出0    
{
         P0DIR |= 0x01;
         SCL = 0;
}

void ReadSDA(void)//这里设置SDA对应IO口DIR可以接收数据    
{
     P0DIR &= 0xFD;
}

/*启动I2C总线的函数，当SCL为高电平时使SDA产生一个负跳变*/       
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

/*终止I2C总线，当SCL为高电平时使SDA产生一个正跳变*/
void I2C_Stop(void)
{
    WriteSDA0();
    Delay_1u(5);
    WriteSCL1();
    Delay_1u(5);
    WriteSDA1();
}

/*发送0，在SCL为高电平时使SDA信号为低*/
void SEND_0(void)
{
    WriteSDA0();
    WriteSCL1();
    Delay_1u(5);
    WriteSCL0();
    Delay_1u(5);
}

/*发送1，在SCL为高电平时使SDA信号为高*/
void SEND_1(void)
{
    WriteSDA1();
    WriteSCL1();
    Delay_1u(5);
    WriteSCL0();
    Delay_1u(5);
}

/*发送完一个字节后检验设备的应答信号*/    
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

/*向I2C总线写一个字节*/
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

/*从I2C总线读一个字节*/
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
        F0=SDA;//寄存器中的一位,用于存储SDA中的一位数据

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
//功    能：读RC522寄存器
//参数说明：reg[IN]:寄存器地址
//返    回：读出的值
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
//功    能：写RC632寄存器
//参数说明：reg[IN]:寄存器地址
//          data[IN]:写入的值
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

