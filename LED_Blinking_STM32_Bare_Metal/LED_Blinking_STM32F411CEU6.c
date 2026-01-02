/*
----------------------------------------------------------------------------------------
1 Locate RCC base address from the reference manual memory map.
2 Locate RCC_AHB1ENR offset and compute its address using RCC_BASE + offset.
3 Enable the required GPIOx clock by setting the corresponding bit in RCC_AHB1ENR.
4 Locate GPIOx base address from the memory map.
5 Locate GPIOx_MODER offset and compute its address using GPIOx_BASE + 0x00.
6 Clear the two MODER bits for the target pin (pin × 2).
7 Set the MODER bits to 01 (output mode) for the target pin.
8 Locate GPIOx_ODR (or BSRR) offset and compute its address.
9 Toggle or set/clear the pin bit to drive the LED.
10 Add a software delay.
11 Repeat pin toggle inside an infinite loop.
----------------------------------------------------------------------------------------
*/

/*
----------------------------------------------------------------------------------------
1 Locate RCC base address from the reference manual memory map.
2 Locate RCC_AHB1ENR offset and compute its address using RCC_BASE + offset.
3 Enable the required GPIOx clock by setting the corresponding bit in RCC_AHB1ENR.
4 Locate GPIOx base address from the memory map.
5 Locate GPIOx_MODER offset and compute its address using GPIOx_BASE + 0x00.
6 Clear the two MODER bits for the target pin (pin × 2).
7 Set the MODER bits to 01 (output mode) for the target pin.
8 Locate GPIOx_ODR (or BSRR) offset and compute its address.
9 Toggle or set/clear the pin bit to drive the LED.
10 Add a software delay.
11 Repeat pin toggle inside an infinite loop.
----------------------------------------------------------------------------------------
*/

#include <stdint.h>

/* ================= RCC ================= */
#define RCC_BASE        0x40023800UL
#define RCC_AHB1ENR     (*(volatile uint32_t*)(RCC_BASE + 0x30))

/* ================= GPIOC ================= */
#define GPIOC_BASE      0x40020800UL
#define GPIOC_MODER     (*(volatile uint32_t*)(GPIOC_BASE + 0x00))
#define GPIOC_ODR       (*(volatile uint32_t*)(GPIOC_BASE + 0x14))

void Delay(uint32_t n)
{
    for (volatile uint32_t i = 0; i < n; i++);
}

int main(void)
{
    /* Enable GPIOB and GPIOC clocks */
    RCC_AHB1ENR |= (1 << 1);   // GPIOB
    RCC_AHB1ENR |= (1 << 2);   // GPIOC

    /* Configure PC14 as output */
    GPIOC_MODER &= ~(3 << (13 * 2));
    GPIOC_MODER |=  (1 << (13 * 2));

    while (1)
    {
        GPIOC_ODR ^= (1 << 13);
        Delay(500000);
    }
}
