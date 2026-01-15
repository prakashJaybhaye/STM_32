# STM32F411 – GPIO, EXTI Interrupt & SysTick Delay (Bare Metal)

## Overview
This project demonstrates **bare-metal programming** on STM32F411 using:
- GPIO for LED control
- EXTI (External Interrupt) for button input
- SysTick timer for millisecond delay
- Direct register-level access (no HAL, no CMSIS)

The application:
- Toggles **LED on PA3** every 1 second using SysTick delay.
- Toggles **LED on PA1** when **Button on PA2** is pressed using EXTI interrupt.

---

## Hardware Connections

| Signal | STM32 Pin | Description |
|--------|-----------|-------------|
| LED1   | PA1       | Toggled by EXTI interrupt |
| LED2   | PA3       | Toggled using SysTick delay |
| Button | PA2       | External interrupt input |

---

## Clock Configuration

- **HSI (Internal High-Speed Oscillator)** = 16 MHz
- SysTick configured to generate **1ms tick**

```
LOAD_VAL = (16000000 / 1000) - 1 = 15999
```

---

## Register Blocks Used

### RCC (Clock Control)
- `RCC_AHB1ENR` → Enable GPIOA clock
- `RCC_APB2ENR` → Enable SYSCFG clock

### GPIOA
- `GPIOA_MODER` → Configure PA1, PA3 as output, PA2 as input
- `GPIOA_ODR` → Read LED state
- `GPIOA_BSRR` → Atomic set/reset LED pins

### SYSCFG
- `SYSCFG_EXTICR1` → Map PA2 to EXTI line 2

### EXTI
- `EXTI_IMR` → Unmask interrupt line 2
- `EXTI_RTSR` → Enable rising edge trigger
- `EXTI_PR` → Clear pending interrupt

### NVIC
- `NVIC_ISER0` → Enable EXTI2 interrupt in NVIC

### SysTick
- `SYST_CSR` → Control and status register
- `SYST_RVR` → Reload value register
- `SYST_CVR` → Current value register

---

## Execution Flow (Step-by-Step)

### 1. Enable Clocks
- Enable GPIOA via `RCC_AHB1ENR`
- Enable SYSCFG via `RCC_APB2ENR`

### 2. Configure GPIO
- PA1 → Output (LED – interrupt controlled)
- PA3 → Output (LED – delay controlled)
- PA2 → Input (Button)

### 3. Configure EXTI for Button (PA2)
- Map PA2 to EXTI2 using `SYSCFG_EXTICR1`
- Unmask EXTI2 line using `EXTI_IMR`
- Enable rising edge trigger using `EXTI_RTSR`
- Enable EXTI2 in NVIC

### 4. Configure SysTick Timer
- Set reload value for 1ms
- Clear current value
- Enable SysTick with processor clock

### 5. Main Loop
- Toggle LED on PA3 every 1000 ms using SysTick delay

### 6. Interrupt Service Routine (EXTI2_IRQHandler)
- Triggered when button on PA2 is pressed
- Toggles LED on PA1
- Clears pending interrupt flag

---

## Key Functions

### SysTimer_Init()
Initializes SysTick for 1ms tick based on 16 MHz clock.

### delay_ms(uint32_t ms)
Blocking delay using SysTick COUNTFLAG.

### EXTI2_IRQHandler()
Interrupt handler for button press. Toggles PA1 LED.

---

## Why This Is Important

This code teaches:
- **Direct register manipulation**
- **Interrupt handling without HAL**
- **SysTick timer usage**
- **Real hardware control flow**

This is the **foundation of professional embedded firmware development**.

---

## Next Learning Steps

1. EXTI falling edge trigger
2. Debouncing using timer
3. Interrupt + timer combination
4. NVIC priority configuration
5. Multiple EXTI lines handling

---

## Author
Bare-metal STM32 learner 🚀  
Practicing register-level firmware development

---

## Note
Tested logic for **STM32F411 Black Pill** board. Adjust pins if using another variant.
