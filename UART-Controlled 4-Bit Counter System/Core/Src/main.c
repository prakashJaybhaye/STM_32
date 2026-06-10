#include "stm32f446xx_gpio.h"
#include "systick.h"
#include "usart2.h"

#define LED_MASK 0xF

typedef enum
{
    Counter_OFF = '0',
    Counter_ON = '1',
    Counter_Speed_Up = '2',
    Counter_Speed_Down = '3'

} CounterState_t;

typedef enum
{
    STATE_OFF,
    STATE_ON

} SystemState_t;

CounterState_t counterState_en = Counter_OFF;
SystemState_t systemState = STATE_OFF;

uint16_t counter = 0;
uint32_t sysTickCounter = 0;
uint32_t Ideal_time = 1000;

void FourBitCounter(void)
{
    if (SysTick_is_time_elapsed(&sysTickCounter, Ideal_time))
    {
        GPIOB_BSRR = (LED_MASK << 12) << 16;      // reset PB12-PB15
        GPIOB_BSRR = (counter & LED_MASK) << 12; // set value

        counter = (counter + 1) & 0x0F;
    }
}

int main(void)
{
    USART2_Init();
    SysTick_Timer_Init(1000);

    GPIO_t PB12 = {GPIOB_PORT, 12};
    GPIO_t PB13 = {GPIOB_PORT, 13};
    GPIO_t PB14 = {GPIOB_PORT, 14};
    GPIO_t PB15 = {GPIOB_PORT, 15};

    GPIO_Init(PB12);
    GPIO_Init(PB13);
    GPIO_Init(PB14);
    GPIO_Init(PB15);

    USART2_SendString("4-bit Counter Ready\r\n");

    while (1)
    {
        /* UART event handling */
        if (USART2_ReceiveChar(&counterState_en))
        {
            switch (counterState_en)
            {
                case Counter_OFF:
                    systemState = STATE_OFF;
                    GPIOB_BSRR = (LED_MASK << 12) << 16;
                    counter = 0;
                    sysTickCounter = 0;
                    break;

                case Counter_ON:
                    systemState = STATE_ON;
                    USART2_SendString("Counter ON\r\n");
                    break;

                case Counter_Speed_Up:
                    if (Ideal_time > 200)
                        Ideal_time -= 200;
                    USART2_SendString("Speed Up\r\n");
                    break;

                case Counter_Speed_Down:
                    Ideal_time += 200;
                    USART2_SendString("Speed Down\r\n");
                    break;

                default:
                    break;
            }
        }

        /* periodic task */
        if (systemState == STATE_ON)
        {
            FourBitCounter();
        }
    }
}