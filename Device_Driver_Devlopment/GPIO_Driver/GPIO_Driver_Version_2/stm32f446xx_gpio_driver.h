#ifndef STM32F446XX_GPIO_DRIVER_H
#define STM32F446XX_GPIO_DRIVER_H

#include "stm32f446xx_types.h"
#include "stm32f446xx_rcc.h"
#include "stm32f446xx_gpio_reg.h"

/*
 * =========================================================
 * GPIO CLOCK ENABLE MACROS
 * =========================================================
 * These macros enable clock for GPIO peripherals via RCC AHB1ENR.
 * Without enabling clock, GPIO registers are not accessible.
 * =========================================================
 */

#define GPIOA_CLK_EN() (RCC->AHB1ENR |= (1 << 0)) /* Enable clock for GPIOA */
#define GPIOB_CLK_EN() (RCC->AHB1ENR |= (1 << 1)) /* Enable clock for GPIOB */
#define GPIOC_CLK_EN() (RCC->AHB1ENR |= (1 << 2)) /* Enable clock for GPIOC */
#define GPIOD_CLK_EN() (RCC->AHB1ENR |= (1 << 3)) /* Enable clock for GPIOD */
#define GPIOE_CLK_EN() (RCC->AHB1ENR |= (1 << 4)) /* Enable clock for GPIOE */
#define GPIOF_CLK_EN() (RCC->AHB1ENR |= (1 << 5)) /* Enable clock for GPIOF */
#define GPIOG_CLK_EN() (RCC->AHB1ENR |= (1 << 6)) /* Enable clock for GPIOG */
#define GPIOH_CLK_EN() (RCC->AHB1ENR |= (1 << 7)) /* Enable clock for GPIOH */


/*
 * =========================================================
 * GPIO RESET MACROS
 * =========================================================
 * Forces GPIO peripheral reset using RCC AHB1RSTR register.
 * Reset is done by setting bit then clearing it.
 * =========================================================
 */

#define GPIOA_CLK_RST() do { (RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0)); } while(0)
#define GPIOB_CLK_RST() do { (RCC->AHB1RSTR |= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1)); } while(0)
#define GPIOC_CLK_RST() do { (RCC->AHB1RSTR |= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2)); } while(0)
#define GPIOD_CLK_RST() do { (RCC->AHB1RSTR |= (1 << 3)); (RCC->AHB1RSTR &= ~(1 << 3)); } while(0)
#define GPIOE_CLK_RST() do { (RCC->AHB1RSTR |= (1 << 4)); (RCC->AHB1RSTR &= ~(1 << 4)); } while(0)
#define GPIOF_CLK_RST() do { (RCC->AHB1RSTR |= (1 << 5)); (RCC->AHB1RSTR &= ~(1 << 5)); } while(0)
#define GPIOG_CLK_RST() do { (RCC->AHB1RSTR |= (1 << 6)); (RCC->AHB1RSTR &= ~(1 << 6)); } while(0)
#define GPIOH_CLK_RST() do { (RCC->AHB1RSTR |= (1 << 7)); (RCC->AHB1RSTR &= ~(1 << 7)); } while(0)


#define BSRR_PIN_RESET 16
/*
 * Used for GPIO pin reset operation in BSRR register.
 * Lower 16 bits = SET
 * Upper 16 bits = RESET (Pin + 16 offset)
 */


/*
 * =========================================================
 * GPIO PIN NUMBERS
 * =========================================================
 * Represents physical GPIO pin numbers (0–15).
 * Each GPIO port has 16 pins mapped to bit positions.
 * =========================================================
 */

typedef enum 
{
    GPIO_PIN_0 = 0,
    GPIO_PIN_1 = 1,
    GPIO_PIN_2 = 2,
    GPIO_PIN_3 = 3,
    GPIO_PIN_4 = 4,
    GPIO_PIN_5 = 5,
    GPIO_PIN_6 = 6,
    GPIO_PIN_7 = 7,
    GPIO_PIN_8 = 8,
    GPIO_PIN_9 = 9,
    GPIO_PIN_10 = 10,
    GPIO_PIN_11 = 11,
    GPIO_PIN_12 = 12,
    GPIO_PIN_13 = 13,
    GPIO_PIN_14 = 14,
    GPIO_PIN_15 = 15
} GPIO_Pin_t;


/*
 * =========================================================
 * GPIO ALTERNATE FUNCTION SELECTION
 * =========================================================
 * Each GPIO pin can be mapped to multiple peripherals.
 * AFRL → pins 0–7, AFRH → pins 8–15
 * =========================================================
 */

