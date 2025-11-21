#include "stm32f10x.h"
#include "EIE3810_USART.h"


void EIE3810_USART1_init(u32 pclk2, u32 baudrate)
{
    // USART1
    float temp;
    u16 mantissa;
    u16 fraction;
    temp = (float)(pclk2 * 1000000) / (baudrate * 16);
    mantissa = temp;
    fraction = (temp - mantissa) * 16;
    mantissa <<= 4;
    mantissa += fraction;
    RCC->APB2ENR |= 1 << 2;      // enable IO port A
    GPIOA->CRH &= 0xFFFFF00F;    // set CNF, MODE of 9, 10 to 0000
    GPIOA->CRH |= 0x000008B0;    // set PA9 to 1011 (output mode with max 50MHz, alternate function output push-pull)
                                 // set PA10 to 1000 (input mode, input with pull-up / pull-down)
    RCC->APB2RSTR |= 1 << 14;    // set USART1RST to 1 (USART1 reset)
    RCC->APB2RSTR &= ~(1 << 14); // set USART1RST to 0 (No effect)
    USART1->BRR = mantissa;      // set the mantissa of USARTDIV
    USART1->CR1 = 0x2008;        // set UE = 1 (enable USART), TE = 1 (enable Transmitter), others = 0
                                 // M = 0 (1 start bit, 8 data bits, n stop bit)
                                 // PCE = 0 (parity control disabled)
                                 // PS = 0 (even parity)
                                 // PEIE, TXEIE, TCIE, RXNEIE, IDLEIE = 0 (interrupt is inhibited)
    USART1->CR1 |= 0x0024;       // set RE = 1 (enable Receiver)
                                 // set RXNEIE = 1 (RXNE interrupt enable)
}

void EIE3810_USART2_init(u32 pclk1, u32 baudrate)
{
    // USART2
    float temp;
    u16 mantissa;
    u16 fraction;
    temp = (float)(pclk1 * 1000000) / (baudrate * 16);
    mantissa = temp;
    fraction = (temp - mantissa) * 16;
    mantissa <<= 4;
    mantissa += fraction;
    RCC->APB2ENR |= 1 << 2;      // enable IO port A
    GPIOA->CRL &= 0xFFFF00FF;    // set CNF, MODE of 2, 3 to 0000
    GPIOA->CRL |= 0x00008B00;    // set PA2 to 1011 (output mode with max 50MHz, alternate function output push-pull)
                                 // set PA3 to 1000 (input mode, input with pull-up / pull-down)
    RCC->APB1RSTR |= 1 << 17;    // set USART2RST to 1 (USART2 reset)
    RCC->APB1RSTR &= ~(1 << 17); // set USART2RST to 0 (No effect)
    USART2->BRR = mantissa;      // set the mantissa of USARTDIV
    USART2->CR1 = 0x2008;        // set UE = 1 (enable USART), TE = 1 (enable Transmitter), others = 0
                                 // M = 0 (1 start bit, 8 data bits, n stop bit)
                                 // PCE = 0 (parity control disabled)
                                 // PS = 0 (even parity)
                                 // PEIE, TXEIE, TCIE, RXNEIE, IDLEIE = 0 (interrupt is inhibited)
}

void EIE3810_USART1_EXTIInit(void)
{
    NVIC->IP[37] = 0x65;       // Set USART1 priority as 0x65
    NVIC->ISER[1] |= (1 << 5); // Enable USART1 interrupt
}

void USART_print(u8 USARTport, char *st)
{
    u8 i = 0;
    while (st[i] != 0x00)
    {
        if (USARTport == 1)
        {
            USART1->DR = st[i];
            while (!((USART1->SR >> 7) & 0x1))
                ;
        }
        if (USARTport == 2)
        {
            USART2->DR = st[i];
            while (!((USART2->SR >> 7) & 0x1))
                ;
        }

        if (i == 255)
            break;
        i++;
    }
}