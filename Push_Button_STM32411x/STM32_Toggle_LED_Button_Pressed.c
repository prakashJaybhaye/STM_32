/*
------------------------------------------------------------------------------------------------------------
1. Locate the RCC base address from the STM32 reference manual memory map.
2. Locate the RCC_AHB1ENR register offset and compute its address as: RCC_BASE + 0x30.
3. Enable the GPIOC peripheral clock by setting bit 2 in RCC_AHB1ENR (PC port is bit 2).
4. Locate the GPIOC base address from the memory map.
5. Locate the GPIOC_MODER register offset and compute its address as: GPIOC_BASE + 0x00.
6. Configure the LED pin (PC13) as a general-purpose output:
    - Clear the two MODER bits for PC13 (bits 26 and 27)
    - Set the MODER bits to 01 to select output mode
7. Configure the Button pin (PC14) as a general-purpose input:
    - Clear the two MODER bits for PC14 (bits 28 and 29)
    - Leave them as 00 for input mode
8. Inside an infinite loop:
    a. Read the current state of the button (PC14) from GPIOC_IDR
    b. Compare with previous state to detect a rising edge (button press)
    c. If a rising edge is detected:
        i. Check the current LED state from GPIOC_ODR
        ii. If LED is ON, reset it using GPIOC_BSRR upper half (bit 29)
        iii. If LED is OFF, set it using GPIOC_BSRR lower half (bit 13)
    d. Update previous button state to current state
    e. Delay briefly to provide simple debounce
9. Repeat the loop to continuously read the button and toggle the LED on each press
------------------------------------------------------------------------------------------------------------
*/

#include <stdint.h>

#define RCC_BASE 0x40023800UL
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30))

#define GPIOC_BASE 0x40020800UL
#define GPIOC_MODER (*(volatile uint32_t *)(GPIOC_BASE + 0x00))
#define GPIOC_BSRR (*(volatile uint32_t *)(GPIOC_BASE + 0x18))
#define GPIOC_IDR (*(volatile uint32_t *)(GPIOC_BASE + 0x10))
#define GPIOC_ODR (*(volatile uint32_t *)(GPIOC_BASE + 0x14))

#define LED_PIN 13
#define PUSH_BUTTON 14
#define BSRR_RESET 16

void delay(uint32_t n)
{
    for (uint32_t i = 0; i < n; i++)
        ;
}

int main(void)
{
    RCC_AHB1ENR |= 1 << 2;

    GPIOC_MODER &= ~(3 << (2 * LED_PIN));
    GPIOC_MODER |= (1 << (2 * LED_PIN));

    GPIOC_MODER &= ~(3 << (2 * PUSH_BUTTON));

    static uint8_t Prev_State = 0;

    while (1)
    {
        uint8_t Curr_State = (GPIOC_IDR >> PUSH_BUTTON) & 1;

        if (Prev_State == 0 && Curr_State == 1)
        {
            if (GPIOC_ODR & (1<<LED_PIN))
            {
                GPIOC_BSRR = 1<<(LED_PIN + BSRR_RESET);
            }
             
            else
            {
                GPIOC_BSRR = 1 << LED_PIN;
            }
        }
        Prev_State = Curr_State;

        delay (100000);
    }
    
}
