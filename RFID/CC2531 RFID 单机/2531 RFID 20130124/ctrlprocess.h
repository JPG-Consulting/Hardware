#ifndef __CTRLPROCESS_H__
#define __CTRLPROCESS_H__

extern uchar RevBuffer[35];
extern uchar RechargeData[4];
extern uchar ConsumeData[4];
extern uchar WriteData[16];
extern uchar Read_Data[16];
extern uchar MLastSelectedSnr[4];

extern uchar bWarn;
extern uchar bPass;
extern uint KuaiN;
extern uchar uart_comp;
extern uchar uart_count;

//extern uchar SysTime;
extern long int SysTime;

extern uchar PassWd[6];
extern uchar KeyTime;
extern uchar oprationcard;
extern uchar bSendID;
extern uint KeyNum;
extern uchar WaitTimes;

void delay_ms(uint tms);
void Warn(void);
void Pass(void);

void uart_init(void);
void InitRc522(void);

void InitAll(void);
void ctrlprocess(void);
void ctrl_uart(void);

#endif
