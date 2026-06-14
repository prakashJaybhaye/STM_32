
# 📘 STM32F446xx GPIO Driver (Bare-Metal Custom Hardware Abstraction)

---

# 🧠 Overview

This project is a **bare-metal GPIO driver** for STM32F446xx MCU written in Embedded C.

It is designed using a **layered architecture model**:

- Low-level register mapping
- Hardware abstraction using structs
- Driver APIs similar to HAL but lightweight

---

# 🏗️ Architecture

```
Application Layer
        ↓
GPIO Driver Layer (gpio_driver.c / .h)
        ↓
Peripheral Register Layer (gpio_reg.h, rcc.h)
        ↓
Type Definition Layer (types.h)
        ↓
Hardware (STM32F446xx Registers)
```

---

# 🧩 Design Philosophy

✔ Register-level control (no HAL dependency)  
✔ Modular driver separation  
✔ Scalable interrupt architecture  
✔ Interview-ready embedded design  
✔ Clean abstraction between peripherals  

---

# 🧱 1. Type Abstraction Layer (types.h)

Provides fixed-width and volatile types.

```c
typedef uint32_t u32;
typedef volatile uint32_t vu32;
```

### Why?

- Ensures portability
- Prevents compiler optimization on registers
- Standardizes embedded data types

---

# 🗺️ 2. Memory Mapping Layer

## RCC Base Address
```
0x40023800
```

## GPIO Base Address
```
GPIOA → 0x40020000
GPIOB → 0x40020400
...
```

---

## Example Mapping

```
GPIOA->ODR = base + offset
0x40020000 + 0x14 = 0x40020014
```

---

# ⚙️ 3. GPIO Register Structure

```c
typedef struct {
    vu32 MODER;
    vu32 OTYPER;
    vu32 OSPEEDR;
    vu32 PUPDR;
    vu32 IDR;
    vu32 ODR;
    vu32 BSRR;
    vu32 LCKR;
    vu32 AFR[2];
} GPIO_RegDef_t;
```

---

# 🔌 4. GPIO Driver Features

### Supported APIs

- GPIO_Init()
- GPIO_DeInit()
- GPIO_WritePin()
- GPIO_ReadPin()
- GPIO_TogglePin()
- GPIO_PeripheralClockControl()

---

# ⚡ 5. GPIO Initialization Flow

When `GPIO_Init()` is called:

### Step-by-step

1. Enable GPIO clock via RCC
2. Configure MODER (mode selection)
3. Configure OTYPER (push-pull/open-drain)
4. Configure OSPEEDR (speed control)
5. Configure PUPDR (pull-up/pull-down)
6. Configure AFR (alternate function if required)

---

# 🧠 6. GPIO Modes

| Mode | Description |
|------|------------|
| INPUT | Digital input |
| OUTPUT | Digital output |
| ALTFN | Alternate function |
| ANALOG | Analog mode |
| IT_FT | Interrupt falling edge |
| IT_RT | Interrupt rising edge |
| IT_RFT | Both edges |

---

# 🚨 7. Interrupt Architecture

## Important Design Rule

GPIO driver DOES NOT handle interrupts.

### Correct separation:

| Module | Responsibility |
|--------|--------------|
| GPIO | Pin configuration |
| SYSCFG | EXTI line mapping |
| EXTI | Interrupt trigger |
| NVIC | Interrupt enable |

---

## Why?

STM32 architecture is:

```
GPIO → EXTI → NVIC
```

So mixing interrupt logic inside GPIO driver is incorrect.

---

# ⚙️ 8. GPIO Write Operation

Uses BSRR register for atomic operation:

```c
Set:
BSRR = (1 << pin)

Reset:
BSRR = (1 << (pin + 16))
```

---

# 📥 9. GPIO Read

Reads input register:

```c
value = (IDR >> pin) & 1;
```

---

# 🔁 10. GPIO Toggle

```c
ODR ^= (1 << pin);
```

---

# 🔄 11. Peripheral Reset

GPIO reset is done using RCC:

```
RCC->AHB1RSTR
```

---

# 📌 12. Example Usage

```c
GPIO_Handler_t led;

led.pGPIOx = GPIOA;

led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_5;
led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
led.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

GPIO_Init(&led);

GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
```

---

# 🚀 13. Project Strength

✔ Clean layered architecture  
✔ Register-level understanding  
✔ Scalable design for EXTI/NVIC  
✔ Real embedded system modeling  
✔ Strong interview project base  

---

# 🧾 14. Future Extensions

- EXTI Driver
- NVIC Driver
- UART Driver
- SPI Driver
- I2C Driver
- RTOS integration

---

# 🏁 Conclusion

This GPIO driver represents a **professional embedded system design approach**, moving from raw register access → structured driver abstraction.

It is suitable for:
- Embedded interviews
- Portfolio projects
- Firmware development base

---
