// 4 bit binary counter with led and Push Button EXTI STM32F446xx

#include <stdint.h>

#define RCC_BASE 0x40023800UL
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB2ENR (*(volatile uint32_t *)(RCC_BASE + 0x44))

#define GPIOA_BASE 0x40020000UL
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_IDR (*(volatile uint32_t *)(GPIOA_BASE + 0x10))
#define GPIOA_BSRR (*(volatile uint32_t *)(GPIOA_BASE + 0x18))


// SYSCFG------------------------------------------------------------------------------

#define SYSCFG_BASE 0x40013800UL
#define SYSCFG_EXTICR2 (*(volatile uint32_t *)(SYSCFG_BASE + 0x0C))

// EXTI-------------------------------------------------------------------------------

#define EXTI_BASE 0x40013C00UL
#define EXTI_IMR (*(volatile uint32_t *)(EXTI_BASE + 0x00))
#define EXTI_RTSR (*(volatile uint32_t *)(EXTI_BASE + 0x08))
#define EXTI_PR (*(volatile uint32_t *)(EXTI_BASE + 0x14))

// NVIC------------------------------------------------------------------------------

#define NVIC_ISER0 (*(volatile uint32_t *)(0xE000E100UL))

#define CLK_FRQ 16000000UL
#define LOAD_VAL (CLK_FRQ / 1000) - 1

#define LED_RST_MASK 0xF // PA0- PA3 led connected

#define PA0 0
#define PA1 1
#define PA2 2
#define PA3 3

#define Button_Pin 4

uint8_t counter = 0;

void EXTI4_IRQHandler(void)
{

    GPIOA_BSRR = LED_RST_MASK << 16;
    GPIOA_BSRR = counter;
    counter++;

    if (counter > 15)
    {
        counter = 0;
    }

    EXTI_PR |= 1 << Button_Pin;
}

int main(void)
{

    RCC_AHB1ENR |= 1 << 0;
    RCC_APB2ENR |= 1 << 14;

    GPIOA_MODER &= ~(3 << (PA0 * 2));
    GPIOA_MODER &= ~(3 << (PA1 * 2));
    GPIOA_MODER &= ~(3 << (PA2 * 2));
    GPIOA_MODER &= ~(3 << (PA3 * 2));

    GPIOA_MODER |= (1 << (PA0 * 2));
    GPIOA_MODER |= (1 << (PA1 * 2));
    GPIOA_MODER |= (1 << (PA2 * 2));
    GPIOA_MODER |= (1 << (PA3 * 2));

    GPIOA_MODER &= ~(3 << (Button_Pin * 2));

    SYSCFG_EXTICR2 &= ~(0xF << 4);

    EXTI_IMR |= 1 << Button_Pin;
    EXTI_RTSR |= 1 << Button_Pin;

    NVIC_ISER0 |= 1 << 10;

    while (1)
    {
    }
}
