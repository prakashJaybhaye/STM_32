# Bare-Metal STM32 LED Toggle (PA8)

## 📌 Overview

This project demonstrates **bare‑metal programming on an STM32 microcontroller** (no HAL, no CMSIS helpers) to **toggle an LED connected to pin PA8** using **direct register access**.

The goal is to understand how to:

* Read the **STM32 Reference Manual**
* Use the **memory map**
* Enable peripheral clocks
* Configure GPIO registers manually
* Control a pin using ODR

This approach builds strong fundamentals required for **driver development, RTOS work, and low‑level embedded systems**.

---

## 🧠 Target Hardware

* **MCU Family:** STM32F4 series (example: STM32F401 / F411 / F446)
* **GPIO Port:** GPIOA
* **Pin Used:** PA8
* **LED:** External LED or onboard LED (if wired to PA8)

> ⚠️ Always verify your **exact MCU reference manual** and **schematic** to confirm pin connections.

---

## 📚 Reference Documents Used

You must refer to the following documents:

1. **Reference Manual (RMxxxx)**

   * Memory Map
   * RCC registers
   * GPIO registers

2. **Datasheet**

   * Pin multiplexing
   * Electrical characteristics

---

## 🔧 Step‑by‑Step Explanation (Mapped to Code)

### Step 1: Locate RCC Base Address

From the *Reference Manual → Memory Map*:

```
RCC Base Address = 0x40023800
```

```c
#define RCC_BASE 0x40023800UL
```

---

### Step 2: Locate RCC_AHB1ENR Register

From *RCC Register Map*:

* **AHB1ENR Offset = 0x30**

```c
#define AHB1ENTR (*(volatile uint32_t *)(RCC_BASE + 0x30))
```

This register controls clocks for:

* GPIOA, GPIOB, GPIOC, etc.

---

### Step 3: Enable GPIOA Clock

GPIOA is connected to **AHB1 bus**, bit **0** enables GPIOA.

```c
AHB1ENTR |= (1 << 0);
```

> ⚠️ Without enabling the clock, GPIO registers will **not work**.

---

### Step 4: Locate GPIOA Base Address

From *Memory Map*:

```
GPIOA Base Address = 0x40020000
```

```c
#define GPIOA_BASE 0x40020000UL
```

---

### Step 5: Locate GPIOA_MODER Register

* Offset: `0x00`
* Each pin uses **2 bits**

| MODER Bits | Mode               |
| ---------- | ------------------ |
| 00         | Input              |
| 01         | Output             |
| 10         | Alternate Function |
| 11         | Analog             |

```c
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
```

---

### Step 6: Clear MODER Bits for PA8

Each pin uses `(pin × 2)` bits.

For **PA8** → bits **[17:16]**

```c
GPIOA_MODER &= ~(3 << (8 * 2));
```

This clears the mode bits.

---

### Step 7: Set PA8 as Output Mode

Set MODER to `01`:

```c
GPIOA_MODER |= (1 << (8 * 2));
```

PA8 is now configured as **General Purpose Output**.

---

### Step 8: Locate GPIOA_ODR Register

* Offset: `0x14`
* Controls output state

```c
#define GPIOA_ODR (*(volatile uint32_t *)(GPIOA_BASE + 0x14))
```

---

### Step 9: Toggle PA8

```c
GPIOA_ODR ^= (1 << 8);
```

* `1` → LED ON
* `0` → LED OFF

> XOR toggles the bit state.

---

### Step 10: Software Delay

Simple blocking delay using empty loop:

```c
void delay(uint32_t n)
{
    for (volatile uint32_t i = 0; i < n; i++);
}
```

> ⚠️ Delay depends on **CPU clock frequency** and is not precise.

---

### Step 11: Infinite Loop

```c
while (1)
{
    GPIOA_ODR ^= (1 << 8);
    delay(100000);
}
```

This continuously toggles the LED.

---

## 🧩 Complete Source Code

```c
#include <stdint.h>

#define RCC_BASE        0x40023800UL
#define AHB1ENTR        (*(volatile uint32_t *)(RCC_BASE + 0x30))

#define GPIOA_BASE      0x40020000UL
#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR       (*(volatile uint32_t *)(GPIOA_BASE + 0x14))

void delay(uint32_t n)
{
    for (volatile uint32_t i = 0; i < n; i++);
}

int main(void)
{
    // Enable GPIOA clock
    AHB1ENTR |= (1 << 0);

    // Configure PA8 as output
    GPIOA_MODER &= ~(3 << (8 * 2));
    GPIOA_MODER |=  (1 << (8 * 2));

    while (1)
    {
        GPIOA_ODR ^= (1 << 8);
        delay(100000);
    }
}
```

---

## ⚠️ Important Notes

* This is **bare‑metal**, no startup code shown
* System clock assumed default (HSI)
* No optimization safe delays
* Prefer **BSRR register** for atomic operations in production code

---

## 🚀 Next Improvements

* Use `GPIOx_BSRR` instead of ODR
* Implement timer‑based delay (SysTick)
* Create reusable GPIO driver
* Support multiple pins using macros
* Move toward CMSIS‑only startup

---

## 🎯 Learning Outcome

After completing this example, you should be able to:

* Read STM32 reference manuals
* Write register‑level GPIO drivers
* Understand clock gating
* Debug bare‑metal embedded code

---

📘 *This project is ideal for learners transitioning from HAL to true embedded systems development.*
