#include <stdint.h>

// --- Register Definitions ---
#define RCC_BASE      0x40023800UL
#define RCC_AHB1ENR   (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB2ENR   (*(volatile uint32_t *)(RCC_BASE + 0x44))
#define RCC_APB1ENR   (*(volatile uint32_t *)(RCC_BASE + 0x40))

#define GPIOA_BASE    0x40020000UL
#define GPIOA_MODER   (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_PUPDR   (*(volatile uint32_t *)(GPIOA_BASE + 0x0C))
#define GPIOA_ODR     (*(volatile uint32_t *)(GPIOA_BASE + 0x14))
#define GPIOA_BSRR    (*(volatile uint32_t *)(GPIOA_BASE + 0x18))
#define GPIOA_AFRL    (*(volatile uint32_t *)(GPIOA_BASE + 0x20))

#define SYST_CSR      (*(volatile uint32_t *)0xE000E010)
#define SYST_RVR      (*(volatile uint32_t *)0xE000E014)
#define SYST_CVR      (*(volatile uint32_t *)0xE000E018)

#define TIM2_BASE     0x40000000UL
#define TIM2_CR1      (*(volatile uint32_t *)(TIM2_BASE + 0x00))
#define TIM2_CCMR1    (*(volatile uint32_t *)(TIM2_BASE + 0x18))
#define TIM2_CCER     (*(volatile uint32_t *)(TIM2_BASE + 0x1C))
#define TIM2_PSC      (*(volatile uint32_t *)(TIM2_BASE + 0x28))
#define TIM2_ARR      (*(volatile uint32_t *)(TIM2_BASE + 0x2C))
#define TIM2_CCR1     (*(volatile uint32_t *)(TIM2_BASE + 0x34))
#define TIM2_EGR      (*(volatile uint32_t *)(TIM2_BASE + 0x14))

#define SYSCFG_BASE   0x40013800UL
#define SYSCFG_EXTICR1 (*(volatile uint32_t *)(SYSCFG_BASE + 0x08))

#define EXTI_BASE     0x40013C00UL
#define EXTI_IMR      (*(volatile uint32_t *)(EXTI_BASE + 0x00))
#define EXTI_RTSR     (*(volatile uint32_t *)(EXTI_BASE + 0x08))
#define EXTI_PR       (*(volatile uint32_t *)(EXTI_BASE + 0x14))

#define NVIC_ISER0    (*(volatile uint32_t *)(0xE000E100UL))

// --- FSM States ---
typedef enum {
    LED_OFF = 0,
    LED_ON = 1,
    LED_TOGGLE = 2,
    LED_PWM = 3
} led_state_en;

// Global variables MUST be volatile because they change in ISR
volatile led_state_en button_state = LED_OFF;
volatile uint8_t pwm_flag = 0;
uint8_t duty = 0;

#define LED_PIN 0
#define BTN_PIN 1

// --- Helper Functions ---

void Sys_Timer_Init(void) {
    SYST_RVR = (16000000UL / 1000) - 1; // 1ms tick
    SYST_CVR = 0;
    SYST_CSR = (1 << 0) | (1 << 2);     // Enable, Internal Clock
}

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) {
        while (!(SYST_CSR & (1 << 16))); // Wait for CountFlag
    }
}

void GPIOA_Init(void) {
    RCC_AHB1ENR |= (1 << 0);
    // LED PA0 as Output
    GPIOA_MODER &= ~(3 << (2 * LED_PIN));
    GPIOA_MODER |= (1 << (2 * LED_PIN));
    // Button PA1 as Input with Pull-down
    GPIOA_MODER &= ~(3 << (2 * BTN_PIN));
    GPIOA_PUPDR &= ~(3 << (2 * BTN_PIN));
    GPIOA_PUPDR |= (2 << (2 * BTN_PIN)); // Pull-down (assuming button to VCC)
}

void GPIOA_Init_AF(void) {
    // LED PA0 as Alternate Function (TIM2_CH1)
    GPIOA_MODER &= ~(3 << (2 * LED_PIN));
    GPIOA_MODER |= (2 << (2 * LED_PIN));
    GPIOA_AFRL &= ~(0xF << (4 * LED_PIN));
    GPIOA_AFRL |= (1 << (4 * LED_PIN)); // AF1 = TIM2
}

void TIM2_PWM_Init(void) {
    RCC_APB1ENR |= (1 << 0);
    TIM2_PSC = 15;        // 16MHz / 16 = 1MHz
    TIM2_ARR = 1000;      // 1kHz Frequency
    TIM2_CCR1 = 0;
    TIM2_CCMR1 = (6 << 4) | (1 << 3); // PWM Mode 1 + Preload
    TIM2_CCER |= (1 << 0);            // Enable CH1
    TIM2_CR1 |= (1 << 7) | (1 << 0);  // ARPE + Start
    TIM2_EGR |= (1 << 0);             // Update
}

// --- Interrupt Handler ---
void EXTI1_IRQHandler(void) {
    if (EXTI_PR & (1 << BTN_PIN)) {
        button_state++;
        if (button_state > 3) button_state = 0;
        EXTI_PR = (1 << BTN_PIN); // Clear interrupt flag
    }
}

// --- Main Program ---
int main(void) {
    // Enable Clocks
    RCC_APB2ENR |= (1 << 14); // SYSCFG Clock
    GPIOA_Init();             // Initialize GPIO early
    Sys_Timer_Init();

    // Configure External Interrupt for PA1
    SYSCFG_EXTICR1 &= ~(0xF << 4); // Map EXTI1 to Port A (Bits 4-7)
    EXTI_IMR  |= (1 << BTN_PIN);
    EXTI_RTSR |= (1 << BTN_PIN);
    NVIC_ISER0 |= (1 << 7);        // Enable EXTI1 IRQ

    while (1) {
        // Cleanup: If we left PWM state, reset GPIO to standard output
        if (pwm_flag && button_state != LED_PWM) {
            GPIOA_Init();
            TIM2_CR1 &= ~(1 << 0); // Stop Timer
            pwm_flag = 0;
        }

        switch (button_state) {
            case LED_OFF:
                GPIOA_BSRR = (1 << (LED_PIN + 16));
                break;

            case LED_ON:
                GPIOA_BSRR = (1 << LED_PIN);
                break;

            case LED_TOGGLE:
                GPIOA_ODR ^= (1 << LED_PIN); // Simplified toggle
                delay_ms(500);
                break;

            case LED_PWM:
                if (!pwm_flag) {
                    GPIOA_Init_AF();
                    TIM2_PWM_Init();
                    pwm_flag = 1;
                    duty = 0;
                }
                TIM2_CCR1 = (TIM2_ARR * duty) / 100;
                delay_ms(20);
                duty = (duty + 1) % 101;
                break;
        }
    }
}
