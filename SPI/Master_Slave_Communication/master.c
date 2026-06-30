#include <stdint.h>
#include "systick.h"

/* ===================== RCC ===================== */

#define RCC_BASE        0x40023800U
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30U))
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x44U))

/* ===================== GPIOA ===================== */

#define GPIOA_BASE      0x40020000U

#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00U))
#define GPIOA_OTYPER    (*(volatile uint32_t *)(GPIOA_BASE + 0x04U))
#define GPIOA_OSPEEDR   (*(volatile uint32_t *)(GPIOA_BASE + 0x08U))
#define GPIOA_PUPDR     (*(volatile uint32_t *)(GPIOA_BASE + 0x0CU))
#define GPIOA_AFRL      (*(volatile uint32_t *)(GPIOA_BASE + 0x20U))
#define GPIOA_BSRR      (*(volatile uint32_t *)(GPIOA_BASE + 0x18U))

/* ===================== SPI1 ===================== */

#define SPI1_BASE       0x40013000U

#define SPI1_CR1        (*(volatile uint32_t *)(SPI1_BASE + 0x00U))
#define SPI1_CR2        (*(volatile uint32_t *)(SPI1_BASE + 0x04U))
#define SPI1_SR         (*(volatile uint32_t *)(SPI1_BASE + 0x08U))
#define SPI1_DR         (*(volatile uint32_t *)(SPI1_BASE + 0x0CU))

/* ===================== Pins ===================== */

#define SPI1_NSS        4U
#define SPI1_SCK        5U
#define SPI1_MISO       6U
#define SPI1_MOSI       7U

#define CS_LOW()        (GPIOA_BSRR = (1U << (SPI1_NSS + 16U)))
#define CS_HIGH()       (GPIOA_BSRR = (1U << SPI1_NSS))

/* ===================== SPI Bits ===================== */

#define SPI_CR1_CPHA    0U
#define SPI_CR1_CPOL    1U
#define SPI_CR1_MSTR    2U
#define SPI_CR1_BR      3U
#define SPI_CR1_SPE     6U
#define SPI_CR1_SSI     8U
#define SPI_CR1_SSM     9U

#define SPI_SR_RXNE     0U
#define SPI_SR_TXE      1U
#define SPI_SR_BSY      7U

void GPIOA_SPI1_Master_Init(void)
{
    /* Enable GPIOA clock */
    RCC_AHB1ENR |= (1U << 0);

    /*
     * PA4 = NSS/CS GPIO output
     * PA5 = SCK AF5
     * PA6 = MISO AF5
     * PA7 = MOSI AF5
     */

    /* PA4 output mode */
    GPIOA_MODER &= ~(3U << (SPI1_NSS * 2U));
    GPIOA_MODER |=  (1U << (SPI1_NSS * 2U));

    /* PA5, PA6, PA7 alternate function mode */
    GPIOA_MODER &= ~(0x3FU << (SPI1_SCK * 2U));
    GPIOA_MODER |=  (0x2AU << (SPI1_SCK * 2U));

    /* Push-pull output */
    GPIOA_OTYPER &= ~(1U << SPI1_NSS);

    /* High speed for PA4, PA5, PA6, PA7 */
    GPIOA_OSPEEDR &= ~(0xFFU << (SPI1_NSS * 2U));
    GPIOA_OSPEEDR |=  (0xFFU << (SPI1_NSS * 2U));

    /* No pull-up/pull-down */
    GPIOA_PUPDR &= ~(0xFFU << (SPI1_NSS * 2U));

    /* AF5 for PA5, PA6, PA7 */
    GPIOA_AFRL &= ~(0xFFFU << (SPI1_SCK * 4U));
    GPIOA_AFRL |=  (0x555U << (SPI1_SCK * 4U));

    /* Keep slave deselected initially */
    CS_HIGH();
}

void SPI1_Master_Init(void)
{
    /* Enable SPI1 clock */
    RCC_APB2ENR |= (1U << 12);

    /* Disable SPI before configuration */
    SPI1_CR1 = 0;
    SPI1_CR2 = 0;

    /*
     * SPI mode 0:
     * CPOL = 0
     * CPHA = 0
     */

    SPI1_CR1 |= (1U << SPI_CR1_MSTR);      /* Master mode */

    /*
     * Baud rate = PCLK / 256
     * Slow speed is good for first test with breadboard/jumper wires.
     */
    SPI1_CR1 |= (7U << SPI_CR1_BR);

    /*
     * Software slave management on master side.
     * Actual NSS is controlled manually using PA4 GPIO.
     */
    SPI1_CR1 |= (1U << SPI_CR1_SSM);
    SPI1_CR1 |= (1U << SPI_CR1_SSI);

    /* Enable SPI */
    SPI1_CR1 |= (1U << SPI_CR1_SPE);
}

uint8_t SPI1_TransferByte(uint8_t data)
{
    uint8_t rx;

    /* Wait until transmit buffer empty */
    while (!(SPI1_SR & (1U << SPI_SR_TXE)))
        ;

    /* 8-bit write to DR */
    *((volatile uint8_t *)&SPI1_DR) = data;

    /* Wait until received byte available */
    while (!(SPI1_SR & (1U << SPI_SR_RXNE)))
        ;

    /* 8-bit read from DR */
    rx = *((volatile uint8_t *)&SPI1_DR);

    /* Wait until SPI not busy */
    while (SPI1_SR & (1U << SPI_SR_BSY))
        ;

    return rx;
}

int main(void)
{
    uint8_t rx1;
    uint8_t rx2;

    GPIOA_SPI1_Master_Init();
    SPI1_Master_Init();

    SysTick_Timer_Init(1000);

    while (1)
    {
        CS_LOW();

        /*
         * Frame 1:
         * MOSI = 0xAA
         * MISO = dummy/old data
         */
        rx1 = SPI1_TransferByte(0xAA);

        /*
         * Small delay gives slave time to process 0xAA
         * and load 0x55 into its DR.
         */
        SysTick_Timer_DelayMs(1);

        /*
         * Frame 2:
         * MOSI = 0xFF dummy
         * MISO = 0x55 expected response
         */
        rx2 = SPI1_TransferByte(0xFF);

        CS_HIGH();

        if (rx2 == 0x55)
        {
            /*
             * SPI communication successful.
             * Do not use PA5 LED because PA5 is SPI SCK.
             */
        }
        else
        {
            /*
             * SPI communication failed.
             */
        }

        SysTick_Timer_DelayMs(1000);

        (void)rx1;
    }
}
