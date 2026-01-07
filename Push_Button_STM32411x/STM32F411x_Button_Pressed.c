/*
------------------------------------------------------------------------------------------------------------
1. Locate the RCC base address from the STM32 reference manual memory map.
2. Locate the RCC_AHB1ENR register offset and compute its address as: RCC_BASE + 0x30.
3. Enable the GPIOC peripheral clock by setting bit 2 in RCC_AHB1ENR (PC port is bit 2).
4. Locate the GPIOC base address from the memory map.
5. Locate the GPIOC_MODER register offset and compute its address as: GPIOC_BASE + 0x00.
6. Configure the LED pin (PC13) as a general-purpose output:
7. Clear the two MODER bits for PC13 (13 × 2)
8. Set the MODER bits to 01 to select output mode.
9. Configure the Button pin (PC14) as a general-purpose input:
10. Clear the two MODER bits for PC14 (14 × 2)
11. Leave them as 00 for input mode.
12. Locate the GPIOC_IDR (Input Data Register) and GPIOC_BSRR (Bit Set/Reset Register) offsets for pin control.
13. Inside an infinite loop, read the button state from GPIOC_IDR:
14. If the button (PC14) is pressed (IDR bit HIGH), set the LED (PC13) HIGH using the lower half of BSRR.
15. If the button is not pressed (IDR bit LOW), reset the LED (PC13) using the upper half of BSRR (bit +16).
16. Repeat this read button → control LED sequence continuously
--------------------------------------------------------------------------------------------------------------
*/

#include <stdint.h>

#define RCC_BASE 0x40023800UL
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30))

#define GPIOC_BASE 0x40020800UL
#define GPIOC_MODER (*(volatile uint32_t *)(GPIOC_BASE + 0x00))
#define GPIOC_BSRR (*(volatile uint32_t *)(GPIOC_BASE + 0x18))
#define GPIOC_IDR (*(volatile uint32_t *)(GPIOC_BASE + 0x10))

#define LED_PIN 13
#define PUSH_BUTTON 14

int main(void)
{
    RCC_AHB1ENR |= 1 << 2;

    GPIOC_MODER &= ~(3 << (2 * LED_PIN));
    GPIOC_MODER |= (1 << (2 * LED_PIN));

    GPIOC_MODER &= ~(3 << (2 * PUSH_BUTTON));

    while (1)
    {
        if (GPIOC_IDR & (1 << PUSH_BUTTON))
        {
            GPIOC_BSRR = 1 << LED_PIN;
        }

        else
        {
            GPIOC_BSRR = 1 << (LED_PIN + 16);
        }
    }
}
