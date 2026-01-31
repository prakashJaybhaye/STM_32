
# STM32 base Event-driven Moore finite state machine implemented in a super-loop architecture 

## Overview
This project demonstrates **bare‑metal firmware design** on an STM32 (STM32F411‑class MCU) using **direct register access**, without HAL or CMSIS abstractions.

The application controls an LED connected to **PA0** using a push button on **PA1**.
Each button press cycles through four states:

1. LED OFF  
2. LED ON  
3. LED TOGGLE (1 second interval)  
4. LED PWM fade (0–100% duty cycle)

This README explains **every subsystem in depth**, so the same design can be **rebuilt on another microcontroller**.

---

## Key Concepts Used
- Memory‑mapped I/O
- Peripheral clock enabling
- GPIO configuration
- External interrupts (EXTI)
- NVIC interrupt handling
- SysTick timer for delays
- Timer‑based PWM (TIM2)
- Flag‑based state machine design
- One‑time peripheral initialization

---

## Memory‑Mapped I/O (Core Concept)

Microcontrollers expose hardware through fixed memory addresses.
Writing to these addresses directly controls peripherals.

Example:
```c
#define GPIOA_MODER (*(volatile uint32_t *)(0x40020000 + 0x00))
```

Why `volatile`?
- Prevents compiler optimizations
- Ensures every read/write accesses real hardware

This concept applies to **all microcontrollers**, regardless of vendor.

---

## Clock Configuration (RCC)

No peripheral works until its clock is enabled.

```c
RCC_AHB1ENR |= (1 << 0);   // GPIOA clock
RCC_APB1ENR |= (1 << 0);  // TIM2 clock
RCC_APB2ENR |= (1 << 14); // SYSCFG clock (EXTI)
```

**Porting rule:**  
Always find:
- GPIO clock register
- Timer clock register
- System configuration clock

---

## GPIO Configuration

### LED Pin (PA0 – Output Mode)

Each GPIO pin uses **2 bits** in the MODER register.

| Mode | Value |
|---|---|
| Input | 00 |
| Output | 01 |
| Alternate Function | 10 |
| Analog | 11 |

```c
GPIOA_MODER &= ~(3 << (2 * 0));
GPIOA_MODER |=  (1 << (2 * 0));
```

---

### Button Pin (PA1 – Input Mode)

```c
GPIOA_MODER &= ~(3 << (2 * 1));
```

Input mode allows external interrupts to detect edges.

---

## SysTick Timer (Delay Engine)

### What Is SysTick?
SysTick is a **core ARM Cortex‑M timer**:
- 24‑bit down‑counter
- Clocked from CPU clock
- Vendor‑independent

---

### Configuration

```c
SYST_RVR = (16000000 / 1000) - 1;
SYST_CSR = (1 << 0) | (1 << 2);
```

Explanation:
- CPU clock = 16 MHz
- Desired tick = 1 ms
- Reload value = 16000 − 1
- Processor clock selected
- Interrupt disabled (polling mode)

---

### Delay Mechanism

```c
while (((SYST_CSR >> 16) & 1) == 0);
```

Bit 16 (`COUNTFLAG`) is set when the counter reaches zero.

Each loop = **1 ms delay**.

---

### Why Polling?
- Simple
- Predictable
- No interrupt nesting

Any timer on any MCU can replace SysTick.

---

## External Interrupt (EXTI) and NVIC

### Why Use EXTI?
- Immediate response
- No CPU polling
- Event‑driven design

Signal flow:
```
GPIO → EXTI → NVIC → ISR
```

---

### EXTI Configuration Steps

1. Enable SYSCFG clock
2. Map GPIO pin to EXTI line
3. Unmask interrupt
4. Select trigger edge
5. Enable NVIC interrupt

---

### EXTI Mapping

```c
SYSCFG_EXTICR1 &= ~(0xF << 8); // PA1 → EXTI1
```

---

### Interrupt Mask and Trigger

```c
EXTI_IMR  |= (1 << 1);  // Enable EXTI1
EXTI_RTSR |= (1 << 1); // Rising edge
```

---

### NVIC Enable

```c
NVIC_ISER0 |= (1 << 7); // EXTI1 IRQ
```

---

### ISR Design

```c
void EXTI1_IRQHandler(void)
{
    button_sate++;
    EXTI_PR = (1 << 1);
}
```

ISR responsibilities:
- Update state
- Clear interrupt flag

No delays. No heavy logic.

---

## State Machine Architecture

```c
typedef enum {
    LED_OFF,
    LED_ON,
    LED_TOGGLE,
    LED_PWM
} led_state_en;
```

This is a **simple cooperative finite state machine**:
- States evaluated in main loop
- Transitions triggered by interrupts

Scales well for real applications.

---

## PWM Generation Using TIM2

### Timer Clock Setup

```c
TIM2_PSC = 15;   // 16 MHz / 16 = 1 MHz
TIM2_ARR = 1000; // 1 kHz PWM
```

Timer tick = 1 µs  
PWM period = 1000 µs

---

### PWM Mode

```c
TIM2_CCMR1 |= (6 << 4); // PWM mode 1
```

Behavior:
- Output HIGH while CNT < CCR
- Output LOW otherwise

---

### Duty Cycle Control

```c
TIM2_CCR1 = (TIM2_ARR * duty) / 100;
```

Percentage‑based → portable and scalable.

---

### Preload Enable (Critical)

```c
TIM2_CCMR1 |= (1 << 3);
TIM2_CR1   |= (1 << 7);
```

Preload ensures:
- No glitches
- Updates only at timer overflow

---

## One‑Time Initialization (Flag‑Based Design)

Problem:
- Reinitializing hardware every loop is inefficient

Solution:
```c
if (!pwm_flag)
{
    GPIOA_Init_AF();
    TIM2_PWM_Init();
    pwm_flag = 1;
}
```

Exit cleanup:
```c
if (pwm_flag && button_sate != LED_PWM)
{
    TIM2_CR1 &= ~(1 << 0);
    GPIOA_Init();
    pwm_flag = 0;
}
```

This pattern is **industry‑grade and portable**.

---

## Porting Guide

To port this design:
1. Replace register addresses
2. Update GPIO modes
3. Map timer channel to pin
4. Adjust clock frequency
5. Adapt interrupt controller

The **state machine logic stays unchanged**.

---

## Engineering Takeaway

This project demonstrates:
- Clean bare‑metal architecture
- Event‑driven firmware
- Safe interrupt usage
- Reusable design patterns

Understanding this README allows rebuilding the project on **any MCU with GPIO, timers, and interrupts**.

---

## Author
Learning‑focused bare‑metal example for embedded firmware engineers.
