#include "usart2.h"
#include "stm32f446xx.h"

/* GPIOA configuration for USART2 */
static void GPIOA_UART2_Init(void)
{
    /* Enable GPIOA clock */
    RCC_AHB1ENR |= (1 << 0);

    /* PA2 -> Alternate Function Mode */
    GPIOA_MODER &= ~(3 << (2 * 2));
    GPIOA_MODER |=  (2 << (2 * 2));

    /* PA3 -> Alternate Function Mode */
    GPIOA_MODER &= ~(3 << (2 * 3));
    GPIOA_MODER |=  (2 << (2 * 3));

    /* Select AF7 for USART2 */

    GPIOA_AFRL &= ~(0xF << (4 * 2));
    GPIOA_AFRL |=  (7 << (4 * 2));

    GPIOA_AFRL &= ~(0xF << (4 * 3));
    GPIOA_AFRL |=  (7 << (4 * 3));

    /* Pull-up for RX pin */

    GPIOA_PUPDR &= ~(3 << (2 * 3));
    GPIOA_PUPDR |=  (1 << (2 * 3));
}

/* USART2 initialization */
void USART2_Init(void)
{
    /* Enable USART2 clock */
    RCC_APB1ENR |= (1 << 17);

    /* Configure GPIO */
    GPIOA_UART2_Init();

    /* Baudrate 9600 @16MHz */
    USART2_BRR = 0x0683;

    /* Enable TX */
    USART2_CR1 |= (1 << 3);

    /* Enable RX */
    USART2_CR1 |= (1 << 2);

    /* Enable USART */
    USART2_CR1 |= (1 << 13);
}

/* Send single character */
void USART2_SendChar(char c)
{
    while (!(USART2_SR & (1 << 7)));

    USART2_DR = c;
}

/* Send string */
void USART2_SendString(const char *str)
{
    while (*str != '\0')
    {
        USART2_SendChar(*str++);
    }
}

/* Receive single character */
uint8_t USART2_ReceiveChar(unsigned char *c)
{
    if (USART2_SR & (1 << 5))
    {
        *c = USART2_DR & 0xFF;

        return 1;
    }

    return 0;
}

/* Receive string (non-blocking) */
uint8_t USART2_ReceiveString(char *buffer, uint32_t max_length)
{
    static uint32_t i = 0;

    unsigned char c;

    if (USART2_ReceiveChar(&c))
    {
        USART2_SendChar(c);

        if (c == '\r' || c == '\n')
        {
            if (i > 0)
            {
                buffer[i] = '\0';

                i = 0;

                return 1;
            }

            return 0;
        }

        if (i < (max_length - 1))
        {
            buffer[i++] = c;
        }
        else
        {
            i = 0;
        }
    }

    return 0;
}