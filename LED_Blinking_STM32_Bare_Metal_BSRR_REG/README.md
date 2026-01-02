# Bare-Metal STM32 LED Toggle (PB15 / GPIOB)

## 📌 Overview

This project demonstrates **bare-metal programming on an STM32 microcontroller** (no HAL, no CMSIS helpers) to **toggle an LED connected to pin PB15** using **direct register access** and **safe atomic operations with ODR + BSRR**.

The goal is to understand how to:

* Read the **STM32 Reference Manual**
* Use the **memory map**
* Enable peripheral clocks
* Configure GPIO registers manually
* Toggle a pin safely using **ODR for reading** and **BSRR for writing**

This approach builds strong fundamentals required for **driver development, RTOS work, and low-level embedded systems**.

---

## 🧠 Target Hardware

* **MCU Family:** STM32F4 series (example: STM32F401 / F411 / F446)  
* **GPIO Port:** GPIOB  
* **Pin Used:** PB15  
* **LED:** External LED (active-high or active-low with current-limiting resistor)  

> ⚠️ Always verify your **MCU datasheet and board schematic** to confirm pin mapping and LED connections.

---

## 📚 Reference Documents Used

1. **STM32 Reference Manual (RMxxxx)**

   * Memory Map  
   * RCC registers  
   * GPIO registers  

2. **Datasheet**

   * Pin multiplexing  
   * Electrical characteristics  

---

## 🔧 Step-by-Step Explanation (Mapped to Code)

### Step 1: Locate RCC Base Address

From the *Reference Manual → Memory Map*:

RCC Base Address = 0x40023800

cpp
Copy code

```c
#define RCC_BASE 0x40023800UL
Step 2: Locate RCC_AHB1ENR Register
From RCC Register Map:

AHB1ENR Offset = 0x30

c
Copy code
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30))
This register controls clocks for GPIO ports (A, B, C, etc.)

Step 3: Enable GPIOB Clock
GPIOB is on AHB1 bus, bit 1 enables GPIOB.

c
Copy code
RCC_AHB1ENR |= (1 << 1);
⚠️ Without enabling the clock, GPIO registers will not work.

Step 4: Locate GPIOB Base Address
From Memory Map:

java
Copy code
GPIOB Base Address = 0x40020400
c
Copy code
#define GPIOB_BASE 0x40020400UL
Step 5: Locate GPIOB_MODER Register
Offset: 0x00

Each pin uses 2 bits

MODER Bits	Mode
00	Input
01	Output
10	Alternate Function
11	Analog

c
Copy code
#define GPIOB_MODER (*(volatile uint32_t *)(GPIOB_BASE + 0x00))
Step 6: Clear MODER Bits for PB15
Each pin uses (pin × 2) bits. For PB15:

c
Copy code
GPIOB_MODER &= ~(3 << (15 * 2));
This clears the mode bits.

Step 7: Set PB15 as Output Mode
Set MODER to 01 (general-purpose output):

c
Copy code
GPIOB_MODER |= (1 << (15 * 2));
Step 8: Locate GPIOB_ODR and GPIOB_BSRR Registers
c
Copy code
#define GPIOB_ODR  (*(volatile uint32_t *)(GPIOB_BASE + 0x14))
#define GPIOB_BSRR (*(volatile uint32_t *)(GPIOB_BASE + 0x18))
ODR → allows reading the output state

BSRR → allows atomic set/reset (safe for interrupts / RTOS)

Step 9: Toggle PB15 Safely
c
Copy code
if (GPIOB_ODR & (1 << 15))
{
    // PB15 HIGH → LED ON → turn OFF
    GPIOB_BSRR = 1 << (15 + 16); // Reset → LOW
}
else
{
    // PB15 LOW → LED OFF → turn ON
    GPIOB_BSRR = 1 << 15;        // Set → HIGH
}
Reads current state from ODR

Writes the opposite using BSRR

Works for active-HIGH or active-LOW LEDs

Step 10: Software Delay
Simple blocking delay using empty loop:

c
Copy code
void delay(uint32_t n)
{
    for (volatile uint32_t i = 0; i < n; i++);
}
⚠️ Delay depends on CPU frequency and is not precise.

Step 11: Infinite Loop
c
Copy code
while (1)
{
    if (GPIOB_ODR & (1 << 15))
        GPIOB_BSRR = 1 << (15 + 16); // Reset → LOW
    else
        GPIOB_BSRR = 1 << 15;        // Set → HIGH

    delay(1000000);
}
Continuously toggles the LED.

🧩 Complete Source Code
c
Copy code
#include <stdint.h>

#define RCC_BASE      0x40023800UL
#define RCC_AHB1ENR   (*(volatile uint32_t *)(RCC_BASE + 0x30))

#define GPIOB_BASE    0x40020400UL
#define GPIOB_MODER   (*(volatile uint32_t *)(GPIOB_BASE + 0x00))
#define GPIOB_ODR     (*(volatile uint32_t *)(GPIOB_BASE + 0x14))
#define GPIOB_BSRR    (*(volatile uint32_t *)(GPIOB_BASE + 0x18))

void delay(uint32_t n)
{
    for (volatile uint32_t i = 0; i < n; i++);
}

int main(void)
{
    // Enable GPIOB clock
    RCC_AHB1ENR |= (1 << 1);

    // Configure PB15 as output
    GPIOB_MODER &= ~(3 << (15 * 2));
    GPIOB_MODER |=  (1 << (15 * 2));

    while (1)
    {
        if (GPIOB_ODR & (1 << 15))
            GPIOB_BSRR = 1 << (15 + 16); // Reset → LOW
        else
            GPIOB_BSRR = 1 << 15;        // Set → HIGH

        delay(1000000);
    }
}
⚠️ Important Notes
Bare-metal example, no HAL or startup code shown

System clock assumed default (HSI)

Delay is blocking and CPU-dependent

BSRR used for atomic set/reset operations

Can be adapted for any GPIO pin by changing port/pin defines

🚀 Next Improvements
Create custom LED driver with port/pin abstraction

Add support for multiple LEDs

Replace delay with timer-based delay (SysTick)

Make driver board-independent

Prepare for RTOS integration

🎯 Learning Outcomes
After completing this example, you should be able to:

Read STM32 reference manuals

Write register-level GPIO drivers

Understand clock gating and atomic operations

Debug bare-metal embedded code

Transition from HAL to low-level embedded systems development
