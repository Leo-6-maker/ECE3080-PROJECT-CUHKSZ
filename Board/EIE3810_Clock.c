#include "stm32f10x.h"
#include "EIE3810_Clock.h"


void EIE3810_clock_tree_init(void)
{
    u8 PLL = 7;
    u8 temp = 0;
    RCC->CR |= 0x00010000;           // set HSEON to 1 (enable HSE clock)
    while (!((RCC->CR >> 17) & 0x1)) // while HSERDY = 0 (external high-speed clock ready flag) = 0
        ;                            // loop until the HSERDY = 1 (HSERDY is ready)
    RCC->CFGR &= 0xFFFDFFFF;         // set PLLXTPRE to 0 (HSE clock not divided)
    RCC->CFGR |= 1 << 16;            // set PLLSRC to 1 (HSE oscillator clock selected as PLL input clock)
    RCC->CFGR |= PLL << 18;          // set PLLMUL[3:0] to x111 (PLL input clock x 9)
    RCC->CR |= 0x01000000;           // set PLLON to 1 (enable PLL)
    while (!(RCC->CR >> 25))         // while PLLRDY = 0 (PLL clock ready flag = 0)
        ;                            // loop until the PLLRDY = 1 (PLLRDY is ready)
    RCC->CFGR &= 0xFFFFFFFE;         // set SW[1:0] to x0
    RCC->CFGR |= 0x00000002;         // set SW[1:0] to 1x ( (PLL selected as system clock))
    while (temp != 0x02)             // while SWS[1:0] != 10 (PLL used as system clock)
    {
        temp = RCC->CFGR >> 2; // temp selected [31:2]
        temp &= 0x03;          // temp selected [3:2]
    }
    RCC->CFGR &= 0xFFFFDC0F; // set HPRE[3:0] to 0000 (SYSCLK not divided)
                             //   PPRE2[2:0] to 0xx (HCLK not divided), PPRE1[2:0] to x00
    RCC->CFGR |= 0x00000400; // set PRE1[2:0] to 1xx (HCLK divided by 2)
    FLASH->ACR = 0x32;       // set FLASH with 2 wait states
    RCC->APB1ENR |= 1 << 17; // set USART2 to 1 (enable USART2 clock)
    RCC->APB2ENR |= 1 << 14; // set USART1 to 1 (enable USART1 clock)
}