# STM32F411 TIM2 Interrupt-Based LED Blink (Bare-Metal)

This project demonstrates **bare-metal register-level programming** on
the STM32F411 (Black Pill) using **TIM2 interrupt** to toggle an LED on
**PA3** every 1000 ms, without HAL or CMSIS.

------------------------------------------------------------------------

## 🔧 Features

-   Pure **register-level C** (no HAL, no CMSIS)
-   Uses **HSI 16 MHz** default clock
-   **TIM2** configured as a millisecond timer
-   **Interrupt-driven delay**
-   LED toggling using **GPIOA BSRR** (atomic set/reset)

------------------------------------------------------------------------

## 🧠 How It Works

1.  **Clock Enable**
    -   RCC AHB1ENR → GPIOA clock
    -   RCC APB1ENR → TIM2 clock
2.  **GPIO Configuration**
    -   PA3 set as output using `GPIOA_MODER`
3.  **TIM2 Configuration**
    -   Prescaler set to get 1 kHz timer clock
    -   Auto-reload register (ARR) loaded with delay value
    -   Update interrupt enabled (DIER)
    -   Update event generated (EGR)
    -   Counter started (CR1)
4.  **Interrupt Handling**
    -   On update event, `TIM2_IRQHandler` clears flag and sets
        `TM2_flag`
    -   Main loop toggles LED when flag is set

------------------------------------------------------------------------

## ⏱ Timer Calculation

HSI = 16 MHz\
Prescaler = (16000000 / 16000) - 1 = 999

Timer clock = 1 kHz → 1 count = 1 ms

So,

``` c
delay(1000); // = 1000 ms = 1 second
```

------------------------------------------------------------------------

## 📁 File Structure

``` text
.
├── main.c
└── README.md
```

------------------------------------------------------------------------

## 🧪 Tested On

-   **STM32F411CEU6 Black Pill**
-   External LED on **PA3**
-   HSI clock (default, no PLL)

------------------------------------------------------------------------

## 🚀 Build & Flash

Use any of the following: - STM32CubeIDE (empty project, no HAL) -
arm-none-eabi-gcc + Makefile - OpenOCD / ST-Link

------------------------------------------------------------------------

## ⚠ Important Notes

-   Vector table must correctly map `TIM2_IRQHandler`
-   NVIC bit 28 corresponds to TIM2 IRQ
-   No HAL, no CMSIS -- you are fully responsible for register
    definitions

------------------------------------------------------------------------

## 📜 License

MIT License -- free to use, modify, and distribute.

------------------------------------------------------------------------

## ✍ Author

Prakash Jaybhaye\
Bare-metal STM32 learner \| Automotive Embedded Engineer

------------------------------------------------------------------------

## ⭐ Tip

This project is an **excellent foundation** for learning: - PWM - Input
capture - RTOS tick implementation - Low-level driver development

------------------------------------------------------------------------

Happy Register Hacking 😎
