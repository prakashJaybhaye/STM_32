# LED Toggle using TIM2 Delay – Bare Metal STM32F411

## Overview

This project demonstrates how to toggle an LED connected to **PA3** on the **STM32F411 Black Pill** board using **TIM2 as a hardware timer** to generate a precise delay.  
The implementation is done in **bare-metal style** using **direct register access** without HAL, CubeMX, or CMSIS drivers.

The timer uses the **internal HSI clock (16 MHz)** and operates in **polling mode**.

---

## Hardware Used

- Board: STM32F411 Black Pill  
- LED connected to: PA3  
- Clock source: HSI (16 MHz internal oscillator)

---

## Software Environment

- Language: C  
- Development style: Bare Metal (Register Level)  
- No HAL / No CubeMX / No CMSIS  
- Compiler: ARM GCC toolchain

---

## Functional Description

The program performs the following tasks:

1. Enables the GPIOA peripheral clock.  
2. Configures PA3 as a general-purpose output pin.  
3. Enables the TIM2 peripheral clock.  
4. Configures TIM2 prescaler to generate a **1 ms time base**.  
5. Uses TIM2 in up-counting mode to create a blocking delay.  
6. Toggles the LED state every **1000 ms (1 second)** using the timer delay.

---

## Clock Configuration

HSI = 16 MHz

Prescaler calculation:

16,000,000 / 16,000 = 1,000 Hz  →  1 ms per tick

So:

TIM2_PSC = (16000000 / 16000) - 1;

---

## Register Usage

### RCC Registers
- RCC_AHB1ENR → Enable GPIOA clock  
- RCC_APB1ENR → Enable TIM2 clock  

### GPIO Registers
- GPIOA_MODER → Configure PA3 as output  
- GPIOA_ODR → Read current LED state  
- GPIOA_BSRR → Atomic set/reset of PA3  

### TIM2 Registers
- TIM2_PSC → Prescaler register  
- TIM2_ARR → Auto-reload register (delay value)  
- TIM2_CNT → Counter register  
- TIM2_CR1 → Control register (start/stop timer)  
- TIM2_SR → Status register (UIF flag)  
- TIM2_EGR → Event generation register (force update)  

---

## Program Flow

1. Enable GPIOA clock.  
2. Configure PA3 as output.  
3. Enable TIM2 clock.  
4. Configure TIM2 prescaler for 1 ms tick.  
5. Force update event to load prescaler.  
6. Enter infinite loop:  
   - Read LED state.  
   - Toggle LED using BSRR.  
   - Call delay function with 1000 ms.  
   - Repeat.

---

## Delay Function Logic

The delay function follows this hardware sequence:

1. Stop TIM2.  
2. Load delay value into ARR.  
3. Reset counter.  
4. Force update event (EGR).  
5. Start TIM2.  
6. Poll UIF flag in SR.  
7. When UIF is set, clear it.  
8. Stop TIM2.

---

## Why EGR (Force Update) Is Used

STM32 timers use shadow registers for PSC and ARR.  
Setting the UG bit in TIM2_EGR forces the new values to be loaded immediately into the active timer logic.

Without this, the new values may not take effect until the next update event.

---

## Expected Output

- LED on PA3 turns ON for 1 second  
- Then OFF for 1 second  
- Repeats continuously  

---

## Learning Outcomes

- RCC clock enabling  
- GPIO configuration at register level  
- Timer prescaler and auto-reload logic  
- Force update (EGR) mechanism  
- Polling-based delay implementation  
- Hardware-level LED control using BSRR  

---

## Author

Prakash Jaybhaye  
Bare-Metal Embedded Systems – STM32

---

## Next Steps

- Implement TIM2 interrupt-based LED toggle  
- Implement PWM using TIM2  
- Explore input capture and output compare modes
