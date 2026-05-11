#include "stm32f446xx_gpio.h"

void GPIO_Init(GPIO_t gpio)
{
    switch (gpio.Port)
    {
        case GPIOA_PORT:

            RCC_AHB1ENR |= (1 << 0);

            GPIOA_MODER &= ~(3 << (gpio.Pin * 2));

            GPIOA_MODER |= (1 << (gpio.Pin * 2));

            break;

        case GPIOB_PORT:

            RCC_AHB1ENR |= (1 << 1);

            GPIOB_MODER &= ~(3 << (gpio.Pin * 2));

            GPIOB_MODER |= (1 << (gpio.Pin * 2));

            break;

        case GPIOC_PORT:

            RCC_AHB1ENR |= (1 << 2);

            GPIOC_MODER &= ~(3 << (gpio.Pin * 2));

            GPIOC_MODER |= (1 << (gpio.Pin * 2));

            break;

        default:
            break;
    }
}

void GPIO_Set(GPIO_t gpio)
{
    switch (gpio.Port)
    {
        case GPIOA_PORT:
            GPIOA_BSRR = (1 << gpio.Pin);
            break;

        case GPIOB_PORT:
            GPIOB_BSRR = (1 << gpio.Pin);
            break;

        case GPIOC_PORT:
            GPIOC_BSRR = (1 << gpio.Pin);
            break;

        default:
            break;
    }
}

void GPIO_Reset(GPIO_t gpio)
{
    switch (gpio.Port)
    {
        case GPIOA_PORT:
            GPIOA_BSRR = (1 << (gpio.Pin + 16));
            break;

        case GPIOB_PORT:
            GPIOB_BSRR = (1 << (gpio.Pin + 16));
            break;

        case GPIOC_PORT:
            GPIOC_BSRR = (1 << (gpio.Pin + 16));
            break;

        default:
            break;
    }
}

void GPIO_Toggle(GPIO_t gpio)
{
    switch (gpio.Port)
    {
        case GPIOA_PORT:

            if (GPIOA_ODR & (1 << gpio.Pin))
            {
                GPIO_Reset(gpio);
            }
            else
            {
                GPIO_Set(gpio);
            }

            break;

        case GPIOB_PORT:

            if (GPIOB_ODR & (1 << gpio.Pin))
            {
                GPIO_Reset(gpio);
            }
            else
            {
                GPIO_Set(gpio);
            }

            break;

        case GPIOC_PORT:

            if (GPIOC_ODR & (1 << gpio.Pin))
            {
                GPIO_Reset(gpio);
            }
            else
            {
                GPIO_Set(gpio);
            }

            break;

        default:
            break;
    }
}