#include "stm32f10x.h"
#include "EIE3810_JOYPAD.h"


void EIE3810_JOYPAD_Init(void)
{
    RCC->APB2ENR |= 1 << 3;   // enable GPIOB clock
    RCC->APB2ENR |= 1 << 5;   // enable GPIOD clock
    GPIOB->CRH &= 0xFFFF00FF; // clear the PB11 and PB10 mode bits
    GPIOB->CRH |= 0x00003800; // set PB11 to 0b0011, general output push-pull mode ,max speed 50MHz
                              // set PB10 to 0b1000, input with pull-up / pull-down
    GPIOB->ODR |= 3 << 10;    // set ODR11, ODR10, PB10 input pull-up, PB11 general purpose output, init High
    GPIOD->CRL &= 0xFFFF0FFF; // clear the PD3 mode bits
    GPIOD->CRL |= 0x00003000; // set PD3 to 0b0011, general output push-pull, max speed 50 MHz
    GPIOD->ODR |= 1 << 3;     // PD3 init High
}

void EIE3810_JOYPAD_Delay(u16 t)
{
    while (t--)
        ;
}

u8 EIE3810_JOYPAD_Read(void)
{
    vu8 temp = 0;
    u8 t;
    GPIOB->BSRR |= 1 << 11; // set PB11 output to High
    EIE3810_JOYPAD_Delay(80);              // generate a pulse based on Delay
    GPIOB->BSRR |= 1 << 27; // reset PB11 bit to Low
    for (t = 0; t < 8; t++)
    {
        temp >>= 1;                             // divide by 2
        if ((((GPIOB->IDR) >> 10) & 0x01) == 0) // if PB10 is Low
            temp |= 0x80;                       // set temp value to 0x80 = 0b10000000
        GPIOD->BSRR |= (1 << 3);                // set PD3 output to High
        EIE3810_JOYPAD_Delay(80);                              // pulse based on Delay
        GPIOD->BSRR |= (1 << 19);               // set PD3 output to Low
        EIE3810_JOYPAD_Delay(80);                              // pulse based on Delay
    }
    return temp;
}

int EIE3810_JOYPAD_GetIndex(void)
{
    u8 PB_10 = EIE3810_JOYPAD_Read();
    if (PB_10 != 0)
    {
        int num;
        for (int i = 0; i <= 7; i++)
        {
            if (((PB_10 >> i) & 0x01) != 0)
            {
                num = i;
                return num;
            }
        }
    }
    return -1;
}