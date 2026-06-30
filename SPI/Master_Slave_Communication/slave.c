#include <stdint.h>

/* ===================== RCC ===================== */

#define RCC_BASE        0x40023800U
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30U))
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x44U))

/* ===================== GPIOA ===================== */

#define GPIOA_BASE      0x40020000U

#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00U))
#define GPIOA_OSPEEDR   (*(volatile uint32_t *)(GPIOA_BASE + 0x08U))
#define GPIOA_PUPDR     (*(volatile uint32_t *)(GPIOA_BASE + 0x0CU))
#define GPIOA_AFRL      (*(volatile uint32_t *)(GPIOA_BASE + 0x20U))

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

/* ===================== SPI Bits ===================== */

#define SPI_CR1_CPHA    0U
#define SPI_CR1_CPOL    1U
#define SPI_CR1_MSTR    2U
#define SPI_CR1_SPE     6U
#define SPI_CR1_SSI     8U
#define SPI_CR1_SSM     9U

#define SPI_SR_RXNE     0U
#define SPI_SR_TXE      1U
#define SPI_SR_BSY      7U

void GPIOA_SPI1_Slave_Init(void)
{
    /* Enable GPIOA clock */
    RCC_AHB1ENR |= (1U << 0);

    /*
     * PA4 = NSS  AF5
     * PA5 = SCK  AF5
     * PA6 = MISO AF5
     * PA7 = MOSI AF5
     */

    /* PA4, PA5, PA6, PA7 alternate function mode */
    GPIOA_MODER &= ~(0xFFU << (SPI1_NSS * 2U));
    GPIOA_MODER |=  (0xAAU << (SPI1_NSS * 2U));

    /* High speed for PA4, PA5, PA6, PA7 */
    GPIOA_OSPEEDR &= ~(0xFFU << (SPI1_NSS * 2U));
    GPIOA_OSPEEDR |=  (0xFFU << (SPI1_NSS * 2U));

    /* No pull-up/pull-down */
    GPIOA_PUPDR &= ~(0xFFU << (SPI1_NSS * 2U));

    /* AF5 for PA4, PA5, PA6, PA7 */
    GPIOA_AFRL &= ~(0xFFFFU << (SPI1_NSS * 4U));
    GPIOA_AFRL |=  (0x5555U << (SPI1_NSS * 4U));
}

void SPI1_Slave_Init(void)
{
    /* Enable SPI1 clock */
    RCC_APB2ENR |= (1U << 12);

    /* Disable SPI before configuration */
    SPI1_CR1 = 0;
    SPI1_CR2 = 0;

    /*
     * Slave mode:
     * MSTR = 0
     *
     * SPI mode 0:
     * CPOL = 0
     * CPHA = 0
     *
     * Hardware NSS:
     * SSM = 0
     * NSS pin PA4 controls slave selection.
     */
    SPI1_CR1 &= ~(1U << SPI_CR1_MSTR);
    SPI1_CR1 &= ~(1U << SPI_CR1_SSM);

    /*
     * Load initial dummy value.
     * This is what master may receive during first frame.
     */
    *((volatile uint8_t *)&SPI1_DR) = 0x00;

    /* Enable SPI */
    SPI1_CR1 |= (1U << SPI_CR1_SPE);
}

uint8_t SPI1_Slave_ReceiveByte(void)
{
    while (!(SPI1_SR & (1U << SPI_SR_RXNE)))
        ;

    return *((volatile uint8_t *)&SPI1_DR);
}

void SPI1_Slave_LoadTxByte(uint8_t data)
{
    while (!(SPI1_SR & (1U << SPI_SR_TXE)))
        ;

    *((volatile uint8_t *)&SPI1_DR) = data;
}

int main(void)
{
    uint8_t received;

    GPIOA_SPI1_Slave_Init();
    SPI1_Slave_Init();

    while (1)
    {
        /*
         * Wait for master frame.
         * Frame 1 expected:
         * MOSI = 0xAA
         */
        received = SPI1_Slave_ReceiveByte();

        if (received == 0xAA)
        {
            /*
             * Prepare response for next frame.
             * Master must send dummy 0xFF to read this.
             */
            SPI1_Slave_LoadTxByte(0x55);
        }
        else
        {
            /*
             * Invalid command.
             */
            SPI1_Slave_LoadTxByte(0x00);
        }
    }
}
