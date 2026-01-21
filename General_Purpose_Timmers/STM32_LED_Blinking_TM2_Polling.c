
/*-------------------------------------------------------------------------------------------------
1   Locate the RCC base address (0x40023800) from the STM32F411 memory map.
2   From the RCC base, locate the RCC_AHB1ENR register at offset 0x30.
3   Enable the GPIOA peripheral clock by setting bit 0 in RCC_AHB1ENR.
4   Locate the GPIOA base address (0x40020000) from the memory map.
5   From the GPIOA base, locate the GPIOA_MODER register at offset 0x00.
6   Clear the two MODER bits corresponding to pin PA3 (bits 7:6).
7   Set the MODER bits for PA3 to 01 to configure PA3 as a general-purpose output.
8   From the RCC base, locate the RCC_APB1ENR register at offset 0x40.
9   Enable the TIM2 peripheral clock by setting bit 0 in RCC_APB1ENR.
10  Locate the TIM2 base address (0x40000000) from the memory map.
11  From the TIM2 base, locate the TIM2_PSC (prescaler) register at offset 0x28.
12  Calculate the prescaler value so that a 16 MHz timer clock is divided by 16000
    resulting in a 1 kHz timer frequency (1 ms per timer tick).
13  Write the calculated prescaler value into TIM2_PSC.
14  From the TIM2 base, locate the TIM2_EGR register at offset 0x14.
15  Set bit 0 (UG – Update Generation) in TIM2_EGR to immediately load the prescaler
    value into the active timer logic.
16  Enter the infinite loop.
17  Read the current output state of PA3 from the GPIOA_ODR register.
18  If PA3 is high (LED ON), write a 1 to bit (3 + 16) in GPIOA_BSRR to reset PA3
    and turn the LED OFF.
19  If PA3 is low (LED OFF), write a 1 to bit 3 in GPIOA_BSRR to set PA3
    and turn the LED ON.
20  Disable TIM2 by clearing bit 0 (CEN) in the TIM2_CR1 register.
21  Clear the TIM2 update interrupt flag (UIF) by writing 0 to bit 0 of TIM2_SR.
22  Load the desired delay value (e.g., 1000) into the TIM2_ARR register.
23  Reset the timer counter by writing 0 into the TIM2_CNT register.
24  Set bit 0 (UG) in TIM2_EGR to transfer the ARR value to the active register
    and reset the internal counter logic.
25  Enable TIM2 by setting bit 0 (CEN) in the TIM2_CR1 register.
26  Continuously poll bit 0 (UIF) in the TIM2_SR register until it becomes 1,
    indicating that the timer has overflowed and the delay period has elapsed.
27  Clear the UIF flag by writing 0 to bit 0 of TIM2_SR.
28  Disable TIM2 again by clearing bit 0 (CEN) in TIM2_CR1.
29  Repeat from step 17 to continuously toggle the LED with the specified delay.
-------------------------------------------------------------------------------------------------*/


#include <stdint.h>

#define RCC_BASE 0x40023800UL
#define RCC_APB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x40))
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30))

#define TIM2_BASE 0x40000000UL
#define TIM2_CR1 (*(volatile uint32_t *)(TIM2_BASE + 0x00))
#define TIM2_SR (*(volatile uint32_t *)(TIM2_BASE + 0x10))
#define TIM2_EGR (*(volatile uint32_t *)(TIM2_BASE + 0x14))
#define TIM2_CNT (*(volatile uint32_t *)(TIM2_BASE + 0x24))
#define TIM2_PSC (*(volatile uint32_t *)(TIM2_BASE + 0x28))
#define TIM2_ARR (*(volatile uint32_t *)(TIM2_BASE + 0x2C))

#define HSI_CLK 16000000U

#define GPIOA_BASE 0x40020000UL
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR (*(volatile uint32_t *)(GPIOA_BASE + 0x14))
#define GPIOA_BSRR (*(volatile uint32_t *)(GPIOA_BASE + 0x18))

#define LED_PA3 3

#define LED_ON (GPIOA_BSRR = (1<<LED_PA3))
#define LED_OFF (GPIOA_BSRR = (1<<(LED_PA3 + 16)))


void Init_Timer_TM_2(void)
{
    RCC_APB1ENR |= (1 << 0);
    TIM2_PSC = (HSI_CLK/1000)-1;
    TIM2_EGR |= (1 << 0);
}



void delay(uint32_t ms)
{
    TIM2_CR1 &= ~(1 << 0);  
    TIM2_SR  &= ~(1 << 0);    

    TIM2_ARR = ms - 1;
    TIM2_CNT = 0;

    TIM2_EGR |= (1 << 0);     
    TIM2_SR  &= ~(1 << 0);    

    TIM2_CR1 |= (1 << 0);     

    while ((TIM2_SR & 1) == 0); 

    TIM2_SR  &= ~(1 << 0);    
    TIM2_CR1 &= ~(1 << 0);    
}


int main()
{
    RCC_AHB1ENR |= (1<<0);
    GPIOA_MODER &= ~(3<<(LED_PA3*2));
    GPIOA_MODER |= (1<<(LED_PA3*2));

    Init_Timer_TM_2();

    while (1)
    {
        if (GPIOA_ODR & (1<<LED_PA3))
        {
            LED_OFF;
        }

        else
        {
            LED_ON;
        }

        delay(500);
    }

}
