#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

/* RCC */

#define RCC_BASE        0x40023800UL
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30U))

/* GPIO */

#define GPIOA_BASE      0x40020000UL
#define GPIOB_BASE      0x40020400UL
#define GPIOC_BASE      0x40020800UL

#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00U))
#define GPIOA_ODR       (*(volatile uint32_t *)(GPIOA_BASE + 0x14U))
#define GPIOA_BSRR      (*(volatile uint32_t *)(GPIOA_BASE + 0x18U))

#define GPIOB_MODER     (*(volatile uint32_t *)(GPIOB_BASE + 0x00U))
#define GPIOB_ODR       (*(volatile uint32_t *)(GPIOB_BASE + 0x14U))
#define GPIOB_BSRR      (*(volatile uint32_t *)(GPIOB_BASE + 0x18U))

#define GPIOC_MODER (*(volatile uint32_t *)(GPIOC_BASE + 0x00U))
#define GPIOC_ODR   (*(volatile uint32_t *)(GPIOC_BASE + 0x14U))
#define GPIOC_BSRR  (*(volatile uint32_t *)(GPIOC_BASE + 0x18U))

typedef enum
{
    GPIOA_PORT = 0,
    GPIOB_PORT,
    GPIOC_PORT

} GPIO_Port_t;

typedef struct
{
    GPIO_Port_t Port;
    uint8_t Pin;

} GPIO_t;

/* API */

void GPIO_Init(GPIO_t gpio);

void GPIO_Toggle(GPIO_t gpio);

void GPIO_Set(GPIO_t gpio);

void GPIO_Reset(GPIO_t gpio);

#endif