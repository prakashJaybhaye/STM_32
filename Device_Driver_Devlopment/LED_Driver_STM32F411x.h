#ifndef LED_DRIVER_STM32_H
#define LED_DRIVER_STM32_H

#include <stdint.h>

/*-------------------------------RCC ENABLE & AHB1 ENABL------------------------------------------------*/
#define RCC_BASE 0x40023800UL
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30))

/*-------------------------------------------GPIOA--------------------------------------------------------*/

#define GPIOA_BASE 0x40020000UL
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR (*(volatile uint32_t *)(GPIOA_BASE + 0x14))
#define GPIOA_BSRR (*(volatile uint32_t *)(GPIOA_BASE + 0x18))

/*-------------------------------------------GPIOB--------------------------------------------------------*/

#define GPIOB_BASE 0x40020400UL
#define GPIOB_MODER (*(volatile uint32_t *)(GPIOB_BASE + 0x00))
#define GPIOB_ODR (*(volatile uint32_t *)(GPIOB_BASE + 0x14))
#define GPIOB_BSRR (*(volatile uint32_t *)(GPIOB_BASE + 0x18))

/*-------------------------------------------GPIOC--------------------------------------------------------*/

#define GPIOC_BASE 0x40020800UL
#define GPIOC_MODER (*(volatile uint32_t *)(GPIOC_BASE + 0x00))
#define GPIOC_ODR (*(volatile uint32_t *)(GPIOC_BASE + 0x14))
#define GPIOC_BSRR (*(volatile uint32_t *)(GPIOC_BASE + 0x18))

typedef enum GPIO_PORTS
{
    GPIOA = 0,
    GPIOB = 1,
    GPIOC = 2,
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

static const GPIO_t GPIOB_PB0 = {GPIOB, 0};
static const GPIO_t GPIOB_PB1 = {GPIOB, 1};
static const GPIO_t GPIOB_PB2 = {GPIOB, 2};
static const GPIO_t GPIOB_PB10 = {GPIOB, 10};

static const GPIO_t GPIOC_PC13 = {GPIOC, 13};
static const GPIO_t GPIOC_PC14 = {GPIOC, 14};
static const GPIO_t GPIOC_PC15 = {GPIOC, 15};

void GPIO_Init(GPIO_t GPIOx)
{
    switch (GPIOx.Port)
    {
    case GPIOA:
        RCC_AHB1ENR |= (1 << GPIOx.Port);
        GPIOA_MODER &= ~(3U << (2U * GPIOx.Pin));
        GPIOA_MODER |= (1U << (2U * GPIOx.Pin));
        break;

    case GPIOB:
        RCC_AHB1ENR |= (1 << GPIOx.Port);
        GPIOB_MODER &= ~(3U << (2U * GPIOx.Pin));
        GPIOB_MODER |= (1U << (2U * GPIOx.Pin));
        break;

    case GPIOC:
        RCC_AHB1ENR |= (1 << GPIOx.Port);
        GPIOC_MODER &= ~(3U << (2U * GPIOx.Pin));
        GPIOC_MODER |= (1U << (2U * GPIOx.Pin));
        break;

    default:
    }
}

void LED_On_Off(volatile uint32_t *GPIOx_ODR, volatile uint32_t *GPIOx_BSRR, GPIO_t GPIOx)
{
    if (*GPIOx_ODR & (1 << GPIOx.Pin))
    {
        *GPIOx_BSRR = 1 << (GPIOx.Pin + 16);
    }
    else
    {
        *GPIOx_BSRR = 1 << GPIOx.Pin;
    }
}

void Toggle_LED(GPIO_t GPIOx)
{
    GPIO_Init(GPIOx);

    switch (GPIOx.Port)
    {
    case GPIOA:
        LED_On_Off(&GPIOA_ODR, &GPIOA_BSRR, GPIOx);
        break;

    case GPIOB:
        LED_On_Off(&GPIOB_ODR, &GPIOB_BSRR, GPIOx);
        break;

    case GPIOC:
        LED_On_Off(&GPIOC_ODR, &GPIOC_BSRR, GPIOx);
        break;

    default:
        break;
    }
}

#endif
