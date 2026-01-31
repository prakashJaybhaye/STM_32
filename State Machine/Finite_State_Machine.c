//event-driven Moore finite state machine implemented in a super-loop architecture

#include <stdint.h>

#define RCC_BASE 0x40023800UL
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30)) // FOR GPIO
#define RCC_APB2ENR (*(volatile uint32_t *)(RCC_BASE + 0x44)) // FOR EXTI
#define RCC_APB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x40)) // FOR TM2

// GPIOA
#define GPIOA_BASE 0x40020000UL
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_BSRR (*(volatile uint32_t *)(GPIOA_BASE + 0x18))
#define GPIOA_ODR (*(volatile uint32_t *)(GPIOA_BASE + 0x14))
#define GPIOA_AFRL (*(volatile uint32_t *)(GPIOA_BASE + 0x20))

// SYStick
#define SYST_CSR (*(volatile uint32_t *)0xE000E010)
#define SYST_RVR (*(volatile uint32_t *)0xE000E014)
#define SYST_CVR (*(volatile uint32_t *)0xE000E018)

// TM2
#define TIM2_BASE 0x40000000UL
#define TIM2_CR1 (*(volatile uint32_t *)(TIM2_BASE + 0x00))
#define TIM2_CCMR1 (*(volatile uint32_t *)(TIM2_BASE + 0x18))
#define TIM2_CCER (*(volatile uint32_t *)(TIM2_BASE + 0x1C))
#define TIM2_PSC (*(volatile uint32_t *)(TIM2_BASE + 0x28))
#define TIM2_ARR (*(volatile uint32_t *)(TIM2_BASE + 0x2C))
#define TIM2_CCR1 (*(volatile uint32_t *)(TIM2_BASE + 0x34))
#define TIM2_EGR (*(volatile uint32_t *)(TIM2_BASE + 0x14))

// SYSCFG
#define SYSCFG_BASE 0x40013800
#define SYSCFG_EXTICR1 (*(volatile uint32_t *)(SYSCFG_BASE + 0x08))

#define EXTI_BASE 0x40013C00UL
#define EXTI_IMR (*(volatile uint32_t *)(EXTI_BASE + 0x00))
#define EXTI_RTSR (*(volatile uint32_t *)(EXTI_BASE + 0x08))
#define EXTI_PR (*(volatile uint32_t *)(EXTI_BASE + 0x14))

// NVIC
#define NVIC_ISER0 (*(volatile uint32_t *)(0xE000E100UL))

typedef enum LED_STATE
{
    LED_OFF = 0,
    LED_ON = 1,
    LED_TOGGLE = 2,
    LED_PWM = 3
} led_state_en;

led_state_en button_sate = LED_OFF;
uint8_t duty = 0;
uint8_t pwm_flag = 0;

#define LED_PIN_GPIOA0 0
#define PUSH_BUTTON_GPIOA1 1

#define CLK_FRQ 16000000UL // Using STM32F411 CPU Clock
#define LOAD_VAL (CLK_FRQ / 1000) - 1

void Sys_Timer_Init()
{
    SYST_RVR = LOAD_VAL;
    SYST_CVR = 0;
    SYST_CSR = 0;
    SYST_CSR = (1 << 0) | (1 << 2);
}

void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++)
    {
        while (((SYST_CSR >> 16) & 1) == 0)
            ;
    }
}

void GPIOA_Init(void)
{
    RCC_AHB1ENR |= 1 << 0;
    GPIOA_MODER &= ~(3 << (2 * LED_PIN_GPIOA0));
    GPIOA_MODER |= (1 << (2 * LED_PIN_GPIOA0));
    GPIOA_MODER &= ~(3 << (2 * PUSH_BUTTON_GPIOA1));
}

void GPIOA_Init_AF(void)
{
    RCC_AHB1ENR |= 1 << 0;
    GPIOA_MODER &= ~(3 << (2 * LED_PIN_GPIOA0));
    GPIOA_MODER |= (2 << (2 * LED_PIN_GPIOA0));

    GPIOA_AFRL &= ~(0xF << (4 * LED_PIN_GPIOA0));
    GPIOA_AFRL |= (1 << (4 * LED_PIN_GPIOA0));
}

void EXTI1_IRQHandler(void)
{
    button_sate++;
    EXTI_PR = (1 << PUSH_BUTTON_GPIOA1);

    if (button_sate > 3)
    {
        button_sate = 0;
    }
}

void TIM2_PWM_Init(void)
{
    RCC_APB1ENR |= (1 << 0); // TIM2 clock

    TIM2_PSC = 15;   // 16MHz / 16 = 1MHz
    TIM2_ARR = 1000; // 1kHz PWM
    TIM2_CCR1 = 0;

    TIM2_CCMR1 = 0;
    TIM2_CCMR1 |= (6 << 4); // PWM mode 1
    TIM2_CCMR1 |= (1 << 3); // preload enable

    TIM2_CCER = 0;
    TIM2_CCER |= (1 << 0); // enable CH1

    TIM2_CR1 |= (1 << 7); // ARPE
    TIM2_EGR = (1 << 0);  // update
    TIM2_CR1 |= (1 << 0); // start timer
}

int main(void)
{

    RCC_APB2ENR |= (1 << 14);
    SYSCFG_EXTICR1 &= ~(0xF << 8);

    EXTI_IMR |= 1 << PUSH_BUTTON_GPIOA1;
    EXTI_RTSR |= 1 << PUSH_BUTTON_GPIOA1;
    NVIC_ISER0 |= 1 << 7;

    RCC_AHB1ENR |= 1 << 0;

    Sys_Timer_Init();

    while (1)
    {
        if (pwm_flag && button_sate != LED_PWM)
        {
            GPIOA_Init();
            pwm_flag = 0;
            TIM2_CR1 &= ~(1 << 0);
        }

        switch (button_sate)
        {
        case LED_OFF:
            GPIOA_BSRR = 1 << (LED_PIN_GPIOA0 + 16);
            break;

        case LED_ON:
            GPIOA_BSRR = 1 << (LED_PIN_GPIOA0);
            break;

        case LED_TOGGLE:
            if (GPIOA_ODR & (1 << LED_PIN_GPIOA0))
            {
                GPIOA_BSRR = 1 << (LED_PIN_GPIOA0 + 16);
            }
            else
            {
                GPIOA_BSRR = 1 << LED_PIN_GPIOA0;
            }
            delay_ms(1000);
            break;

        case LED_PWM:

            if (!pwm_flag)
            {
                GPIOA_Init_AF();
                TIM2_PWM_Init();
                duty = 0;
                pwm_flag = 1;
            }
            TIM2_CCR1 = (TIM2_ARR * duty) / 100;
            delay_ms(50);
            duty += 1;
            if (duty > 100)
            {
                duty = 0;
            }
            break;
        default:
            break;
        }
    }
}
