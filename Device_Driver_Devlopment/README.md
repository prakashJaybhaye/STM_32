# STM32 LED Driver (Register-Level GPIO Control)

A minimal, educational STM32 GPIO LED driver implemented using direct register access (no HAL/LL/CMSIS helpers).  
This project demonstrates how to enable GPIO clocks, configure a pin as a general-purpose output, and toggle an LED using atomic register operations (BSRR). The code targets STM32F4-series microcontrollers (e.g., STM32F401/F411/F446) and includes an example that toggles PA8.

---

## Table of contents

- [Overview](#overview)
- [Target hardware](#target-hardware)
- [Reference documents](#reference-documents)
- [Files](#files)
- [Design & API](#design--api)
- [Usage example](#usage-example)
- [Build & flash (general guidance)](#build--flash-general-guidance)
- [Limitations & notes](#limitations--notes)
- [Next improvements](#next-improvements)
- [License](#license)

---

## Overview

This driver is intentionally minimal and educational. It shows how to:

- Read the STM32 reference manual and memory map
- Enable the RCC clock for GPIO ports
- Configure GPIO pin mode manually through MODER
- Toggle a pin atomically using BSRR
- Provide a small, reusable API: `GPIO_Init`, `GPIO_TogglePin`, and `LED_Toggle`

No external libraries (HAL/LL) are required.

---

## Target hardware

- MCU family: STM32F4 series (e.g., STM32F401, STM32F411, STM32F446)
- GPIO port: GPIOA (example uses PA8)
- LED: external LED or board LED wired to PA8

Important: Verify exact device reference manual and board schematic before connecting hardware.

---

## Reference documents

Consult your MCU's reference manual and datasheet for correct memory map and register/bit definitions:

- STM32F4 Reference Manual (RM0008 or RM0368 depending on the specific family)
- STM32F4 datasheet for pin multiplexing and electrical characteristics

---

## Files

- `led_driver_stm32.h` — Register-level GPIO driver header (init + toggle)
- `example_main.c` — Minimal example demonstrating toggling PA8
- `README.md` — This file

---

## Design & API

The driver exposes a tiny API through `led_driver_stm32.h`.

Types:
```c
typedef enum
{
    GPIO_PORT_A = 0,
    GPIO_PORT_B = 1,
    GPIO_PORT_C = 2
} GPIO_PORTS;

typedef struct
{
    GPIO_PORTS Port;
    uint8_t Pin;   // 0..15
} GPIO_t;
```

Predefined pins in the header (examples):
```c
#define GPIOA_PA5   ((GPIO_t){GPIO_PORT_A, 5})
#define GPIOA_PA8   ((GPIO_t){GPIO_PORT_A, 8})
#define GPIOB_PB0   ((GPIO_t){GPIO_PORT_B, 0})
#define GPIOC_PC13  ((GPIO_t){GPIO_PORT_C, 13})
```

API functions (all are `static inline` in the header for simplicity):

- `void GPIO_Init(GPIO_t gpio);`
  - Enables the GPIO port clock (RCC_AHB1ENR) and sets the pin MODER bits to output (`01`).

- `void GPIO_TogglePin(GPIO_t gpio);`
  - Reads the pin state from ODR and uses BSRR to atomically set or reset the pin.

- `void LED_Toggle(GPIO_t gpio);`
  - High-level convenience: calls `GPIO_Init()` then `GPIO_TogglePin()`.

Implementation notes:
- Uses hard-coded base addresses for STM32F4-family (RCC and GPIOA/B/C).
- Uses `RCC_AHB1ENR` to enable clocks and performs a readback to ensure synchronization.
- Uses `GPIOx_BSRR` to set/reset pins atomically — preferred over direct ODR writes for concurrency safety.

---

## Usage example

Minimal example that toggles PA8 (from `example_main.c`):

```c
#include <stdint.h>
#include "led_driver_stm32.h"

/* crude busy-wait delay */
static void delay(volatile uint32_t n)
{
    while (n--)
    {
        __asm__("nop");
    }
}

int main(void)
{
    while (1)
    {
        LED_Toggle(GPIOA_PA8);     // initialize (idempotent) and toggle PA8
        delay(1000000U);          // crude software delay
    }

    return 0;
}
```

If you prefer toggling a different pin, use the predefined macro or construct a `GPIO_t`:

```c
LED_Toggle(GPIOC_PC13); // toggle PC13
```

---

## Build & flash (general guidance)

This repository contains only source/header files. Integrate into your preferred toolchain/project:

- Create a project for your STM32F4 MCU (toolchains: GNU Arm Embedded toolchain, Keil, IAR, etc.)
- Add a minimal startup (vector table) and linker script suitable for your target device
- Include `led_driver_stm32.h` and a `main.c` (or `example_main.c`) in the project
- Build and flash via your chosen programmer (ST-Link, J-Link, etc.)

Example compile command (GCC, simplified — assumes startup and system files are present):
```sh
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O2 -ffreestanding -nostdlib \
  -T path/to/your.ld startup_stm32.s example_main.c -o firmware.elf
arm-none-eabi-objcopy -O binary firmware.elf firmware.bin
# then flash firmware.bin with st-flash or openocd depending on your setup
```

Always follow recommended board/device setup and reset/power sequencing when flashing.

---

## Limitations & notes

This driver is intentionally minimal for learning purposes:

- No configuration for pull-up / pull-down resistors (PUPDR)
- No output type (push-pull/open-drain) or speed configuration (OTYPER, OSPEEDR)
- No input mode support or interrupt/event handling
- No SysTick or timer-based delays — only crude busy-wait loops
- Base addresses are hard coded for the STM32F4 family — porting to another family (e.g., F1, F7, G0) requires address and register offset adjustments
- The header performs `GPIO_Init()` on each `LED_Toggle()` call for simplicity. In production, track initialization state to avoid repeated configuration.

Recommended production improvements:
- Use `GPIOx_BSRR` for atomic operations (already used here)
- Implement init-once semantics or an explicit `GPIO_Config()` separate from `Toggle`
- Add configuration API to set pull-up/pull-down, speed, type
- Replace busy-wait delays with timer or SysTick-based timing
- Add unit tests and integrate with a build system

---

## Next improvements (suggested)

- Add `GPIO_WritePin()` / `GPIO_SetPin()` / `GPIO_ResetPin()` convenience functions
- Add `GPIO_Config()` that accepts mode/pull/type/speed flags
- Track per-pin initialization status to avoid re-configuring in `LED_Toggle`
- Provide a small Makefile or CMake configuration and CI for build checks
- Add a minimal HAL-like adapter layer for easier transition to production projects

---

## License

This code is provided for educational purposes. You may reuse or adapt the code in accordance with your project's licensing policy. No warranty is provided.

---

If you'd like, I can:
- Add a Makefile/CMake for building the example,
- Implement an init-once optimization (per-pin init tracking),
- Extend the driver with pull-up/pull-down and speed configuration,
- Or adapt the driver to a specific STM32F4 device and provide a preconfigured startup + linker script.

Which improvement would you like next?
