# 🚀 STM32 Bare-Metal Project Template (IDE-Free)

---

## 📌 Overview

A clean, minimal, and **fully IDE-independent** STM32 bare-metal template.

This project is designed for developers who want to:
- Understand what happens **behind the IDE**
- Build firmware using **Makefile + GCC**
- Work at **register level**
- Gain **real embedded systems knowledge**

---

## 🎯 Why Use This Template?

Most IDEs hide critical low-level details.

This template exposes:

- 🔧 Startup flow (Reset → main)
- 🧠 Memory layout (Flash, RAM, Stack, Heap)
- ⚙️ Linker script control
- 🧩 Register-level drivers
- 🐞 Real debugging using GDB

---

## 🧭 Startup Flow (How MCU Executes Your Code)

```
Power ON / Reset
        │
        ▼
+----------------------+
|  Vector Table        |
|  (startup_xxx.s)     |
+----------------------+
        │
        ▼
+----------------------+
| Reset_Handler()      |
+----------------------+
        │
        ├── Initialize .data
        ├── Zero .bss
        ├── Setup stack
        │
        ▼
+----------------------+
|      main()          |
+----------------------+
        │
        ▼
 Application Runs
```

---

## 🧠 Memory Layout (Linker Controlled)

```
FLASH (0x08000000)
+------------------------+
| Vector Table           |
| .text (code)           |
| .rodata (constants)    |
+------------------------+

RAM (0x20000000)
+------------------------+
| .data (initialized)    |
| .bss (zero init)       |
| Heap                   |
| Stack                  |
+------------------------+
```

---

## 📂 Project Structure

```
.
├── Core/
│   ├── Inc/        # Application headers
│   └── Src/        # main.c
│
├── Drivers/
│   ├── Inc/        # Peripheral headers
│   └── Src/        # Peripheral drivers
│
├── Startup/
│   └── startup_xxx.s   # Vector table
│
├── Linker/
│   └── memory.ld       # Memory mapping
│
├── .vscode/
│   └── launch.json     # Debug config
│
└── Makefile            # Build system
```

---

## 🛠️ Required Tools

### 1. ARM GCC Toolchain
- arm-none-eabi-gcc
- arm-none-eabi-gdb  
---

### 2. OpenOCD


---

### 3. Debug Probe
- ST-Link / J-Link

---

### 4. VS Code (Recommended)

Extensions:
- C/C++
- Cortex-Debug

---

## ⚙️ Build Instructions

### Clean
```
make clean
```

### Build
```
make
```

### Output Files

```
main.elf   → Debug file
main.bin   → Flashable binary
main.map   → Memory usage
```

---

## 🚀 Flash Firmware

```
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
-c "program main.elf verify reset exit"
```

---

## 🐞 Debugging Flow

```
VS Code (F5)
      │
      ▼
Cortex-Debug
      │
      ▼
GDB Server (OpenOCD)
      │
      ▼
STM32 MCU
```

---

## 🧩 How to Use

1. Clone repo
```
git clone <repo-url>
```

2. Write application code
```
Core/Src/main.c
```

3. Add drivers
```
Drivers/
```

4. Build & flash

---

## 🧠 What You Will Learn

- MCU boot process
- Vector table & interrupts
- Memory mapping via linker
- Register-level programming
- Bare-metal debugging

---
---

## ⚠️ Important Notes

- Clock configuration affects all peripherals
- Incorrect timing → communication errors
- Always verify with debugger / logic analyzer

---

## 📜 License

Free for learning and development.

---

## 💬 Final Thought

> If you understand this template, you understand what IDEs hide.

This is the foundation of **real embedded engineering**.
