✅ Completed Learning & Implementation
1. STM32 Documentation & Architecture
Learned how to read and understand STM32 datasheets and reference manuals
Understood memory mapping, peripheral base addresses, and offset addressing
Learned how to access peripheral registers using base address + offset

3. GPIO & RCC Register-Level Programming
Studied and implemented GPIO configuration using register-level programming
Learned usage and purpose of the following registers:
RCC (AHB1ENR) – Peripheral clock enabling
GPIOxMODER – GPIO mode configuration
GPIOxODR – Output Data Register
GPIOxIDR – Input Data Register
GPIOxBSRR – Atomic set/reset operations
Implemented GPIO input/output configuration without HAL or external libraries

3. Bare-Metal LED Control
Wrote LED blinking code entirely in bare-metal C
Created a custom LED Toggle API:
LED_Toggle(PIN_Number);
Can toggle any GPIO pin
No HAL, CMSIS abstraction, or external libraries used

4. Button Input & GPIO IDR Usage
Learned how GPIO IDR (Input Data Register) works
Implemented:
LED toggling on button press
LED remains ON while the button is continuously pressed
Pure register-level implementation

6. SysTick Timer & Delay Implementation
Learned Cortex-M4 SysTick timer registers
Designed and implemented an accurate delay function
Used SysTick for precise LED blinking timing
Bare-metal implementation without HAL

6. External Interrupts (EXTI)
Learned external interrupt architecture
Configured EXTI at register level
Implemented LED toggling on external interrupt
Fully bare-metal interrupt handling

6. Genral perpose timmer
Lerned about how timmer works
How many register are involved in TM2
work flow of TM2
Implementation of basic delay function using HSI and TM2
Implementation of own Delay function using HSI and TM2 Interrupt handler.
Learn How PWM is generated form Genral perpose timmer.
Implementated code to control brightness of led with TM2 and delay by sytimmer.

*Project Implementation 
1. 4 Bit Counter Design in 3 different manner
   a) For Loop and Delay base for evert 2-3 second counter will increase form 0-15(SysTimer is Useed for own Delay Function)
   b) Push Button base on every button press counter will increase form 0-15.
   c) EXTI and NVIC base on every button press interrupt will fire and counter will increase form 0-15

🎯 Goal & Roadmap
Short-Term Goals
Master true bare-metal programming on STM32 microcontrollers
Avoid usage of:
HAL libraries
Pre-written driver libraries
Build custom low-level drivers from scratch
Long-Term Goals
Learn and implement all major STM32 peripherals, including:
GPIO
Timers
ADC
DAC
UART, SPI, I²C, CAN
Understand communication protocols from low-level signaling to full driver design
Learn and implement RTOS concepts:
Task scheduling
Context switching
Interrupt management

