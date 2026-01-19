# STM32F411 Bare-Metal PWM LED Brightness Control

## Overview

This project demonstrates **bare‑metal PWM generation** on the
**STM32F411 Black Pill** using **TIM2 Channel 1** to control LED
brightness on **PA0**. The LED brightness increases every **500 ms** in
steps of **20% duty cycle**, reaches **100%**, then resets to **0%** and
repeats.

No HAL, no CMSIS abstraction --- **direct register‑level programming
only**.

------------------------------------------------------------------------

## Hardware & Clock Configuration

-   MCU: STM32F411
-   Clock source: **HSI = 16 MHz**
-   AHB Prescaler: 1
-   APB1 Prescaler: 1
-   TIM2 clock: 16 MHz

------------------------------------------------------------------------

## GPIO Configuration (PA0 → TIM2_CH1)

### Registers Used

  Register      Purpose
  ------------- ---------------------------
  RCC_AHB1ENR   Enable GPIOA clock
  GPIOA_MODER   Configure pin mode
  GPIOA_AFRL    Select alternate function

### Configuration Steps

1.  Enable GPIOA clock via `RCC_AHB1ENR`.
2.  Set PA0 mode to **Alternate Function** (`MODER = 10`).
3.  Select **AF1** in `GPIOA_AFRL` to connect PA0 to **TIM2_CH1**.

------------------------------------------------------------------------

## Timer Selection

-   **TIM2** is a 32‑bit general‑purpose timer.
-   Channel 1 output is available on PA0 via AF1.

------------------------------------------------------------------------

## PWM Timing Calculations

### Prescaler (PSC)

    TIM2_PSC = 15
    Timer clock = 16 MHz / (15 + 1) = 1 MHz
    Timer tick = 1 µs

### Auto‑Reload Register (ARR)

    TIM2_ARR = 1000
    PWM period = 1000 × 1 µs = 1 ms
    PWM frequency = 1 kHz

### Duty Cycle (CCR1)

    CCR1 = (ARR × duty_cycle) / 100

  Duty %   CCR1   Output
  -------- ------ -------------------
  0%       0      LED OFF
  20%      200    Low brightness
  50%      500    Medium brightness
  100%     1000   Full brightness

------------------------------------------------------------------------

## TIM2 Registers Used

  Register      Function
  ------------- -----------------------
  RCC_APB1ENR   Enable TIM2 clock
  TIM2_PSC      Timer prescaler
  TIM2_ARR      PWM period
  TIM2_CCR1     Duty cycle control
  TIM2_CCMR1    PWM mode selection
  TIM2_CCER     Enable channel output
  TIM2_CR1      Timer control
  TIM2_EGR      Update generation

------------------------------------------------------------------------

## PWM Mode Configuration

### TIM2_CCMR1

-   **OC1M = 110** → PWM Mode 1
-   **OC1PE = 1** → Enable preload for smooth updates

PWM Mode 1 behavior:

    CNT < CCR1  → Output HIGH
    CNT ≥ CCR1  → Output LOW

------------------------------------------------------------------------

## Timer Start Sequence

1.  Enable ARR preload (ARPE).
2.  Generate update event (UG) to load PSC/ARR.
3.  Enable counter (CEN).

PWM output appears on PA0.

------------------------------------------------------------------------

## Delay Implementation (SysTick)

### Registers Used

  Register   Purpose
  ---------- ------------------
  SYST_RVR   Reload value
  SYST_CVR   Current value
  SYST_CSR   Control & status

### Calculation

    Reload = (16,000,000 / 1000) - 1 = 15999

This produces a **1 ms delay tick**.

The `delay_ms()` function polls the COUNTFLAG bit.

------------------------------------------------------------------------

## Main Loop Logic

1.  Convert duty percentage into CCR1 value.
2.  Wait 500 ms using SysTick.
3.  Increase duty cycle by 20%.
4.  Reset to 0% after reaching 100%.

Result:

    0% → 20% → 40% → 60% → 80% → 100% → 0%

------------------------------------------------------------------------

## Key Learning Outcomes

-   Timer‑based PWM generation
-   Prescaler vs ARR role separation
-   Safe bit‑masking practices
-   Independent delay using SysTick
-   Professional bare‑metal register flow

------------------------------------------------------------------------

## Author Notes

This project is intentionally written at **register level** to build
strong fundamentals for embedded firmware development.

Recommended next steps: - PWM using TIM3 or TIM4 - Interrupt‑driven
brightness control - Gamma‑corrected LED dimming

------------------------------------------------------------------------
