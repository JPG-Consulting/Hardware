#include "include.h"

void delay_ns(unsigned int ns)
{
  unsigned int i;
  for(i=0;i<ns;i++)
  {
    asm("nop");
    asm("nop");
    asm("nop");
	asm("nop");
    asm("nop");
    asm("nop");
	asm("nop");
    asm("nop");
    asm("nop");
	asm("nop");
    asm("nop");
    asm("nop");
  }
}
