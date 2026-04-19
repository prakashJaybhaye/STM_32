#include <stdint.h>

#define RCC_BASE        0x40023800UL
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x40))

#define GPIOA_BASE      0x40020000UL
#define GPIO_MODER      (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIO_AFRL       (*(volatile uint32_t *)(GPIOA_BASE + 0x20))

#define USART2_BASE     0x40004400UL
#define USART2_SR       (*(volatile uint32_t *)(USART2_BASE + 0x00))
#define USART2_DR       (*(volatile uint32_t *)(USART2_BASE + 0x04))
#define USART2_BRR      (*(volatile uint32_t *)(USART2_BASE + 0x08))
#define USART2_CR1      (*(volatile uint32_t *)(USART2_BASE + 0x0C))

#define GPIOA_EN        (RCC_AHB1ENR |= (1 << 0))
#define USART2_EN       (RCC_APB1ENR |= (1 << 17))

void delay(void)
{
    for (volatile uint32_t i = 0; i < 300000; i++);
}

void GPIOA_UART2_Init(void)
{
    GPIOA_EN;
    (void)RCC_AHB1ENR;

    GPIO_MODER &= ~(3 << (2 * 2));
    GPIO_MODER |=  (2 << (2 * 2));

    GPIO_AFRL &= ~(0xF << (4 * 2));
    GPIO_AFRL |=  (7 << (4 * 2));
}

void USART2_Init(void)
{
    USART2_EN;

    USART2_BRR = 0x0683;

    USART2_CR1 |= (1 << 13);
    USART2_CR1 |= (1 << 3);
}

void USART2_SendChar(char c)
{
    while (!(USART2_SR & (1 << 7)));
    USART2_DR = c;
}

int main(void)
{
    GPIOA_UART2_Init();
    USART2_Init();

    while (1)
    {
        USART2_SendChar('A');
        delay();
    }
}
