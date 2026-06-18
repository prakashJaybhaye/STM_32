#ifndef STM32F446XX_GPIO_REG_H
#define STM32F446XX_GPIO_REG_H

#include <stdint.h>
#include "stm32f446xx_types.h"

#define GPIOA_BASE_ADDR 0x40020000UL
#define GPIOB_BASE_ADDR 0x40020400UL
#define GPIOC_BASE_ADDR 0x40020800UL
#define GPIOD_BASE_ADDR 0x40020C00UL
#define GPIOE_BASE_ADDR 0x40021000UL
#define GPIOF_BASE_ADDR 0x40021400UL
#define GPIOG_BASE_ADDR 0x40021800UL
#define GPIOH_BASE_ADDR 0x40021C00UL

typedef struct
{
    vu32 MODER;   /* GPIOx_BASE + 0x00 = 0x40020000 */
    vu32 OTYPER;  /* GPIOx_BASE + 0x04 = 0x40020004 */
    vu32 OSPEEDR; /* GPIOx_BASE + 0x08 = 0x40020008 */
    vu32 PUPDR;   /* GPIOx_BASE + 0x0C = 0x4002000C */

    vu32 IDR;  /* GPIOx_BASE + 0x10 = 0x40020010 */
    vu32 ODR;  /* GPIOx_BASE + 0x14 = 0x40020014 */
    vu32 BSRR; /* GPIOx_BASE + 0x18 = 0x40020018 */
    vu32 LCKR; /* GPIOx_BASE + 0x1C = 0x4002001C */

    vu32 AFR[2]; /* GPIOx_BASE + 0x20 = 0x40020020 (AFRL)
                    GPIOx_BASE + 0x24 = 0x40020024 (AFRH) */
} GPIO_RegDef_t;


/*
 * GPIO Peripheral Base Address Mapping
 *
 * Each GPIO port (A–H) is memory mapped at fixed base addresses.
 * These macros cast the base address into a pointer of type GPIO_RegDef_t,
 * allowing direct register access using structure members.
 *
 * Address calculation example:
 *
 * GPIOA->ODR
 * = Base Address (0x40020000)
 *   + Offset of ODR (0x14)
 * = 0x40020014
 *
 * So:
 * GPIOA->ODR directly accesses memory-mapped register ODR of GPIOA
 *
 * Same applies for all GPIO ports:
 * GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH
 */


#define GPIOA ((GPIO_RegDef_t *)GPIOA_BASE_ADDR)
#define GPIOB ((GPIO_RegDef_t *)GPIOB_BASE_ADDR)
#define GPIOC ((GPIO_RegDef_t *)GPIOC_BASE_ADDR)
#define GPIOD ((GPIO_RegDef_t *)GPIOD_BASE_ADDR)
#define GPIOE ((GPIO_RegDef_t *)GPIOE_BASE_ADDR)
#define GPIOF ((GPIO_RegDef_t *)GPIOF_BASE_ADDR)
#define GPIOG ((GPIO_RegDef_t *)GPIOG_BASE_ADDR)
#define GPIOH ((GPIO_RegDef_t *)GPIOH_BASE_ADDR)

#endif // STM32F446XX_GPIO_REG_H
