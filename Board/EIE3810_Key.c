#include "stm32f10x.h"
#include "EIE3810_KEY.h"


void EIE3810_Key_Init(void)
{
    // APB2 peripheral clock enable
    RCC->APB2ENR |= 1 << 2; // port A
    RCC->APB2ENR |= 1 << 6; // port E
    // Key2 (PE2) settings
    // CNF: input with pull-up/pull-down (10)
    // Mode: input mode (reset state) (00)
    GPIOE->CRL &= 0xFFFFF0FF; // reset the value of [8: 12], for pos 2
    GPIOE->CRL |= 0x00000800; // set the value of [8: 12] to 0b1000
    // ODR: select pull-up (1)
    GPIOE->ODR &= 0xFFFFFFFB; // reset the value of [0: 4] to 0bx0xx, for pos 2
    GPIOE->ODR |= 1 << 2;     // set the value of [2: 3] to 0b1
    // Key1 (PE3) settings
    // CNF: input with pull-up/pull-down (10)
    // Mode: input mode (reset state) (00)
    GPIOE->CRL &= 0xFFFF0FFF; // reset the value of [12: 16], for pos 3
    GPIOE->CRL |= 0x00008000; // set the value of [12: 16] to 0b1000
    // ODR: select pull-up (1)
    GPIOE->ODR &= 0xFFFFFFF7; // reset the value of [0: 4] to 0b0xxx, for pos 3
    GPIOE->ODR |= 1 << 3;     // set the value of [3: 4] to 0b1
    // Key_Up (PA0) settings
    // CNF: input with pull-up/pull-down (10)
    // Mode: input mode (reset state) (00)
    GPIOA->CRL &= 0xFFFFFFF0; // reset the value of [0: 4], for pos 0
    GPIOA->CRL |= 0x00000008; // set the value of [0: 4] to 0b1000
    // ODR: select pull-down (0)
    GPIOA->ODR &= 0xFFFFFFFE; // reset the value of [0: 4] to 0bxxx0, for pos 0
}

void EIE3810_Key2_EXTIInit(void)
{
    /* enable PE2 port */
    RCC->APB2ENR |= 1 << 6;   // enable the clock of GPIOE
    GPIOE->CRL &= 0xFFFFF0FF; // reset the port 2 to 0b0000
    GPIOE->CRL |= 0x00000800; // set the port 2 to 0b1000, input with pull-up/pull-down
    GPIOE->ODR |= 1 << 2;      // select input pull-up
    RCC->APB2ENR |= 0x01;     // enable the clock of AFIO
    /* set external interrupt config register in AFIO for PE2 */
    AFIO->EXTICR[0] &= 0xFFFFF0FF; // reset the EXTI2 to 0b0000
    AFIO->EXTICR[0] |= 0x00000400; // set EXTI2 to 0b0100, slect PE[2] pin
    /* set interrupt mask register of EXTI2 */
    EXTI->IMR |= 1 << 2; // set MR2 to 0b1, interrupt from Line 2 is not masked
    /* set falling trigger selection register of EXTI2 */
    EXTI->FTSR |= 1 << 2; // set TR2 to 0b1, falling trigger enabled for line 2
    // EXTI->RTSR |= 1 << 2; // set TR2 to 0b1, rising trigger enabled for line 2
    /* set interrupt priority register of EXTI2 */
    NVIC->IP[8] = 0x65; // set the priority of EXTI2 to 0b0110_0101
    /* set interrupt set-enable register to enable EXTI2 */
    NVIC->ISER[0] |= (1 << 8); // the EXTI2's interrupt pos is #8, enable it by setting it
}

void EIE3810_Key1_EXTIInit(void)
{
    /* enable PE3 port */
    RCC->APB2ENR |= 1 << 6;   // enable the clock of GPIOE
    GPIOE->CRL &= 0xFFFF0FFF; // reset the port 3 to 0b0000
    GPIOE->CRL |= 0x00008000; // set the port 3 to 0b1000, input with pull-up/pull-down
    GPIOE->ODR |= 1 << 3;      // select input pull-up
    RCC->APB2ENR |= 0x01;     // enable the clock of AFIO
    /* set external interrupt config register in AFIO for PE3 */
    AFIO->EXTICR[0] &= 0xFFFF0FFF; // reset the EXTI3 to 0b0000
    AFIO->EXTICR[0] |= 0x00004000; // set EXTI3 to 0b0100, slect PE[3] pin
    /* set interrupt mask register of EXTI3 */
    EXTI->IMR |= 1 << 3; // set MR3 to 0b1, interrupt from Line 3 is not masked
    /* set falling trigger selection register of EXTI3 */
    EXTI->FTSR |= 1 << 3; // set TR3 to 0b1, falling trigger enabled for line 3
    // EXTI->RTSR |= 1 << 3; // set TR3 to 0b1, rising trigger enabled for line 3
    /* set interrupt priority register of EXTI3 */
    NVIC->IP[9] = 0x65; // set the priority of EXTI3 to 0b0110_0101
    /* set interrupt set-enable register to enable EXTI3 */
    NVIC->ISER[0] |= (1 << 9); // the EXTI3's interrupt pos is #9, enable it by setting it
}

