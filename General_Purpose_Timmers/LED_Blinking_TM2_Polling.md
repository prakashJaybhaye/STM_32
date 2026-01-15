TIM2 Based LED Toggle Using Bare-Metal (STM32F411 Black Pill)
Project Title

LED Toggle using TIM2 Delay – Bare Metal STM32F411 (HSI Clock, No HAL)

Overview

This project demonstrates how to toggle an LED connected to PA3 on the STM32F411 Black Pill board using TIM2 as a hardware timer to generate a precise delay.
The implementation is done in bare-metal style using direct register access without HAL, CubeMX, or CMSIS drivers.

The timer uses the internal HSI clock (16 MHz) and operates in polling mode.

Hardware Used

Board: STM32F411 Black Pill

LED connected to: PA3

Clock source: HSI (16 MHz internal oscillator)

Software Environment

Language: C

Development style: Bare Metal (Register Level)

No HAL / No CubeMX / No CMSIS

Compiler: Any ARM GCC compatible toolchain

Functional Description

The program performs the following tasks:

Enables the GPIOA peripheral clock.

Configures PA3 as a general-purpose output pin.

Enables the TIM2 peripheral clock.

Configures TIM2 prescaler to generate a 1 ms time base.

Uses TIM2 in up-counting mode to create a blocking delay.

Toggles the LED state every 1000 ms (1 second) using the timer delay.

Clock Configuration

HSI = 16 MHz

Prescaler calculation:

16,000,000 / 16,000 = 1,000 Hz  →  1 ms per tick


So:

TIM2_PSC = (16000000 / 16000) - 1;

Register Usage
RCC Registers

RCC_AHB1ENR → Enable GPIOA clock

RCC_APB1ENR → Enable TIM2 clock

GPIO Registers

GPIOA_MODER → Configure PA3 as output

GPIOA_ODR → Read current LED state

GPIOA_BSRR → Atomic set/reset of PA3

TIM2 Registers

TIM2_PSC → Prescaler register

TIM2_ARR → Auto-reload register (delay value)

TIM2_CNT → Counter register

TIM2_CR1 → Control register (start/stop timer)

TIM2_SR → Status register (UIF flag)

TIM2_EGR → Event generation register (force update)

Program Flow

Enable GPIOA clock.

Configure PA3 as output.

Enable TIM2 clock.

Configure TIM2 prescaler for 1 ms tick.

Force update event to load prescaler.

Enter infinite loop:

Read LED state.

Toggle LED using BSRR.

Call delay function with 1000 ms.

Repeat.

Delay Function Logic

The delay function follows this hardware sequence:

Stop TIM2.

Load delay value into ARR.

Reset counter.

Force update event (EGR).

Start TIM2.

Poll UIF flag in SR.

When UIF is set, clear it.

Stop TIM2.

This ensures accurate and deterministic delay timing.

Why EGR (Force Update) Is Used

STM32 timers use shadow registers for PSC and ARR.
Setting the UG bit in TIM2_EGR forces the new values to be loaded immediately into the active timer logic.

Without this, the new values may not take effect until the next update event.

How to Build and Run

Connect STM32F411 Black Pill via ST-Link.

Compile the code using ARM GCC toolchain.

Flash the binary to the board.

Observe LED on PA3 toggling every 1 second.

Expected Output

LED on PA3 turns ON for 1 second

Then OFF for 1 second

Repeats continuously

Learning Outcomes

After this project, you will understand:

RCC clock enabling

GPIO configuration at register level

Timer prescaler and auto-reload logic

Force update (EGR) mechanism

Polling-based delay implementation

Hardware-level LED control using BSRR

Important Notes

This implementation uses polling, which blocks the CPU.

For real-time applications, interrupt-based timers should be used.

This project is intended for learning and understanding hardware behavior.

Author

Developed by: Prakash Jaybhaye
Learning Path: Bare-Metal Embedded Systems – STM32

Next Steps

Implement TIM2 interrupt-based LED toggle

Implement PWM using TIM2

Explore input capture and output compare modes
