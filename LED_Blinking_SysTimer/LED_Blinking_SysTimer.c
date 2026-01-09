#include <stdint.h>
// Led PA3 Blinking code using SysTimer Clock Black Pill

// SysTick Registers-------------------------------------------------------------------
#define SYST_CSR (*(volatile uint32_t *)(0xE000E010UL))
#define SYST_RVR (*(volatile uint32_t *)(0xE000E014UL))
#define SYST_CVR (*(volatile uint32_t *)(0xE000E018UL))

#define CLK_FRQ 16000000UL // Using STM32F411 CPU Clock
#define LOAD_VAL (CLK_FRQ / 1000) - 1

#define SYST_CSR_ENABLE_CLKSOURCE 5

// RCC & AHB1 Enable------------------------------------------------------------------

#define RCC_BASE 0x40023800UL
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30))

// GPIOA------------------------------------------------------------------------------

#define GPIOA_BASE 0x40020000UL
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR (*(volatile uint32_t *)(GPIOA_BASE + 0x14))
#define GPIOA_BSRR (*(volatile uint32_t *)(GPIOA_BASE + 0x18))

#define GPIOA_BSRR_RESET 16

#define LED_PA3_PIN 3

// Systimer_Init---------------------------------------------------------------------

void Systimer_Init(void)
{
    SYST_RVR = LOAD_VAL;
    SYST_CVR = 0;

    SYST_CSR &= ~SYST_CSR_ENABLE_CLKSOURCE;
    SYST_CSR |= SYST_CSR_ENABLE_CLKSOURCE;
}

// Delay------------------------------------------------------------------------------

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
    Systimer_Init();

    RCC_AHB1ENR |= 1 << 0;

    GPIOA_MODER &= ~(3 << (2 * LED_PA3_PIN));
    GPIOA_MODER |= (1 << (2 * LED_PA3_PIN));

    while (1)
    {
        if (GPIOA_ODR & (1 << LED_PA3_PIN))
        {
            GPIOA_BSRR = (1 << (LED_PA3_PIN + GPIOA_BSRR_RESET));
        }

        else
        {
            GPIOA_BSRR = 1 << LED_PA3_PIN;
        }

        delay_ms(1000);
    }
}
