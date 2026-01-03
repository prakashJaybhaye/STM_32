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

typedef enum GPIOA_RCC_AHB1_ENABLE
{
    GPIOA_EN,
    GPIOB_EN,
    GPIOC_EN,

} GPIOA_MODER_ENABLE;

typedef enum GPIOA_PORT
{
    PA0,
    PA1,
    PA2,
    PA3,
    PA4,
    PA5,
    PA6,
    PA7,
} GPIOA_PORT;

typedef enum GPIOB_PORT
{
    PB0 = 0,
    PB1 = 1,
    PB2 = 2,
    PB10 = 10,
} GPIOB_PORT;

typedef enum GPIOC_PORT
{
    PC13 = 13,
    PC14 = 14,
    PC15 = 15,
} GPIOC_PORT;

void Enable_GPIO_PORT(GPIOA_RCC_AHB1_ENABLE Pin)
{
    RCC_AHB1ENT |= (1 << Pin);
}

void Set_GPIOA_MODER_Register(GPIOA_PORT Pin)
{
    GPIOA_MODER &= ~(3 << (2 * Pin));
    GPIOA_MODER |= (1 << (2 * Pin));
}

void Set_GPIOB_MODER_Register(GPIOB_PORT Pin)
{
    GPIOB_MODER &= ~(3 << (2 * Pin));
    GPIOB_MODER |= (1 << (2 * Pin));
}

void Set_GPIOC_MODER_Register(GPIOC_PORT Pin)
{
    GPIOC_MODER &= ~(3 << (2 * Pin));
    GPIOC_MODER |= (1 << (2 * Pin));
}



void Toggle_LED_with_Port(GPIO_PORTS Port, GPIOA_PORT PinA, GPIOB_PORT PinB, GPIOC_PORT PinC)
{
    switch (Port)
    {
    case GPIOA:
        Enable_GPIO_PORT(GPIOA_EN);
        Set_GPIOA_MODER_Register(PinA);
        if (GPIOA_ODR & (1 << PinA))
        {
            GPIOA_BSRR = (1 << (PinA + 16));
        }
        else
        {
            GPIOA_BSRR = (1 << PinA);
        }
        break;

    case GPIOB:
        Enable_GPIO_PORT(GPIOB_EN);
        Set_GPIOB_MODER_Register(PinB);
        if (GPIOB_ODR & (1 << PinB))
        {
            GPIOB_BSRR = (1 << (PinB + 16));
        }
        else
        {
            GPIOB_BSRR = (1 << PinB);
        }
        break;

    case GPIOC:
        Enable_GPIO_PORT(GPIOC_EN);
        Set_GPIOC_MODER_Register(PinC);
        if (GPIOC_ODR & (1 << PinC))
        {
            GPIOC_BSRR = (1 << (PinC + 16));
        }
        else
        {
            GPIOC_BSRR = (1 << PinC);
        }
        break;

    default:
        break;
    }
}

#endif
