#include "stm32f10x.h"
#include "EIE3810_Buzzer.h"


void EIE3810_Buzzer_Init(void)
{
    // APB2 peripheral clock enable
    RCC->APB2ENR |= 1 << 3; // port B

    // Buzzer (PB8) settings
    // CNF: general purpose output push-pull (00)
    // Mode: output mode, max speed 50 MHz (11)
    GPIOB->CRH &= 0xFFFFFFF0;
    GPIOB->CRH |= 0x00000003;
    // BRR: reset the port (1)
    GPIOB->BRR |= 1 << 8; // reset the value of [8: 9] to 0b1
}

void EIE3810_Toggle_Buzzer(void)
{
    if ((GPIOB->IDR & (1 << 8)) != 0) // buzzer currently on
    {
        GPIOB->BRR |= 1 << 8; // buzzer off
    }
    else // buzzer currently off
    {
        GPIOB->BSRR |= 1 << 8; // buzzer on
    }
}