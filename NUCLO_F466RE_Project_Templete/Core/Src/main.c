#include "usart2.h"
#include "systick.h"

int main(void)
{
    USART2_Init();          // UART first
    SysTick_Timer_Init(1000); // 1ms tick

    while (1)
    {
        USART2_SendString("This Code is flased thrugh vs code\r\n");
        SysTick_Timer_DelayMs(500); // 1000 ms delay
    }
}
