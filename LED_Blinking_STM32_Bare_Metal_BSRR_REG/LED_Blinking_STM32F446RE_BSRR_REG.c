/*
------------------------------------------------------------
STM32 (Bare-Metal) – Toggle External LED on PB15
Using ODR (read) + BSRR (write)

1. Locate RCC base address from the reference manual memory map.
2. Locate RCC_AHB1ENR offset and compute its address (RCC_BASE + 0x30).
3. Enable GPIOB clock by setting bit 1 in RCC_AHB1ENR.
4. Locate GPIOB base address from the memory map.
5. Locate GPIOB_MODER offset and compute its address (GPIOB_BASE + 0x00).
6. Clear the two MODER bits corresponding to PB15 (15 × 2).
7. Set the MODER bits to 01 to configure PB15 as output.
8. Locate GPIOB_ODR and GPIOB_BSRR offsets for pin control.
9. Read PB15 state from ODR and toggle it using BSRR:
   - If PB15 is HIGH, reset it (LOW)
   - If PB15 is LOW, set it (HIGH)
10. Use a software delay to control blink speed.
11. Repeat the toggle operation inside an infinite loop.
------------------------------------------------------------
*/

#include <stdint.h>

#define RCC_BASE 0x40023800UL
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define GPIOC_BASE 0x40020800UL
#define GPIOC_MODR (*(volatile uint32_t *)(GPIOC_BASE + 0x00))
#define GPIOC_ODR (*(volatile uint32_t *)(GPIOC_BASE + 0x14))
#define GPIOC_BSRR (*(volatile uint32_t *)(GPIOC_BASE + 0x18))

void delay(uint32_t n)
{
    for (volatile uint32_t i = 0; i < n; i++)
        ;
}

int main(void)
{
    RCC_AHB1ENR |= (1 << 2);

    GPIOC_MODR &= ~(3 << (7 * 2));
    GPIOC_MODR |= (1 << (7 * 2));

    while (1)
    {
        if (GPIOC_ODR & (1 << 7))
        {
            GPIOC_BSRR = 1 << (7 + 16);
        }

        else
        {
            GPIOC_BSRR = (1 << 7);
        }
        delay(1000000);
    }
}
