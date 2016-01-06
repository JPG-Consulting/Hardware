#ifndef __KEY_H__
#define __KEY_H__

#define YLED P1_1	//定义LED2为P11口控制
#define beep P0_7

#define  CLR_BEEP  beep=0
#define  SET_BEEP  beep=1

#define  N_1  1
#define  N_2  2
#define  N_3  3
#define  N_4  4
#define  N_NOP  255

#define  SENDID     0xA0
#define  READCARD   0xA1
#define  WRITECARD  0xA2
#define  CONSUME    0xA3
#define  ADDMONEY   0xA4

#define Low_MsgLength  0
#define High_MsgLength 1

#define  RELOAD_COUNT  0xfb    //18.432M  9600  0xfb
                               //         4800  0xf6 
#define A_MsgLen     0 
#define A_DevType    1
#define A_MsgID_0    2
#define A_MsgID_1    3
#define A_seqID_0    4
#define A_seqID_1    5
#define A_seqID_2    6
#define A_seqID_3    7

#define A_MsgType    8

#define DevType      0x00

//#define DEBUG 1

uchar GetKey(void);

#endif
