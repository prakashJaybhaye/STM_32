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
// Toggle LEd at PA 8
#include <stdint.h>

#define RCC_BASE 0x40023800UL
#define AHB1ENTR (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define GPIOA_BASE 0x40020000UL
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR (*(volatile uint32_t *)(GPIOA_BASE + 0x14))

void delay(uint32_t n)
{
    for (volatile uint32_t i = 0; i < n; i++)
        ;
}

int main()
{
    AHB1ENTR |= (1 << 0);

    GPIOA_MODER &= ~(3 << (8 * 2));
    GPIOA_MODER |= (1 << (8 * 2));

    while (1)
    {
        GPIOA_ODR ^= (1 << 8);
        delay(100000);
    }
}