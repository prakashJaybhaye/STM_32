// led Blinling profram for B2 pin of STM 32

#ifndef LED_DRIVER_STM32_H
#define LED_DRIVER_STM32_H

#include <stdint.h>

/*-------------------------------RCC ENABLE & AHB1 ENABL------------------------------------------------*/
#define RCC_BASE 0x40023800UL
#define RCC_AHB1ENT (*(volatile uint32_t *)(RCC_BASE + 0x30))

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
    GPIOA,
    GPIOB,
    GPIOC,
} GPIO_PORTS;

typedef enum GPIO_PINS
{
    PA0 = 0,
    PA1 = 1,
    PA2 = 2,
    PA3 = 3,
    PA4 = 4,
    PA5 = 5,
    PA6 = 6,
    PA7 = 7,

    PB0 = 0,
    PB1 = 1,
    PB2 = 2,
    PB10 = 10,

    PC13 = 13,
    PC14 = 14,
    PC15 = 15
} GPIO_PINS;

void GPIO_Init(GPIO_PORTS Port, GPIO_PINS Pin)
{
    switch (Port)
    {
    case GPIOA:
        RCC_AHB1ENT |= 1 << GPIOA;
        GPIOA_MODER &= ~(3 << (2 * Pin));
        GPIOA_MODER |= (1 << (Pin * 2));
        break;

    case GPIOB:
        RCC_AHB1ENT |= 1 << GPIOB;
        GPIOB_MODER &= ~(3 << (2 * Pin));
        GPIOB_MODER |= (1 << (Pin * 2));
        break;

    case GPIOC:
        RCC_AHB1ENT |= 1 << GPIOC;
        GPIOC_MODER &= ~(3 << (2 * Pin));
        GPIOC_MODER |= (1 << (Pin * 2));
        break;

    default:
        break;
    }
}

void LED_On_Off(volatile uint32_t *GPIOx_ODR, volatile uint32_t *GPIOx_BSRR, GPIO_PINS Pin)
{
    if (*GPIOx_ODR & (1 << Pin))
    {
        *GPIOx_BSRR = 1 << (Pin + 16);
    }
    else
    {
        *GPIOx_BSRR = 1 << Pin;
    }
}

void Toggle_LED(GPIO_PORTS Port, GPIO_PINS Pin)
{
    GPIO_Init(Port, Pin);

    switch (Port)
    {
    case GPIOA:
        LED_On_Off(&GPIOA_ODR, &GPIOA_BSRR, Pin);
        break;

    case GPIOB:
        LED_On_Off(&GPIOB_ODR, &GPIOB_BSRR, Pin);
        break;

    case GPIOC:
        LED_On_Off(&GPIOC_ODR, &GPIOC_BSRR, Pin);
        break;

    default:
        break;
    }
}

#endif
