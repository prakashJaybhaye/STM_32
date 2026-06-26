#include <stdint.h>
#include "systick.h"

#define RCC_BASE 0x40023800U

#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30U))
#define RCC_APB2ENR (*(volatile uint32_t *)(RCC_BASE + 0x44U))

#define GPIOA_BASE 0x40020000U

#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + 0x00U))
#define GPIOA_AFRL  (*(volatile uint32_t *)(GPIOA_BASE + 0x20U))

#define SPI1_BASE 0x40013000U

#define SPI1_CR1 (*(volatile uint32_t *)(SPI1_BASE + 0x00U)) // SPI configuration register
#define SPI1_CR2 (*(volatile uint32_t *)(SPI1_BASE + 0x04U)) // Advanced SPI features
#define SPI1_SR  (*(volatile uint32_t *)(SPI1_BASE + 0x08U)) // SPI status register
#define SPI1_DR  (*(volatile uint32_t *)(SPI1_BASE + 0x0CU)) // SPI data register

#define SPI1_SCK  5
#define SPI1_MISO 6
#define SPI1_MOSI 7

void GPIOA_Init(void)
{
    RCC_AHB1ENR |= (1U << 0); // Enable GPIOA clock

    /* PA5, PA6, PA7 -> Alternate Function Mode */
    GPIOA_MODER &= ~(0x3FU << (SPI1_SCK * 2));
    GPIOA_MODER |=  (0x2AU << (SPI1_SCK * 2));

    /* PA5, PA6, PA7 -> AF5 (SPI1) */
    GPIOA_AFRL &= ~(0xFFFU << (SPI1_SCK * 4));
    GPIOA_AFRL |=  (0x555U << (SPI1_SCK * 4));
}

void SPI1_Init_Master(void)
{
    RCC_APB2ENR |= (1U << 12); // Enable SPI1 clock

    SPI1_CR1 = 0;              // Start from reset state

    SPI1_CR1 |= (1U << 2);     // MSTR = Master mode
    SPI1_CR1 |= (7U << 3);     // BR = PCLK / 256
    SPI1_CR1 |= (1U << 9);     // SSM = Software slave management
    SPI1_CR1 |= (1U << 8);     // SSI = Internal NSS high

    /* CPOL = 0, CPHA = 0 (SPI Mode 0) */

    SPI1_CR1 |= (1U << 6);     // SPE = Enable SPI
}

void SPI1_SendByte(uint8_t data)
{
    while (!(SPI1_SR & (1U << 1)))
        ;                       // Wait until TXE = 1

    SPI1_DR = data;             // Load data into transmit buffer

    while (SPI1_SR & (1U << 7))
        ;                       // Wait until BSY = 0
}

int main(void)
{
    GPIOA_Init();
    SPI1_Init_Master();

    SysTick_Timer_Init(1000);

    while (1)
    {
        SPI1_SendByte(0x32);    // Send 0x32 over MOSI

        SysTick_Timer_DelayMs(1000);
    }
}