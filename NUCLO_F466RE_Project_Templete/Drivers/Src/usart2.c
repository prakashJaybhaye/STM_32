#include "usart2.h"

/* Initialize GPIOA for USART2 (PA2 TX, PA3 RX) */
static void GPIOA_UART2_Init(void)
{
    /* Enable GPIOA clock */
    RCC_AHB1ENR |= (1 << 0);

    /* PA2 TX -> Alternate Function */
    GPIO_MODER &= ~(3 << (2 * 2));
    GPIO_MODER |=  (2 << (2 * 2));

    /* PA3 RX -> Alternate Function */
    GPIO_MODER &= ~(3 << (2 * 3));
    GPIO_MODER |=  (2 << (2 * 3));

    /* Select AF7 (USART2 function) */
    GPIO_AFRL &= ~(0xF << (4 * 2));
    GPIO_AFRL |=  (7 << (4 * 2));

    GPIO_AFRL &= ~(0xF << (4 * 3));
    GPIO_AFRL |=  (7 << (4 * 3));

    /* Pull-up for RX stability */
    GPIO_PUPDR &= ~(3 << (2 * 3));
    GPIO_PUPDR |=  (1 << (2 * 3));
}

/* Initialize USART2 peripheral */
void USART2_Init(void)
{
    /* Enable USART2 clock */
    RCC_APB1ENR |= (1 << 17);

    /* Configure GPIO pins */
    GPIOA_UART2_Init();

    /* Set baud rate (9600 @ 16MHz) */
    USART2_BRR = 0x0683;

    /* Enable USART, TX, RX */
    USART2_CR1 |= (1 << 13); /* UE */
    USART2_CR1 |= (1 << 3);  /* TE */
    USART2_CR1 |= (1 << 2);  /* RE */
}

/* Send one character */
void USART2_SendChar(char c)
{
    while (!(USART2_SR & (1 << 7))); /* wait TX empty */
    USART2_DR = c;
}

/* Send string */
void USART2_SendString(char *str)
{
    while (*str)
    {
        USART2_SendChar(*str++);
    }
}

/* Receive one character */
char USART2_ReceiveChar(void)
{
    while (!(USART2_SR & (1 << 5))); /* wait RX data */
    return (char)USART2_DR;
}
