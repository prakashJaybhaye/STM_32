#include "stm32f446xx_gpio_driver.h"
#include "systick.h"
#include "usart2.h"

int main(void)
{
    GPIO_Handler_t LED = {
        .pGPIOx = GPIOA,
        .GPIO_PinConfig.GPIO_PinAltFunMode = 0,
        .GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT,
        .GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_5,
        .GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP,
        .GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD,
        .GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM,
    };


    GPIO_Init(&LED);

    SysTick_Timer_Init(1000);

    USART2_Init();

    USART2_SendString("System Initialized\r\n");

    while (1)
    {
        GPIO_TogglePin(LED.pGPIOx, LED.GPIO_PinConfig.GPIO_PinNumber);

        USART2_SendString("LED Toggled\r\n");

        SysTick_Timer_DelayMs(500);
    }
}
















