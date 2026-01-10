#include <stdint.h>

// RCC RCC_AHB1ENR RCC_APB2ENR Enable---------------------------------------------------

#define RCC_BASE 0x40023800UL
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB2ENR (*(volatile uint32_t *)(RCC_BASE + 0x44))

// GPIOA ------------------------------------------------------------------------------

#define GPIOA_BASE 0x40020000UL
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR (*(volatile uint32_t *)(GPIOA_BASE + 0x14))
#define GPIOA_BSRR (*(volatile uint32_t *)(GPIOA_BASE + 0x18))
#define GPIOA_IDR (*(volatile uint32_t *)(GPIOA_BASE + 0x10))
#define GPIO_BSRR_RESET 16U

// SYSCFG------------------------------------------------------------------------------

#define SYSCFG_BASE 0x40013800
#define SYSCFG_EXTICR1 (*(volatile uint32_t *)(SYSCFG_BASE + 0x08))

// EXTI-------------------------------------------------------------------------------

#define EXTI_BASE 0x40013C00UL
#define EXTI_IMR (*(volatile uint32_t *)(EXTI_BASE + 0x00))
#define EXTI_RTSR (*(volatile uint32_t *)(EXTI_BASE + 0x08))
#define EXTI_PR (*(volatile uint32_t *)(EXTI_BASE + 0x14))

// NVIC------------------------------------------------------------------------------

#define NVIC_ISER0 (*(volatile uint32_t *)(0xE000E100UL))

#define SYST_CSR (*(volatile uint32_t *)(0xE000E010UL))
#define SYST_RVR (*(volatile uint32_t *)(0xE000E014UL))
#define SYST_CVR (*(volatile uint32_t *)(0xE000E018UL))

#define CLK_FRQ 16000000UL // Using STM32F411 CPU Clock
#define LOAD_VAL (CLK_FRQ / 1000) - 1

#define SYST_CSR_ENABLE_CLKSOURCE 5

// LED BUTTON ----------------------------------------------------------------------

#define LED_PIN 1
#define LED_PIN_1 3
#define BUTTON 2

void SysTimer_Init(void)
{
    SYST_RVR = LOAD_VAL;
    SYST_CVR = 0;

    SYST_CSR &= ~SYST_CSR_ENABLE_CLKSOURCE;
    SYST_CSR |= SYST_CSR_ENABLE_CLKSOURCE;
}

void EXTI2_IRQHandler(void)
{
    if (GPIOA_ODR & (1 << LED_PIN))
    {
        GPIOA_BSRR = 1 << (LED_PIN + GPIO_BSRR_RESET);
    }

    else
    {
        GPIOA_BSRR = 1<<LED_PIN;
    }

    EXTI_PR = 1 << BUTTON;
}

void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++)
    {
        while (((SYST_CSR >> 16) & 1) == 0)
            ;
    }
}

int main(void)
{
    RCC_AHB1ENR |= 1 << 0;
    RCC_APB2ENR |= 1 << 14;

    GPIOA_MODER &= ~(3 << (2 * BUTTON));
    GPIOA_MODER &= ~(3 << (2 * LED_PIN));
    GPIOA_MODER |= (1 << (2 * LED_PIN));

    GPIOA_MODER &= ~(3 << (2 * LED_PIN_1));
    GPIOA_MODER |= (1 << (2 * LED_PIN_1));

    SYSCFG_EXTICR1 &= ~(0xF << 8);

    EXTI_IMR |= 1 << BUTTON;
    EXTI_RTSR |= 1 << BUTTON;

    NVIC_ISER0 |= 1 << 8;

    while (1)
    {
        if (GPIOA_ODR & (1 << LED_PIN_1))
        {
            GPIOA_BSRR = 1 << (LED_PIN_1 + GPIO_BSRR_RESET);
        }

        else
        {
            GPIOA_BSRR = 1<<LED_PIN_1;
        }

        delay_ms (1000);
    }
}


