#ifndef STM32F446XX_H
#define STM32F446XX_H

#include <stdint.h>

/* ================= RCC ================= */

#define RCC_BASE        0x40023800UL

#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30U))
#define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x40U))

/* ================= GPIOA ================= */

#define GPIOA_BASE      0x40020000UL

#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00U))
#define GPIOA_PUPDR     (*(volatile uint32_t *)(GPIOA_BASE + 0x0CU))
#define GPIOA_ODR       (*(volatile uint32_t *)(GPIOA_BASE + 0x14U))
#define GPIOA_BSRR      (*(volatile uint32_t *)(GPIOA_BASE + 0x18U))
#define GPIOA_AFRL      (*(volatile uint32_t *)(GPIOA_BASE + 0x20U))
#define GPIOA_AFRH      (*(volatile uint32_t *)(GPIOA_BASE + 0x24U))

/* ================= GPIOB ================= */

#define GPIOB_BASE      0x40020400UL

#define GPIOB_MODER     (*(volatile uint32_t *)(GPIOB_BASE + 0x00U))
#define GPIOB_PUPDR     (*(volatile uint32_t *)(GPIOB_BASE + 0x0CU))
#define GPIOB_ODR       (*(volatile uint32_t *)(GPIOB_BASE + 0x14U))
#define GPIOB_BSRR      (*(volatile uint32_t *)(GPIOB_BASE + 0x18U))
#define GPIOB_AFRL      (*(volatile uint32_t *)(GPIOB_BASE + 0x20U))
#define GPIOB_AFRH      (*(volatile uint32_t *)(GPIOB_BASE + 0x24U))

/* ================= GPIOC ================= */

#define GPIOC_BASE      0x40020800UL

#define GPIOC_MODER     (*(volatile uint32_t *)(GPIOC_BASE + 0x00U))
#define GPIOC_PUPDR     (*(volatile uint32_t *)(GPIOC_BASE + 0x0CU))
#define GPIOC_ODR       (*(volatile uint32_t *)(GPIOC_BASE + 0x14U))
#define GPIOC_BSRR      (*(volatile uint32_t *)(GPIOC_BASE + 0x18U))
#define GPIOC_AFRL      (*(volatile uint32_t *)(GPIOC_BASE + 0x20U))
#define GPIOC_AFRH      (*(volatile uint32_t *)(GPIOC_BASE + 0x24U))

/* ================= USART2 ================= */

#define USART2_BASE     0x40004400UL

#define USART2_SR       (*(volatile uint32_t *)(USART2_BASE + 0x00U))
#define USART2_DR       (*(volatile uint32_t *)(USART2_BASE + 0x04U))
#define USART2_BRR      (*(volatile uint32_t *)(USART2_BASE + 0x08U))
#define USART2_CR1      (*(volatile uint32_t *)(USART2_BASE + 0x0CU))

#endif
