# External Interrupt (EXTI) — LED Toggle Example

This repository contains a minimal bare-metal example for the STM32 family that demonstrates toggling LEDs in two ways:
- A main loop toggles one LED using the SysTick-based delay.
- An external interrupt (EXTI) triggered by a button toggles a second LED from the IRQ handler.

This example is implemented in plain C using direct peripheral register access (no HAL/LL libraries) and is intended for learning and experimentation.

## Files
- External_Interrupt_EXTI/LED_Toggle_Interrupt_Base.c — Main source file implementing GPIO, EXTI and SysTick usage.

(Referenced source: [LED_Toggle_Interrupt_Base.c](https://github.com/prakashJaybhaye/STM_32/blob/063ed98962a5f401450d0e37768f0785667d3cee/External_Interrupt_EXTI/LED_Toggle_Interrupt_Base.c))

## Target MCU / Board
- STM32 (example uses STM32F4 family register addresses and assumptions — the code comments mention STM32F411).
- Example assumes GPIOA pins are available (PA1, PA2, PA3). Adjust pin definitions for your board.

## Pinout (as used in the code)
- PA1  — LED_PIN      (toggled from EXTI2 IRQ handler)
- PA2  — BUTTON        (external interrupt source)
- PA3  — LED_PIN_1    (toggled in main loop every 1 second)

Modify these macros in the source if your board uses different pins:
- LED_PIN (default 1)
- LED_PIN_1 (default 3)
- BUTTON (default 2)

## How it works (summary)
- RCC registers are used to enable GPIOA and SYSCFG clocks.
- GPIOA pins PA1 and PA3 are configured as outputs. PA2 is configured as input (button).
- SYSCFG external interrupt configuration register (EXTICR1) is cleared for EXTI2 mapping to PA2.
- EXTI2 is unmasked (IMR) and rising trigger is enabled (RTSR) so a rising edge on PA2 will generate EXTI2 interrupt.
- NVIC ISER0 bit is set to enable the EXTI2 IRQ in the NVIC.
- SysTick is configured to create a millisecond tick (polled via the COUNTFLAG in the SysTick CSR).
- The main loop toggles LED_PIN_1 every 1000 ms using delay_ms.
- EXTI2_IRQHandler toggles LED_PIN when a button press occurs and clears EXTI_PR pending bit.

## Important code notes
- The delay implementation polls SysTick COUNTFLAG (bit 16 of SYST_CSR). SysTick is set up with a 1 kHz reload to create millisecond ticks.
- The code uses direct register access (volatile pointers and #define address macros), so it is not portable between different STM32 families without verifying register addresses.
- The code enables the SYSCFG APB2 clock (RCC_APB2ENR bit 14) before configuring SYSCFG_EXTICR1.
- NVIC enable is performed by writing to NVIC_ISER0. Ensure the correct IRQ number is used for your device; this example enables bit 8 which corresponds to the EXTI2 IRQ on the target used by the author. Confirm with your MCU's reference manual.

## Build (example)
This project does not include a full build system. Below are minimal commands to compile with the GNU ARM toolchain. Adjust include paths, startup files and linker script as required for your board.

Prerequisites:
- arm-none-eabi-gcc toolchain
- A startup file and linker script for your MCU
- Alternatively, use an IDE like STM32CubeIDE or Keil

Example compile command (replace `startup.s` and `stm32_flash.ld` with appropriate files for your MCU):

```bash
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O2 -g \
  -T stm32_flash.ld \
  -nostdlib \
  -Wl,--gc-sections \
  startup.s External_Interrupt_EXTI/LED_Toggle_Interrupt_Base.c -o led_exti.elf

arm-none-eabi-objcopy -O binary led_exti.elf led_exti.bin
```

A minimal Makefile snippet:

```make
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -mcpu=cortex-m4 -mthumb -O2 -g -Wall
LD_SCRIPT = stm32_flash.ld
SRC = External_Interrupt_EXTI/LED_Toggle_Interrupt_Base.c
STARTUP = startup.s

all: led_exti.bin

led_exti.elf: $(SRC) $(STARTUP) $(LD_SCRIPT)
	$(CC) $(CFLAGS) -T $(LD_SCRIPT) -nostdlib -Wl,--gc-sections $(STARTUP) $(SRC) -o $@

led_exti.bin: led_exti.elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f led_exti.elf led_exti.bin
```

## Flashing
Use your preferred programmer (ST-Link, OpenOCD, J-Link, etc.). Example with OpenOCD + st-flash or using `st-link`:

OpenOCD:
```bash
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
  -c "program led_exti.bin 0x08000000 verify reset exit"
```

st-flash:
```bash
st-flash write led_exti.bin 0x8000000
```

Adjust flash address to match your MCU boot memory map.

## Running and Testing
1. Flash the binary to your board.
2. Power/reset the board.
3. The LED connected to PA3 (LED_PIN_1) should toggle every 1 second.
4. Press the push-button connected to PA2; on a rising edge the EXTI2 IRQ handler toggles the LED on PA1 (LED_PIN).

## Limitations & Notes
- Debouncing: The example does not debounce the button. Mechanical switches may generate bounces and multiple interrupts on a single press.
- Safety: Code runs with interrupts enabled in NVIC for EXTI2. Ensure interrupt priorities and other system settings are configured correctly for your application.
- SysTick usage: SysTick is polled for delay; this avoids using the SysTick interrupt but prevents low-power usage during delay loops.
- Registers and IRQ numbers: The bit used when setting NVIC_ISER0 (1 << 8) maps to an IRQn number; verify with the device-specific vector table in your reference manual. Different devices or families may have different IRQ numbers for EXTI lines.
- No startup or vector table is included in this example. You must provide proper startup code (vectors) in your build so that the EXTI2_IRQHandler symbol is used in the vector table.

## Suggested improvements
- Add button debouncing (software or hardware).
- Use HAL/LL for portability across STM32 families (or provide separate versions).
- Add vector table and a sample linker script/startup for the specific MCU to make the project directly buildable.
- Implement EXTI falling-edge support or bi-directional control.
- Use a proper SysTick driver that supports delay and elapsed time without busy polling (or use an interrupt-driven SysTick).

## License
This example is provided as-is for educational purposes. You may reuse and modify it. Consider adding a specific license for your repository (e.g., MIT).

## Author
Original example from repository: prakashJaybhaye/STM_32  
File referenced: External_Interrupt_EXTI/LED_Toggle_Interrupt_Base.c
