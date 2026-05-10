#ifndef USART2_H
#define USART2_H

#include <stdint.h>

/**
 * @file    usart2.h
 * @author  Prakash Jaybhaye
 * @brief   USART2 driver (bare-metal STM32F4)
 */

/* ========================= RCC REGISTERS ========================= */
#define RCC_BASE        0x40023800UL
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30))  /* GPIO clock enable */
#define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x40))  /* USART clock enable */

/* ========================= GPIOA REGISTERS ========================= */
#define GPIOA_BASE      0x40020000UL
#define GPIO_MODER      (*(volatile uint32_t *)(GPIOA_BASE + 0x00)) /* mode select */
#define GPIO_AFRL       (*(volatile uint32_t *)(GPIOA_BASE + 0x20)) /* alternate function */
#define GPIO_PUPDR      (*(volatile uint32_t *)(GPIOA_BASE + 0x0C)) /* pull-up/down */

/* ========================= USART2 REGISTERS ========================= */
#define USART2_BASE     0x40004400UL
#define USART2_SR       (*(volatile uint32_t *)(USART2_BASE + 0x00)) /* status */
#define USART2_DR       (*(volatile uint32_t *)(USART2_BASE + 0x04)) /* data */
#define USART2_BRR      (*(volatile uint32_t *)(USART2_BASE + 0x08)) /* baud rate */
#define USART2_CR1      (*(volatile uint32_t *)(USART2_BASE + 0x0C)) /* control */

/* ========================= API ========================= */
void USART2_Init(void);
void USART2_SendChar(char c);
void USART2_SendString(const char *str);
uint8_t USART2_ReceiveChar(unsigned char *c);
uint8_t USART2_ReceiveString(char *buffer, uint32_t max_length);

#endif