typedef enum
{
    GPIO_AF0  = 0,   /* System functions (SYS, MCO, RTC) */
    GPIO_AF1  = 1,   /* TIM1 / TIM2 */
    GPIO_AF2  = 2,   /* TIM3 / TIM4 / TIM5 */
    GPIO_AF3  = 3,   /* TIM8–TIM11 */
    GPIO_AF4  = 4,   /* I2C1–I2C3 */
    GPIO_AF5  = 5,   /* SPI1 / SPI2 */
    GPIO_AF6  = 6,   /* SPI3 */
    GPIO_AF7  = 7,   /* USART1 / USART2 / USART3 */
    GPIO_AF8  = 8,   /* UART4 / UART5 / USART6 */
    GPIO_AF9  = 9,   /* CAN / TIM12–14 */
    GPIO_AF10 = 10,  /* USB OTG */
    GPIO_AF11 = 11,  /* Ethernet */
    GPIO_AF12 = 12,  /* FMC / SDIO */
    GPIO_AF13 = 13,  /* DCMI */
    GPIO_AF14 = 14,  /* LTDC */
    GPIO_AF15 = 15   /* EVENTOUT */
} GPIO_AltFunction_t;


/*
 * =========================================================
 * GPIO PORT IDENTIFIER
 * =========================================================
 * Used for identifying GPIO ports in abstraction layer.
 * Useful for SYSCFG / EXTI mapping.
 * =========================================================
 */

typedef enum
{
    GPIO_PORTA = 0,
    GPIO_PORTB = 1,
    GPIO_PORTC = 2,
    GPIO_PORTD = 3,
    GPIO_PORTE = 4,
    GPIO_PORTF = 5,
    GPIO_PORTG = 6,
    GPIO_PORTH = 7
} GPIO_Port_Code_t;


/*
 * =========================================================
 * GPIO MODES
 * =========================================================
 * Defines functional behavior of GPIO pin.
 * Includes input/output/alternate/analog/interrupt modes.
 * =========================================================
 */

typedef enum
{
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT = 1,
    GPIO_MODE_ALTFN = 2,
    GPIO_MODE_ANALOG = 3,

    GPIO_MODE_IT_FT = 4,   /* Falling edge interrupt */
    GPIO_MODE_IT_RT = 5,   /* Rising edge interrupt */
    GPIO_MODE_IT_RFT = 6   /* Both edges interrupt */
} GPIO_Mode_t;


/* GPIO OUTPUT TYPE */
typedef enum
{
    GPIO_OP_TYPE_PP = 0,
    GPIO_OP_TYPE_OD = 1
} GPIO_OutputType_t;


/* GPIO SPEED CONTROL */
typedef enum
{
    GPIO_SPEED_LOW = 0,
    GPIO_SPEED_MEDIUM = 1,
    GPIO_SPEED_FAST = 2,
    GPIO_SPEED_HIGH = 3
} GPIO_Speed_t;


/* GPIO PULL CONFIGURATION */
typedef enum
{
    GPIO_NO_PUPD = 0,
    GPIO_PU = 1,
    GPIO_PD = 2
} GPIO_PuPd_t;


/*
 * =========================================================
 * GPIO PIN CONFIG STRUCTURE
 * =========================================================
 * Used to configure a GPIO pin in GPIO_Init()
 * =========================================================
 */

typedef struct
{
    GPIO_Pin_t GPIO_PinNumber;
    GPIO_Mode_t GPIO_PinMode;
    GPIO_Speed_t GPIO_PinSpeed;
    GPIO_OutputType_t GPIO_PinOPType;
    GPIO_PuPd_t GPIO_PinPuPdControl;
    uint8_t GPIO_PinAltFunMode;
} GPIO_PinCfg_t;


/*
 * =========================================================
 * GPIO HANDLE STRUCTURE
 * =========================================================
 * Combines GPIO port + configuration
 * =========================================================
 */

typedef struct
{
    GPIO_RegDef_t *pGPIOx;
    GPIO_PinCfg_t GPIO_PinConfig;
} GPIO_Handler_t;


/* GPIO PIN STATE */
typedef enum
{
    GPIO_PIN_RESET = 0,
    GPIO_PIN_SET
} GPIO_PinState_t;


/*
 * =========================================================
 * GPIO DRIVER APIs
 * =========================================================
 */

void GPIO_Init(GPIO_Handler_t *pGPIOHandler);
void GPIO_PeripheralClockControl(GPIO_RegDef_t *pGPIOx, u8 ENABLE);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);
void GPIO_WritePin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, GPIO_PinState_t Value);
GPIO_PinState_t GPIO_ReadPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
void GPIO_TogglePin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);



#endif // STM32F446XX_GPIO_DRIVER_H
