

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
#define TIM2_DIER (*(volatile uint32_t *)(TIM2_BASE + 0x0C))

#define NVIC_ISER0 (*(volatile uint32_t *)(0xE000E100UL))

#define GPIOA_BASE 0x40020000UL
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR (*(volatile uint32_t *)(GPIOA_BASE + 0x14))
#define GPIOA_BSRR (*(volatile uint32_t *)(GPIOA_BASE + 0x18))

#define HSI_CLK 16000000U

#define LED_PA3 3

#define LED_ON (GPIOA_BSRR = (1 << LED_PA3))
#define LED_OFF (GPIOA_BSRR = (1 << (LED_PA3 + 16)))

volatile uint8_t TM2_flag = 0;

void Init_Timer_TM_2(void)
{
    RCC_APB1ENR |= (1 << 0);
    TIM2_PSC = (HSI_CLK / 16000) - 1;
    TIM2_EGR |= (1 << 0);
    TIM2_DIER |= 1 << 0;
}

void delay(uint32_t ms)
{
    TIM2_SR &= ~(1 << 0);
    TIM2_CR1 &= ~(1 << 0);
    TIM2_ARR = ms-1;
    TIM2_CNT = 0;
    TIM2_EGR |= (1 << 0);
    TIM2_CR1 |= (1 << 0);
}

void TIM2_IRQHandler(void)
{

    TIM2_SR &= ~(1 << 0);
    TIM2_CR1 &= ~(1 << 0);

    TM2_flag = 1;
}

int main(void)
{
    RCC_AHB1ENR |= (1 << 0);
    GPIOA_MODER &= ~(3 << (LED_PA3 * 2));
    GPIOA_MODER |= (1 << (LED_PA3 * 2));

    NVIC_ISER0 |= 1 << 28;

    Init_Timer_TM_2();

    delay(1000);

    while (1)
    {
        if (TM2_flag == 1)
        {
            if (GPIOA_ODR & (1 << LED_PA3))
            {
                LED_OFF;
            }

            else
            {
                LED_ON;
            }
            TM2_flag = 0;
            delay(1000);
        }
    }
}
