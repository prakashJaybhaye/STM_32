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

7. UART (Universal Asyncronus Reciver Tarnsmmiter ) 
Learned the fundamentals of the UART communication protocol and serial data transmission.
Understood the USART2 peripheral architecture and the registers required for UART configuration.
Studied baud rate calculation using the HSI (High-Speed Internal) clock source.
Implemented UART driver functions from scratch, including:
UART_SendChar() for transmitting a single character
UART_SendString() for sending complete strings
UART_ReceiveChar() using a non-blocking reception method
UART_ReceiveString() for receiving strings over UART
Developed practical experience with embedded serial communication and real-time data exchange between the STM32 microcontroller and a serial terminal.


*Driver Development
 STM32 Hardware Abstraction Layer
 Implemented stm32f446xx.h, containing:
  - Peripheral base addresses
  - Register definitions
  - Register offset mappings
  - Memory-mapped hardware access macros for the STM32F446 microcontroller

 GPIO Driver Implementation
  - GPIO pin initialization
  - Digital output control
  - Pin state toggling
  - Direct register-level manipulation
    void GPIO_Init(GPIO_t gpio);
    void GPIO_Toggle(GPIO_t gpio);
    void GPIO_Set(GPIO_t gpio);
    void GPIO_Reset(GPIO_t gpio);

 SysTick Timer Driver
  - Millisecond delay generation
  - Periodic task scheduling
  - Non-blocking software timing mechanism
  - Real-time event timing using the SysTick peripheral
    void SysTick_Timer_DelayMs(uint32_t ms);
    uint8_t SysTick_is_time_elapsed(uint32_t *last, uint32_t delay);

 USART Driver Implementation
  - USART2 peripheral initialization
  - Character and string transmission
  - Non-blocking UART reception
  - String-based serial communication
  - Embedded CLI communication support
    void USART2_Init(void);
    void USART2_SendChar(char c);
    void USART2_SendString(const char *str);
    uint8_t USART2_ReceiveChar(unsigned char *c);
    uint8_t USART2_ReceiveString(char *buffer, uint32_t max_length);


*Project Implementation 
1. 4 Bit Counter Design in 3 different manner
   a) For Loop and Delay base for evert 2-3 second counter will increase form 0-15(SysTimer is Useed for own Delay Function)
   b) Push Button base on every button press counter will increase form 0-15.
   c) EXTI and NVIC base on every button press interrupt will fire and counter will increase form 0-15

2. Event-driven Moore finite state machine implemented in a super-loop architecture
   LED and switch base state machine on every buttom press led will
   LED Off
   LED ON
   LED TOGGLE 
   LED PWM
   EXTI used for button state

3. UART CLI Counter
   This project implements a UART Command Line Interface (CLI) controlled 4-bit binary counter using the STM32F446 microcontroller.
   The system displays a continuously incrementing 4-bit binary count on four LEDs connected to GPIO pins PB12–PB15.
   User commands are sent through UART communication to control the counter behavior in real time.

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

