
# STM32 Bare‑Metal LED Control with Button and PWM

## Overview
This project demonstrates **bare‑metal firmware design** on an STM32 (STM32F411‑class MCU) using **direct register access**, without HAL or CMSIS abstractions.

The application controls an LED connected to **PA0** using a push button on **PA1**.  
Each button press cycles through four states:

1. LED OFF  
2. LED ON  
3. LED TOGGLE (1 second interval)  
4. LED PWM fade (0–100% duty cycle)

The goal of this README is to explain **every step**, so the same design can be **re‑implemented on another microcontroller**.

---

## Key Concepts Used
- Memory‑mapped I/O
- Peripheral clock enabling
- GPIO mode configuration
- External interrupts (EXTI)
- SysTick timer for delays
- Timer‑based PWM (TIM2)
- State machine design
- One‑time peripheral initialization using flags

---

## Memory‑Mapped Registers

### Why registers?
Microcontrollers expose peripherals through fixed memory addresses.  
Writing to these addresses configures hardware directly.

Example:
```c
#define GPIOA_MODER (*(volatile uint32_t *)(0x40020000 + 0x00))
```

- `volatile` → prevents compiler optimizations
- Pointer cast → maps C variable to hardware register

---

## Clock Configuration

### RCC (Reset and Clock Control)

Before using any peripheral, its clock must be enabled.

```c
RCC_AHB1ENR |= (1 << 0);   // Enable GPIOA
RCC_APB1ENR |= (1 << 0);  // Enable TIM2
RCC_APB2ENR |= (1 << 14); // Enable SYSCFG (for EXTI)
```

**Porting note:**  
On another MCU, identify:
- GPIO clock register
- Timer clock register
- System configuration block

---

## GPIO Configuration

### LED (PA0) as Output
```c
GPIOA_MODER &= ~(3 << (2 * 0));
GPIOA_MODER |=  (1 << (2 * 0));
```

Each GPIO pin uses **2 bits** in `MODER`:
- `00` → Input
- `01` → Output
- `10` → Alternate function
- `11` → Analog

---

### Button (PA1) as Input
```c
GPIOA_MODER &= ~(3 << (2 * 1));
```

Input mode allows EXTI to detect edges.

---

## Alternate Function (PWM Output)

When using PWM, PA0 must connect to TIM2_CH1.

```c
GPIOA_MODER |= (2 << (2 * 0));      // Alternate function mode
GPIOA_AFRL  |= (1 << (4 * 0));      // AF1 → TIM2_CH1
```

**Porting note:**  
Check:
- Which timer channel maps to which pin
- Which alternate function number is required

---

## External Interrupt (Button)

### EXTI Configuration
```c
SYSCFG_EXTICR1 &= ~(0xF << 8); // PA1 mapping
EXTI_IMR  |= (1 << 1);         // Unmask line 1
EXTI_RTSR |= (1 << 1);         // Rising edge trigger
NVIC_ISER0 |= (1 << 7);        // Enable EXTI1 IRQ
```

### ISR Behavior
```c
void EXTI1_IRQHandler(void)
{
    button_sate++;
    EXTI_PR = (1 << 1); // Clear pending bit

    if (button_sate > 3)
        button_sate = 0;
}
```

ISR only:
- Updates state
- Clears interrupt flag

**Design rule:**  
Never put delays or heavy logic inside ISRs.

---

## State Machine Design

```c
typedef enum {
    LED_OFF,
    LED_ON,
    LED_TOGGLE,
    LED_PWM
} led_state_en;
```

The `switch(button_sate)` inside `while(1)` implements the state machine.

This structure scales well to larger applications.

---

## SysTick Timer (Delay)

SysTick runs at CPU clock frequency.

```c
SYST_RVR = (16000000 / 1000) - 1;
SYST_CSR = (1 << 0) | (1 << 2);
```

Used for:
- LED toggle timing
- PWM fade timing

**Porting note:**  
Any periodic timer can replace SysTick.

---

## PWM Generation (TIM2)

### Timer Setup
```c
TIM2_PSC = 15;    // Prescaler → 1 MHz
TIM2_ARR = 1000;  // 1 kHz PWM
```

PWM duty cycle:
```c
TIM2_CCR1 = (TIM2_ARR * duty) / 100;
```

### Why preload?
```c
TIM2_CCMR1 |= (1 << 3); // Preload enable
```

Ensures smooth duty cycle updates without glitches.

---

## One‑Time Initialization (Key Design Idea)

### Problem
Reinitializing GPIO and timers repeatedly wastes CPU time and can break PWM.

### Solution: Flag‑based control
```c
uint8_t pwm_flag = 0;
```

- Initialize PWM only once when entering PWM state
- Deinitialize when leaving PWM state

```c
if (!pwm_flag) {
    GPIOA_Init_AF();
    TIM2_PWM_Init();
    pwm_flag = 1;
}
```

Exit cleanup:
```c
if (pwm_flag && button_sate != LED_PWM) {
    TIM2_CR1 &= ~(1 << 0);
    GPIOA_Init();
    pwm_flag = 0;
}
```

This pattern is **portable and scalable**.

---

## How to Port This Design to Another MCU

1. Replace register base addresses
2. Update GPIO mode bits
3. Map timer channel to correct pin
4. Adjust clock frequencies
5. Update EXTI / interrupt controller logic
6. Keep the **state machine and flag logic unchanged**

---

## Summary

This project demonstrates:
- Clean bare‑metal firmware structure
- Proper peripheral ownership
- Interrupt‑driven state changes
- Safe and efficient PWM handling

By understanding this README, the same logic can be rebuilt on:
- Any STM32 family
- Any ARM Cortex‑M MCU
- Even non‑ARM microcontrollers with timers and GPIO

---

## Author
Designed as a **learning‑focused bare‑metal example** for embedded firmware engineers.
