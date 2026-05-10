#include "stm32f446xx_gpio.h"
#include "systick.h"
#include "usart2.h"

int main(void)
{
    GPIO_t LED = {GPIOA_PORT, 5};

    GPIO_Init(LED);

    SysTick_Init(1000);

    USART2_Init();

    USART2_SendString("System Initialized\r\n");

    while (1)
    {
        GPIO_Toggle(LED);

        USART2_SendString("LED Toggled\r\n");

        SysTick_DelayMs(500);
    }
}