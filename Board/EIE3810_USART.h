#ifndef __EIE3810_USART_H
#define __EIE3810_USART_H
#include "stm32f10x.h"


void EIE3810_USART1_init(u32 pclk2, u32 baudrate);
void EIE3810_USART2_init(u32 pclk1, u32 baudrate);
void EIE3810_USART1_EXTIInit(void);

void USART_print(u8 USARTport, char *st);

#endif
