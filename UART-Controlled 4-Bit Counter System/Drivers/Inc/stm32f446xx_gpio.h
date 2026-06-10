#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "stm32f446xx.h"

typedef enum
{
    GPIOA_PORT = 0,
    GPIOB_PORT,
    GPIOC_PORT

} GPIO_Port_t;

typedef struct
{
    GPIO_Port_t Port;
    uint8_t Pin;

} GPIO_t;

/* API */

void GPIO_Init(GPIO_t gpio);

void GPIO_Toggle(GPIO_t gpio);

void GPIO_Set(GPIO_t gpio);

void GPIO_Reset(GPIO_t gpio);

#endif