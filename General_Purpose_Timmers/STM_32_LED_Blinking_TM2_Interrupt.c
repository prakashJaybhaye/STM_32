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