void EIE3810_Key0_EXTIInit(void)
{
    /* enable PE4 port */
    RCC->APB2ENR |= 1 << 6;   // enable the clock of GPIOE
    GPIOE->CRL &= 0xFFF0FFFF; // reset the port 4 to 0b0000
    GPIOE->CRL |= 0x00080000; // set the port 4 to 0b1000, input with pull-up/pull-down
    GPIOE->ODR |= 1 << 4;      // select input pull-up
    RCC->APB2ENR |= 0x01;     // enable the clock of AFIO
    /* set external interrupt config register in AFIO for PE4 */
    AFIO->EXTICR[1] &= 0xFFFFFFF0; // reset the EXTI4 to 0b0000
    AFIO->EXTICR[1] |= 0x00000004; // set EXTI4 to 0b0100, slect PE[4] pin
    /* set interrupt mask register of EXTI4 */
    EXTI->IMR |= 1 << 4; // set MR4 to 0b1, interrupt from Line 4 is not masked
    /* set falling trigger selection register of EXTI4 */
    EXTI->FTSR |= 1 << 4; // set TR4 to 0b1, falling trigger enabled for line 4
    // EXTI->RTSR |= 1 << 4; // set TR4 to 0b1, rising trigger enabled for line 4
    /* set interrupt priority register of EXTI4 */
    NVIC->IP[10] = 0x65; // set the priority of EXTI4 to 0b0110_0101
    /* set interrupt set-enable register to enable EXTI4 */
    NVIC->ISER[0] |= (1 << 10); // the EXTI4's interrupt pos is #10, enable it by setting it

}


void EIE3810_KeyUp_EXTIInit(void)
{
    /* enable PA0 port */
    RCC->APB2ENR |= 1 << 2;   // enable the clock of GPIOA
    GPIOA->CRL &= 0xFFFFFFF0; // reset the port 0 to 0b0000
    GPIOA->CRL |= 0x00000008; // set the port 0 to 0b1000, input with pull-up/pull-down
    GPIOA->ODR &= 0xFFFFFFFE; // select pull-down
    RCC->APB2ENR |= 0x01;     // enable the clock of AFIO
    /* set external interrupt config register in AFIO for PA0 */
    AFIO->EXTICR[0] &= 0xFFFFFFF0; // reset the EXTI0 to 0b0000
    //AFIO->EXTICR[0] |= 0x00000000; // set EXTI0 to 0b0000, slect PA[0] pin
    /* set interrupt mask register of EXTI0 */
    EXTI->IMR |= 0x01; // set MR0 to 0b1, interrupt from Line 0 is not masked
    /* set falling trigger selection register of EXTI0 */
    // EXTI->FTSR |= 1; // set TR0 to 0b1, falling trigger enabled for line 0
    EXTI->RTSR |= 0x01; // set TR0 to 0b1, rising trigger enabled for line 0
    /* set interrupt priority register of EXTI2 */
    NVIC->IP[6] = 0x75; // set the priority of EXTI0 to 0b0111_0101
    /* set interrupt set-enable register to enable EXTI2 */
    NVIC->ISER[0] |= (1 << 6); // the EXTI0's interrupt pos is #6, enable it by setting it
}

void EIE3810_FourKeys_EXTIInit(void)
{
    EIE3810_Key2_EXTIInit();
    EIE3810_Key1_EXTIInit();
    EIE3810_Key0_EXTIInit();
    EIE3810_KeyUp_EXTIInit();
}

u16 EIE3810_Read_Key2_IDR(void)
{
    return GPIOE->IDR & (1 << 2); // 0 for closed 1 for open
}

u16 EIE3810_Read_Key1_IDR(void)
{
    return GPIOE->IDR & (1 << 3); // 0 for closed 1 for open
}

u16 EIE3810_Read_Key0_IDR(void)
{
    return GPIOE->IDR & (1 << 4); // 0 for closed 1 for open
}

u16 EIE3810_Read_Key_Up_IDR(void)
{
    return GPIOA->IDR & (1); // 1 for closed 0 for open
}