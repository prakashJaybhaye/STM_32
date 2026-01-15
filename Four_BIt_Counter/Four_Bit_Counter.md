# STM32 4-Bit Binary Counter – Three Implementations (SysTick Delay, Polling Button, EXTI Button)

This document explains **three different 4-bit binary counter implementations** on STM32F446xx using PA0–PA3 LEDs:
1. **SysTick delay based counter**
2. **Polling button based counter**
3. **EXTI interrupt based button counter**

All implementations are **bare-metal, register-level** and use **GPIOA PA0–PA3** for LEDs.

---

## Common Hardware Mapping

| Signal | STM32 Pin | Description |
|--------|-----------|-------------|
| LED0   | PA0       | LSB of 4-bit counter |
| LED1   | PA1       | Bit1 |
| LED2   | PA2       | Bit2 |
| LED3   | PA3       | MSB |
| Button | PA5 / PA4 | Depending on implementation |

---

# 1) 4-Bit Counter using SysTick Delay (No Button)

## Purpose
Demonstrates:
- SysTick timer usage
- GPIO output control
- Binary counting using LEDs

## Flow

1. Enable **GPIOA clock** using RCC_AHB1ENR.
2. Configure **PA0–PA3 as output mode** in GPIOA_MODER.
3. Configure **SysTick**:
   - Load value = (16 MHz / 1000) – 1
   - Enable counter + processor clock
4. Initialize `counter = 0`.
5. Loop 16 times:
   - Clear PA0–PA3 using **BSRR upper half**
   - Write counter value to PA0–PA3 using **BSRR lower half**
   - Delay 2000 ms
   - Increment counter
6. Repeat forever.

## Key Concept
Binary value of `counter` directly drives LED pattern on PA0–PA3.

---

# 2) 4-Bit Counter using Polling Push Button

## Purpose
Demonstrates:
- GPIO input reading
- Edge detection in software
- Debouncing using SysTick delay

## Flow

1. Enable **GPIOA clock**.
2. Configure:
   - PA0–PA3 as output
   - PA5 as input (button)
3. Initialize SysTick for 1 ms tick.
4. Initialize:
   - `counter = 0`
   - `Button_Prv_State = 0`
5. In infinite loop:
   - Read current button state from GPIOA_IDR.
   - If transition detected **(0 → 1)**:
     - Clear PA0–PA3
     - Output counter
     - Increment counter
     - Reset counter if > 15
   - Store current state as previous state.
   - Delay 50 ms (debounce)

## Key Concept
This is **software edge detection + software debounce**.

---

# 3) 4-Bit Counter using EXTI Interrupt (Best Practice)

## Purpose
Demonstrates:
- External interrupt (EXTI)
- NVIC usage
- Hardware event-driven design

## Flow

1. Enable:
   - GPIOA clock
   - SYSCFG clock
2. Configure:
   - PA0–PA3 as output
   - PA4 as input (button)
3. Map **PA4 → EXTI4** using SYSCFG_EXTICR2.
4. Configure EXTI:
   - Unmask line 4 in EXTI_IMR
   - Enable rising edge trigger in EXTI_RTSR
5. Enable **EXTI4 IRQ in NVIC** (bit 10).
6. Initialize `counter = 0`.
7. On button press → **EXTI4_IRQHandler executes**:
   - Clear PA0–PA3
   - Output counter
   - Increment counter
   - Reset if > 15
   - Clear pending flag in EXTI_PR

## Key Concept
This is **hardware interrupt driven**, no polling, no delay, real embedded style.

---

# Register Blocks Used

## RCC
- RCC_AHB1ENR → GPIO clock enable
- RCC_APB2ENR → SYSCFG clock enable

## GPIOA
- MODER → pin mode selection
- IDR → input read
- BSRR → atomic set/reset

## SysTick
- CSR → control and status
- RVR → reload value
- CVR → current value

## EXTI
- IMR → interrupt mask
- RTSR → rising edge select
- PR → pending clear

## SYSCFG
- EXTICR2 → port selection for EXTI line

## NVIC
- ISER0 → interrupt enable register

---

# Learning Outcomes

After these three programs, you have learned:

- How **binary counters work in hardware**
- How to use **SysTick for timing**
- Difference between **polling vs interrupt**
- How **EXTI + NVIC** works internally
- How to design **event-driven firmware**

---

# Very Important Note

You are not just blinking LEDs.
You are learning **real embedded firmware architecture**.

This progression:
GPIO → SysTick → Polling → EXTI → NVIC  
is exactly how **professional firmware engineers** grow.

---

# Next Recommended Steps

1. 4-bit counter using **TIM2 interrupt**
2. 8-bit counter using **two ports**
3. Counter with **up/down mode**
4. Counter with **auto-reset using timer**
5. PWM based LED brightness control

---

## Author Note

All code is **bare-metal, register-level** without HAL.
Target: **STM32F446xx**  
Clock: **16 MHz HSI**

---

Happy hacking. Real firmware engineer path. 💪
