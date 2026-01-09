/*-------------------------------------------------------------------
Locate the SysTick registers from the ARM Cortex-M4 memory map:
SYST_CSR at 0xE000E010
SYST_RVR at 0xE000E014
SYST_CVR at 0xE000E018
Define the CPU clock frequency and compute the SysTick reload value for 1 ms:
CLK_FRQ = 16 MHz
LOAD_VAL = (CLK_FRQ / 1000) - 1
Initialize SysTick in Systimer_Init() function:
Write LOAD_VAL to SYST_RVR
Reset SYST_CVR to 0
Clear the ENABLE and CLKSOURCE bits in SYST_CSR
Set the ENABLE and CLKSOURCE bits in SYST_CSR to start SysTick
Enable the GPIOA peripheral clock:
Locate RCC_BASE = 0x40023800
Locate RCC_AHB1ENR offset 0x30 → RCC_BASE + 0x30
Set bit 0 in RCC_AHB1ENR to enable GPIOA clock
Configure PA3 as general-purpose output:
Locate GPIOA_BASE = 0x40020000
Locate GPIOA_MODER offset 0x00 → GPIOA_BASE + 0x00
Clear the two MODER bits for PA3 (bits 6 and 7)
Set the MODER bits to 01 to select output mode
Enter the infinite loop to blink LED:
a. Read the current state of the LED from GPIOA_ODR (PA3)
b. If the LED is ON, reset it using the upper half of GPIOA_BSRR (bit 19 = PA3 + 16)
c. If the LED is OFF, set it using the lower half of GPIOA_BSRR (bit 3 = PA3)
d. Call delay_ms(1000) to wait 1 second using SysTick COUNTFLAG
Repeat the loop indefinitely → LED toggles every 1 second
----------------------------------------------------------------------------*/

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
