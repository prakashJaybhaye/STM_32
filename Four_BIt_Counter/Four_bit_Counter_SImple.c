// 4 bit binary counter with led and Delay STM32F446xx

#include <stdint.h>

#define RCC_BASE 0x40023800UL
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30))

#define GPIOA_BASE 0x40020000UL
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_BSRR (*(volatile uint32_t *)(GPIOA_BASE + 0x18))

#define SYST_CSR (*(volatile uint32_t *)(0xE000E010UL))
#define SYST_RVR (*(volatile uint32_t *)(0xE000E014UL))
#define SYST_CVR (*(volatile uint32_t *)(0xE000E018UL))

#define CLK_FRQ 16000000UL
#define LOAD_VAL (CLK_FRQ / 1000) - 1

#define LED_RST_MASK 0xF // PA0- PA3 led connected

#define PA0 0
#define PA1 1
#define PA2 2
#define PA3 3

void SysTimer_Init(void)
{
    SYST_RVR = LOAD_VAL;
    SYST_CVR = 0;

    SYST_CSR = 0;
    SYST_CSR |= (1 << 0) | (1 << 2);
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

    GPIOA_MODER &= ~(3 << (PA0 * 2));
    GPIOA_MODER &= ~(3 << (PA1 * 2));
    GPIOA_MODER &= ~(3 << (PA2 * 2));
    GPIOA_MODER &= ~(3 << (PA3 * 2));

    GPIOA_MODER |= (1 << (PA0 * 2));
    GPIOA_MODER |= (1 << (PA1 * 2));
    GPIOA_MODER |= (1 << (PA2 * 2));
    GPIOA_MODER |= (1 << (PA3 * 2));

    SysTimer_Init();

    uint8_t counter = 0;

    while (1)
    {
        counter = 0;
        for (uint8_t i = 0; i < 16; i++)
        {
            GPIOA_BSRR = LED_RST_MASK << 16;
            GPIOA_BSRR = counter;
            delay_ms(2000); 
            counter++;
        }
    }
}
