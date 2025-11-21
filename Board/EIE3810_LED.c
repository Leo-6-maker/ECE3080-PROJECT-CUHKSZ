#include "stm32f10x.h"
#include "EIE3810_LED.h"


void EIE3810_LED_Init(void)
{
	// APB2 peripheral clock enable
	RCC->APB2ENR |= 1 << 3; // port B
	RCC->APB2ENR |= 1 << 6; // port E
	// LED0 (PB5) settings
	// CNF: general purpose output push-pull (00)
	// Mode: output mode, max speed 50 MHz (11)
	GPIOB->CRL &= 0xFF0FFFFF; // reset the value of [20: 24], for pos 5
	GPIOB->CRL |= 0x00300000; // set the value of [20: 24] to 0b0011
	// BSRR: set the port (1)
	GPIOB->BSRR |= 1 << 5; // set the value of [5: 6] to 0b1
	// LED1 (PE5) settings
	// CNF: general purpose output push-pull (00)
	// Mode: output mode, max speed 50 MHz (11)
	GPIOE->CRL &= 0xFF0FFFFF; // reset the value of [20: 24], for pos 5
	GPIOE->CRL |= 0x00300000; // set the value of [20: 24] to 0b0011
	// BSRR: set the port (1)
	GPIOE->BSRR |= 1 << 5; // set the value of [5: 6] to 0b1
}

void EIE3810_TurnOn_LED0(void)
{
	GPIOB->BRR |= 1 << 5; // LED0 on
}

void EIE3810_TurnOff_LED0(void)
{
	GPIOB->BSRR |= 1 << 5; // LED0 off
}

void EIE3810_Toggle_LED1(void)
{
	if ((GPIOE->IDR & (1 << 5)) != 0) // LED1 currently off
	{
		GPIOE->BRR |= 1 << 5; // LED1 on
	}
	else // LED1 currently on
	{
		GPIOE->BSRR |= 1 << 5; // LED1 off
	}
}
