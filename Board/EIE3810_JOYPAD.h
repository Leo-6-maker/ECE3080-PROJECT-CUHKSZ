#ifndef __EIE3810_JOYPAD_H
#define __EIE3810_JOYPAD_H
#include "stm32f10x.h"


/* initialize JOYPAD */
void EIE3810_JOYPAD_Init(void);
/* JOYPAD delay */
void EIE3810_JOYPAD_Delay(u16 t);
/* JOYPAD read */
u8 EIE3810_JOYPAD_Read(void);

int EIE3810_JOYPAD_GetIndex(void);

#endif