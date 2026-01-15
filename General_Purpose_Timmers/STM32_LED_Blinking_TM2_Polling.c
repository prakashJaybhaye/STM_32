/*------------------------------------------------------------------------
1	Start by locating the RCC base address from the STM32F411 memory map.
2	From the RCC base, locate the RCC_AHB1ENR register offset (0x30) and compute its address.
3	Enable the GPIOA peripheral clock by setting bit 0 in RCC_AHB1ENR.
4	Locate the GPIOA base address from the memory map.
5	From GPIOA base, locate the GPIOA_MODER register offset (0x00).
6	Clear the two MODER bits corresponding to PA3 (3 × 2).
7	Set those two bits to 01 to configure PA3 as a general purpose output.
8	Locate the RCC_APB1ENR register offset (0x40) from the RCC base.
9	Enable the TIM2 peripheral clock by setting bit 0 in RCC_APB1ENR.
10	Locate the TIM2 base address from the memory map.
11	From TIM2 base, locate the TIM2_PSC register offset (0x28).
12	Calculate the prescaler value so that 16 MHz / 16000 = 1000 Hz, giving 1 ms per timer tick.
13	Write the calculated value into TIM2_PSC.
14	From TIM2 base, locate the TIM2_EGR register offset (0x14).
15	Set bit 0 (UG – Update Generation) in TIM2_EGR to force the prescaler value to load immediately into the active counter logic.
16	Enter the infinite loop.
17	Read the current state of PA3 from GPIOA_ODR.
18	If the PA3 bit is 1 (LED ON), write 1 to bit (3 + 16) in GPIOA_BSRR to reset PA3 and turn the LED OFF.
19	If the PA3 bit is 0 (LED OFF), write 1 to bit 3 in GPIOA_BSRR to set PA3 and turn the LED ON.
20	Disable TIM2 by clearing bit 0 (CEN) in TIM2_CR1.
21	Load the required delay value (for example 1000) into TIM2_ARR.
22	Reset the timer counter by writing 0 into TIM2_CNT.
23	Set bit 0 (UG) in TIM2_EGR again to transfer ARR value into the active register and reset the internal counter logic.
24	Enable TIM2 by setting bit 0 (CEN) in TIM2_CR1.
25	Continuously poll bit 0 (UIF) in TIM2_SR until it becomes 1, indicating the timer has overflowed and the delay period has completed.
26	Clear the UIF flag by writing 0 to bit 0 in TIM2_SR.
27	Disable TIM2 again by clearing bit 0 (CEN) in TIM2_CR1.
28	Repeat from step 17 to continuously toggle the LED with the specified delay.
--------------------------------------------------------------------------------------------------------------------------
*/



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
    TIM2_PSC = (HSI_CLK / 16000) - 1;
    TIM2_EGR |= (1 << 0);
}



void delay(uint32_t ms)
{
    TIM2_CR1 &= ~(1 << 0);
    TIM2_ARR = ms;
    TIM2_CNT = 0;
    TIM2_EGR |= (1 << 0);
    TIM2_CR1 |= (1 << 0);

    while ((TIM2_SR & 1) == 0);

    TIM2_SR &= ~(1 << 0);
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

        delay(1000);
    }
    
}
