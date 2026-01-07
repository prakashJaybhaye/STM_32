Bare-Metal STM32 LED Toggle (PA8)
📌 Overview

This project demonstrates bare-metal programming on an STM32 microcontroller (no HAL, no CMSIS helpers) to toggle an LED connected to pin PA8 using direct register access.

The goal is to understand how to:

Read the STM32 Reference Manual

Use the memory map

Enable peripheral clocks

Configure GPIO registers manually

Control a pin using ODR

This approach builds strong fundamentals required for driver development, RTOS work, and low-level embedded systems.

🧠 Target Hardware

MCU Family: STM32F4 series (e.g., STM32F401 / STM32F411 / STM32F446)

GPIO Port: GPIOA

Pin Used: PA8

LED: External LED or onboard LED (if wired to PA8)

⚠️ Always verify your exact MCU reference manual and board schematic to confirm pin connections.

📚 Reference Documents Used

You must refer to the following STM32 documents:

Reference Manual (RMxxxx)

Memory Map

RCC registers

GPIO registers

Datasheet

Pin multiplexing

Electrical characteristics

🔧 Step-by-Step Explanation (Mapped to Code)
Step 1: Locate RCC Base Address

From Reference Manual → Memory Map:

RCC Base Address = 0x40023800

#define RCC_BASE 0x40023800UL

Step 2: Locate RCC_AHB1ENR Register

From RCC Register Map:

AHB1ENR Offset = 0x30

#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30))


This register controls clocks for:

GPIOA

GPIOB

GPIOC

etc.

Step 3: Enable GPIOA Clock

GPIOA is connected to the AHB1 bus.
Bit 0 enables GPIOA.

RCC_AHB1ENR |= (1 << 0);


⚠️ Without enabling the clock, GPIO registers will not function.

Step 4: Locate GPIOA Base Address

From Memory Map:

GPIOA Base Address = 0x40020000

#define GPIOA_BASE 0x40020000UL

Step 5: Locate GPIOA_MODER Register

Offset: 0x00

Each GPIO pin uses 2 bits

MODER Bits	Mode
00	Input
01	Output
10	Alternate Function
11	Analog
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + 0x00))

Step 6: Clear MODER Bits for PA8

Each pin uses (pin × 2) bits.

For PA8 → bits [17:16]

GPIOA_MODER &= ~(3 << (8 * 2));


This clears the existing mode configuration.

Step 7: Set PA8 as Output Mode

Set MODER bits to 01:

GPIOA_MODER |= (1 << (8 * 2));


PA8 is now configured as a general-purpose output pin.

Step 8: Locate GPIOA_ODR Register

Offset: 0x14

Controls output state

#define GPIOA_ODR (*(volatile uint32_t *)(GPIOA_BASE + 0x14))

Step 9: Toggle PA8
GPIOA_ODR ^= (1 << 8);


1 → LED ON

0 → LED OFF

XOR toggles the pin state.

Step 10: Software Delay

Simple blocking delay using an empty loop:

void delay(uint32_t n)
{
    for (volatile uint32_t i = 0; i < n; i++);
}


⚠️ Delay depends on CPU clock frequency and is not precise.

Step 11: Infinite Loop
while (1)
{
    GPIOA_ODR ^= (1 << 8);
    delay(100000);
}


This continuously toggles the LED.

🧩 Complete Source Code
#include <stdint.h>

#define RCC_BASE        0x40023800UL
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30))

#define GPIOA_BASE      0x40020000UL
#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR       (*(volatile uint32_t *)(GPIOA_BASE + 0x14))

void delay(uint32_t n)
{
    for (volatile uint32_t i = 0; i < n; i++);
}

int main(void)
{
    /* Enable GPIOA clock */
    RCC_AHB1ENR |= (1 << 0);

    /* Configure PA8 as output */
    GPIOA_MODER &= ~(3 << (8 * 2));
    GPIOA_MODER |=  (1 << (8 * 2));

    while (1)
    {
        GPIOA_ODR ^= (1 << 8);
        delay(100000);
    }
}

⚠️ Important Notes

Bare-metal example (startup code not shown)

Default system clock assumed (HSI)

Blocking delay is not accurate

ODR is not atomic — BSRR is preferred in production

🚀 Next Improvements

Use GPIOx_BSRR for atomic pin control

Implement timer-based delay (SysTick)

Create a reusable GPIO driver

Support multiple pins and ports

Add CMSIS-only startup code

🎯 Learning Outcome

After completing this project, you should be able to:

Read STM32 reference manuals

Write register-level GPIO code

Understand clock gating

Build a foundation for driver and RTOS development
