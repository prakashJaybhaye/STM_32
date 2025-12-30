#include <stdint.h>

/* ================= RCC ================= */
/* RCC base address (RM0383, Memory map) */
#define RCC_BASE        0x40023800UL

/* RCC AHB1 peripheral clock enable register
 * Offset: 0x30 (RM0383, RCC register map)
 */
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30))

/* ================= GPIOC ================= */
/* GPIOC base address (RM0383, Memory map) */
#define GPIOC_BASE      0x40020800UL

/* GPIOC mode register
 * Offset: 0x00 (RM0383, GPIO register map)
 */
#define GPIOC_MODER     (*(volatile uint32_t *)(GPIOC_BASE + 0x00))

/* GPIOC output data register
 * Offset: 0x14 (RM0383, GPIO register map)
 */
#define GPIOC_ODR       (*(volatile uint32_t *)(GPIOC_BASE + 0x14))

/* ================= Simple delay ================= */
void delay(void)
{
    for (volatile uint32_t i = 0; i < 100000; i++);
}

/* ================= Main ================= */
int main(void)
{
    /* 1. Enable GPIOC clock
     * RCC_AHB1ENR bit 2 = GPIOCEN
     */
    RCC_AHB1ENR |= (1 << 2);

    /* 2. Configure PC13 as output
     * Each pin uses 2 bits in MODER
     * PC13 → bits 26 & 27
     * 01 = General purpose output
     */
    GPIOC_MODER &= ~(3 << (13 * 2));  // Clear mode bits
    GPIOC_MODER |=  (1 << (13 * 2));  // Set output mode

    while (1)
    {
        /* 3. Toggle PC13
         * Note: Black Pill LED is usually ACTIVE-LOW
         * 0 = LED ON, 1 = LED OFF
         */
        GPIOC_ODR ^= (1 << 13);

        /* 4. Delay */
        delay();
    }
}
