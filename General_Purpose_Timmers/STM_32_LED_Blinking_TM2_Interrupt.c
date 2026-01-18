/*-----------------------------------------------------------------------------------------------------
1. Locate the RCC base address (0x40023800) from the STM32F411 memory map.
2. From the RCC base, locate the RCC_AHB1ENR register at offset 0x30.
3. Enable the GPIOA peripheral clock by setting bit 0 in RCC_AHB1ENR.
4. Locate the GPIOA base address (0x40020000) from the memory map.
5. From the GPIOA base, locate the GPIOA_MODER register at offset 0x00.
6. Clear the two MODER bits corresponding to pin PA3 (bits 7:6).
7. Set the MODER bits for PA3 to 01 to configure PA3 as a general-purpose output.
8. From the RCC base, locate the RCC_APB1ENR register at offset 0x40.
9. Enable the TIM2 peripheral clock by setting bit 0 in RCC_APB1ENR.
10. Locate the TIM2 base address (0x40000000) from the memory map.
11. From the TIM2 base, locate the TIM2_PSC register at offset 0x28.
12. Calculate the prescaler value so that the 16 MHz timer clock is divided by 1000,
    resulting in 1 kHz timer frequency (1 ms per timer tick).
13. Write the calculated prescaler value into TIM2_PSC.
14. From the TIM2 base, locate the TIM2_ARR register at offset 0x2C.
15. Set TIM2_ARR = 1 - 1 to configure 1 ms auto-reload for timer tick.
16. From the TIM2 base, locate the TIM2_DIER register at offset 0x0C.
17. Enable the update interrupt (UIE) by setting bit 0 in TIM2_DIER.
18. From the NVIC base (0xE000E100), enable TIM2 IRQ in NVIC by setting bit 28 in NVIC_ISER0.
19. From the TIM2 base, locate the TIM2_CR1 register at offset 0x00.
20. Enable the timer by setting bit 0 (CEN) in TIM2_CR1.
21. The timer now starts running and will generate an update interrupt every 1 ms.
22. When TIM2 update event occurs, TIM2_IRQHandler() is executed:
    - Clear the update interrupt flag (UIF) by writing 0 to bit 0 of TIM2_SR.
    - Increment the global millisecond counter ms_counter.
23. In the main loop, read the current output state of PA3 from GPIOA_ODR.
24. If PA3 is high (LED ON), write 1 to bit LED_PA3 + 16 in GPIOA_BSRR to turn LED OFF.
25. If PA3 is low (LED OFF), write 1 to bit LED_PA3 in GPIOA_BSRR to turn LED ON.
26. Call the delay(ms) function to wait for the specified number of milliseconds:
    - Record the current ms_counter value in start_time.
    - Continuously poll ms_counter until (ms_counter - start_time) >= ms.
27. After the delay, the main loop repeats from step 23 to continuously toggle the LED
    at 1-second intervals.
------------------------------------------------------------------------------------------------------------------*/


#include <stdint.h>

#define RCC_BASE 0x40023800UL
#define RCC_APB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x40))
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30))

#define TIM2_BASE 0x40000000UL
#define TIM2_CR1 (*(volatile uint32_t *)(TIM2_BASE + 0x00))
#define TIM2_SR (*(volatile uint32_t *)(TIM2_BASE + 0x10))
#define TIM2_DIER (*(volatile uint32_t *)(TIM2_BASE + 0x0C))
#define TIM2_CNT (*(volatile uint32_t *)(TIM2_BASE + 0x24))
#define TIM2_PSC (*(volatile uint32_t *)(TIM2_BASE + 0x28))
#define TIM2_ARR (*(volatile uint32_t *)(TIM2_BASE + 0x2C))
#define TIM2_EGR (*(volatile uint32_t *)(TIM2_BASE + 0x14))

#define NVIC_ISER0 (*(volatile uint32_t *)(0xE000E100UL))

#define GPIOA_BASE 0x40020000UL
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR (*(volatile uint32_t *)(GPIOA_BASE + 0x14))
#define GPIOA_BSRR (*(volatile uint32_t *)(GPIOA_BASE + 0x18))

#define HSI_CLK 16000000U
#define LED_PA3 3

#define LED_ON  (GPIOA_BSRR = (1 << LED_PA3))
#define LED_OFF (GPIOA_BSRR = (1 << (LED_PA3 + 16)))

volatile uint32_t ms_counter = 0;

void TIM2_IRQHandler(void)
{
    TIM2_SR &= ~(1 << 0);
    ms_counter++;
}

void Init_GPIOA(void)
{
    RCC_AHB1ENR |= 1 << 0;
    GPIOA_MODER &= ~(3 << (LED_PA3 * 2));
    GPIOA_MODER |= 1 << (LED_PA3 * 2);
}

void Init_TIM2(void)
{
    RCC_APB1ENR |= 1 << 0;
    TIM2_PSC = (HSI_CLK / 1000) - 1;
    TIM2_ARR = 1 - 1;
    TIM2_DIER |= 1 << 0;
    NVIC_ISER0 |= 1 << 28;
    TIM2_CR1 |= 1 << 0;
}

void delay(uint32_t ms)
{
    uint32_t start_time = ms_counter;
    while ((ms_counter - start_time) < ms);
}

int main(void)
{
    Init_GPIOA();
    Init_TIM2();

    while (1)
    {
        if (GPIOA_ODR & (1 << LED_PA3))
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
