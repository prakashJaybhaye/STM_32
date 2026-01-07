STM32 LED Driver (Register-Level GPIO Control)
Overview

This repository contains a lightweight STM32 GPIO LED driver implemented using direct register access (no HAL/LL libraries).
It provides a simple abstraction to initialize GPIO pins and toggle LEDs on GPIOA, GPIOB, and GPIOC.

The driver is designed for STM32F4-series microcontrollers (RCC and GPIO base addresses follow STM32F4 memory mapping).

Features

🚀 Direct register-level programming (bare-metal)

🔌 Supports GPIO ports A, B, and C

💡 Easy LED toggling using a single API

🧩 Lightweight and dependency-free

🛠️ Suitable for learning, low-level drivers, and embedded fundamentals

File Structure
.
├── led_driver_stm32.h   # GPIO & LED driver implementation
└── README.md

Supported Microcontrollers

STM32F4 series (e.g., STM32F401, STM32F411, STM32F446)

⚠️ Base addresses are hardcoded. Using a different STM32 family may require changes.

GPIO Abstraction

Each GPIO pin is represented using the GPIO_t structure:

typedef struct GPIO_t
{
    GPIO_PORTS Port;
    uint8_t Pin;
} GPIO_t;

Predefined GPIO Pins

Examples:

GPIOA_PA5   // Port A, Pin 5
GPIOB_PB0   // Port B, Pin 0
GPIOC_PC13  // Port C, Pin 13

API Reference
1. GPIO Initialization
void GPIO_Init(GPIO_t GPIO_Port_Pin);


Enables the GPIO clock

Configures the pin as output mode

2. Toggle GPIO Pin (Low-level)
void GPIO_TogglePin(
    volatile uint32_t *GPIOx_ODR,
    volatile uint32_t *GPIOx_BSRR,
    GPIO_t GPIO_Port_Pin
);


Uses ODR and BSRR registers

Atomic pin set/reset

3. LED Toggle (High-level API)
void LED_Toggle(GPIO_t GPIO_Port_Pin);


Initializes the GPIO pin

Toggles the LED state

✅ Recommended function for application use.

Example Usage
#include "led_driver_stm32.h"

int main(void)
{
    while (1)
    {
        LED_Toggle(GPIOA_PA5);  // Toggle LED on PA5
        for (volatile int i = 0; i < 100000; i++); // Delay
    }
}

How It Works

Clock Enable
Enables AHB1 clock for the selected GPIO port.

Pin Configuration
Sets GPIO pin mode to output.

Toggle Logic

Reads current pin state from ODR

Uses BSRR to atomically set or reset the pin

Limitations

❌ No input mode support

❌ No pull-up/pull-down configuration

❌ No speed or output type configuration

❌ No interrupt support

This driver is intentionally minimal and educational.

Future Improvements

Add GPIO input support

Add configurable pin modes

Add delay utility

Add multi-LED support

Port to other STM32 families

License

This project is open-source and free to use for learning and personal projects.
You may add a license (MIT / Apache 2.0) as needed.

Author

Developed for bare-metal STM32 learning and experimentation.

Happy hacking! 🔧💡
