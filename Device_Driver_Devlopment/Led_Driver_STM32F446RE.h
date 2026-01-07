#ifndef LED_DRIVER_STM32_H
#define LED_DRIVER_STM32_H

#include <stdint.h>

/*------------------------------RCC BASE ADDRESS ---------------------------------*/

#define RCC_BASE 0x40023800UL
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30U))

/*-------------------------------GPIOA CONFIGRATION----------------------------------*/

#define GPIOA_BASE 0x40020000UL
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + 0x00U))
#define GPIOA_ODR (*(volatile uint32_t *)(GPIOA_BASE + 0x14U))
#define GPIOA_BSRR (*(volatile uint32_t *)(GPIOA_BASE + 0x18U))

/*-------------------------------GPIOB CONFIGRATION----------------------------------*/
#define GPIOB_BASE 0x40020400UL
#define GPIOB_MODER (*(volatile uint32_t *)(GPIOB_BASE + 0x00U))
#define GPIOB_ODR (*(volatile uint32_t *)(GPIOB_BASE + 0x14U))
#define GPIOB_BSRR (*(volatile uint32_t *)(GPIOB_BASE + 0x18U))

/*-------------------------------GPIOC CONFIGRATION----------------------------------*/
#define GPIOC_BASE 0x40020800UL
#define GPIOC_MODER (*(volatile uint32_t *)(GPIOC_BASE + 0x00U))
#define GPIOC_ODR (*(volatile uint32_t *)(GPIOC_BASE + 0x14U))
#define GPIOC_BSRR (*(volatile uint32_t *)(GPIOC_BASE + 0x18U))

typedef enum GPIO_PORTS
{
    GPIOA = 0,
    GPIOB = 1,
    GPIOC = 2
} GPIO_PORTS;

typedef struct GPIO_t
{
    GPIO_PORTS Port;
    uint8_t Pin;
} GPIO_t;

static const GPIO_t GPIOA_PA0 = {GPIOA, 0};
static const GPIO_t GPIOA_PA1 = {GPIOA, 1};
static const GPIO_t GPIOA_PA2 = {GPIOA, 2};
static const GPIO_t GPIOA_PA3 = {GPIOA, 3};
static const GPIO_t GPIOA_PA4 = {GPIOA, 4};
static const GPIO_t GPIOA_PA5 = {GPIOA, 5};
static const GPIO_t GPIOA_PA6 = {GPIOA, 6};
static const GPIO_t GPIOA_PA7 = {GPIOA, 7};
static const GPIO_t GPIOA_PA8 = {GPIOA, 8};
static const GPIO_t GPIOA_PA9 = {GPIOA, 9};
static const GPIO_t GPIOA_PA10 = {GPIOA, 10};
static const GPIO_t GPIOA_PA11 = {GPIOA, 11};
static const GPIO_t GPIOA_PA12 = {GPIOA, 12};
static const GPIO_t GPIOA_PA13 = {GPIOA, 13};
static const GPIO_t GPIOA_PA14 = {GPIOA, 14};
static const GPIO_t GPIOA_PA15 = {GPIOA, 15};

static const GPIO_t GPIOB_PB0 = {GPIOB, 0};
static const GPIO_t GPIOB_PB1 = {GPIOB, 1};
static const GPIO_t GPIOB_PB2 = {GPIOB, 2};
static const GPIO_t GPIOB_PB3 = {GPIOB, 3};
static const GPIO_t GPIOB_PB4 = {GPIOB, 4};
static const GPIO_t GPIOB_PB5 = {GPIOB, 5};
static const GPIO_t GPIOB_PB6 = {GPIOB, 6};
static const GPIO_t GPIOB_PB7 = {GPIOB, 7};
static const GPIO_t GPIOB_PB8 = {GPIOB, 8};
static const GPIO_t GPIOB_PB9 = {GPIOB, 9};
static const GPIO_t GPIOB_PB10 = {GPIOB, 10};
static const GPIO_t GPIOB_PB12 = {GPIOB, 12};
static const GPIO_t GPIOB_PB13 = {GPIOB, 13};
static const GPIO_t GPIOB_PB14 = {GPIOB, 14};
static const GPIO_t GPIOB_PB15 = {GPIOB, 15};

static const GPIO_t GPIOC_PC0 = {GPIOC, 0};
static const GPIO_t GPIOC_PC1 = {GPIOC, 1};
static const GPIO_t GPIOC_PC2 = {GPIOC, 2};
static const GPIO_t GPIOC_PC3 = {GPIOC, 3};
static const GPIO_t GPIOC_PC4 = {GPIOC, 4};
static const GPIO_t GPIOC_PC5 = {GPIOC, 5};
static const GPIO_t GPIOC_PC6 = {GPIOC, 6};
static const GPIO_t GPIOC_PC7 = {GPIOC, 7};
static const GPIO_t GPIOC_PC8 = {GPIOC, 8};
static const GPIO_t GPIOC_PC9 = {GPIOC, 9};
static const GPIO_t GPIOC_PC10 = {GPIOC, 10};
static const GPIO_t GPIOC_PC11 = {GPIOC, 11};
static const GPIO_t GPIOC_PC12 = {GPIOC, 12};
static const GPIO_t GPIOC_PC13 = {GPIOC, 13};
static const GPIO_t GPIOC_PC14 = {GPIOC, 14};
static const GPIO_t GPIOC_PC15 = {GPIOC, 15};

void GPIO_Init(GPIO_t GPIO_Port_Pin)
{
    switch (GPIO_Port_Pin.Port)
    {
    case GPIOA:
        RCC_AHB1ENR |= (1 << GPIO_Port_Pin.Port);
        GPIOA_MODER &= ~(3 << (2 * GPIO_Port_Pin.Pin));
        GPIOA_MODER |= (1 << (2 * GPIO_Port_Pin.Pin));
        break;

    case GPIOB:
        RCC_AHB1ENR |= (1 << GPIO_Port_Pin.Port);
        GPIOB_MODER &= ~(3 << (2 * GPIO_Port_Pin.Pin));
        GPIOB_MODER |= (1 << (2 * GPIO_Port_Pin.Pin));
        break;

    case GPIOC:
        RCC_AHB1ENR |= (1 << GPIO_Port_Pin.Port);
        GPIOC_MODER &= ~(3 << (2 * GPIO_Port_Pin.Pin));
        GPIOC_MODER |= (1 << (2 * GPIO_Port_Pin.Pin));
        break;

    default:
        break;
    }
}

void GPIO_TogglePin(volatile uint32_t *GPIOx_ODR, volatile uint32_t *GPIOx_BSRR, GPIO_t GPIO_Port_Pin)

{
    if (*GPIOx_ODR & (1 << GPIO_Port_Pin.Pin))
    {
        *GPIOx_BSRR = 1 << (GPIO_Port_Pin.Pin + 16);
    }

    else
    {
        *GPIOx_BSRR = 1 << GPIO_Port_Pin.Pin;
    }
}

void LED_Toggle(GPIO_t GPIO_Port_Pin)
{
    GPIO_Init(GPIO_Port_Pin);

    switch (GPIO_Port_Pin.Port)
    {
    case GPIOA:
        GPIO_TogglePin(&GPIOA_ODR, &GPIOA_BSRR, GPIO_Port_Pin);
        break;

    case GPIOB:
        GPIO_TogglePin(&GPIOB_ODR, &GPIOB_BSRR, GPIO_Port_Pin);
        break;

    case GPIOC:
        GPIO_TogglePin(&GPIOC_ODR, &GPIOC_BSRR, GPIO_Port_Pin);
        break;

    default:
        break;
    }
}

#endif
