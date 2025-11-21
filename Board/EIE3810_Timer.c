#include "stm32f10x.h"
#include "EIE3810_Timer.h"


void EIE3810_TIM3_Init(u16 arr, u16 psc)
{
    // TIM3
    RCC->APB1ENR |= 1 << 1;     // enable TIM3 timer clock
    TIM3->ARR = arr;            // set the value to be loaded in the auto-reload register
    TIM3->PSC = psc;            // PSC contains the value to be loaded in the active prescaler register at each update event
    TIM3->DIER |= 1 << 0;       // enable Capture/Compare 1 DMA request
    TIM3->CR1 |= 0x01;          // enable counter
    NVIC->IP[29] = 0x45;        // set the priority of TIM3 to 0b0100_0101
    NVIC->ISER[0] |= (1 << 29); // the TIM3's interrupt pos is #29, enable it by setting it
}

void EIE3810_TIM4_Init(u16 arr, u16 psc)
{
    // TIM3
    RCC->APB1ENR |= 1 << 2;     // enable TIM4 timer clock
    TIM4->ARR = arr;            // set the value to be loaded in the auto-reload register
    TIM4->PSC = psc;            // PSC contains the value to be loaded in the active prescaler register at each update event
    TIM4->DIER |= 1 << 0;       // enable Capture/Compare 1 DMA request
    TIM4->CR1 |= 0x01;          // enable counter
    NVIC->IP[30] = 0x45;        // set the priority of TIM4 to 0b0100_0101
    NVIC->ISER[0] |= (1 << 30); // the TIM4's interrupt pos is #30, enable it by setting it
}

