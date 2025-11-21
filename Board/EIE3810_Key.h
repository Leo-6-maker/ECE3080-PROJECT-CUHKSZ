#ifndef __EIE3810_KEY_H
#define __EIE3810_KEY_H
#include "stm32f10x.h"


/* initialize key 2, 1, and up */
void EIE3810_KEY_Init(void);
/* initialize key 2 interrupt */
void EIE3810_Key2_EXTIInit(void);
/* initialize key 1 interrupt */
void EIE3810_Key1_EXTIInit(void);
/* initialize key 0 interrupt */
void EIE3810_Key0_EXTIInit(void);
/* initialize key up interrupt */
void EIE3810_KeyUp_EXTIInit(void);

void EIE3810_FourKeys_EXTIInit(void);

/* read values of keys */
u16 EIE3810_Read_Key2_IDR(void); 
u16 EIE3810_Read_Key1_IDR(void);
u16 EIE3810_Read_Key0_IDR(void);
u16 EIE3810_Read_Key_Up_IDR(void);

#endif
