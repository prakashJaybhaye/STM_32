/*-------------------------------------------------------------------------------------------------
APPLICATION OVERVIEW
--------------------
Generate a PWM signal on PA0 using TIM2 Channel 1 to control LED brightness.
The LED brightness increases every 500 ms in steps of 20% duty cycle,
reaches 100%, then resets to 0% and repeats.
Delay timing is generated using the SysTick timer (1 ms tick).

SYSTEM CLOCK
------------
HSI = 16 MHz
AHB Prescaler = 1
APB1 Prescaler = 1
TIM2 clock = 16 MHz

---------------------------------------------------------------------------------------------------
GPIO CONFIGURATION (PA0 AS TIM2_CH1 OUTPUT)
---------------------------------------------------------------------------------------------------
1   Locate the RCC base address (0x40023800) from the STM32F411 memory map.
2   From the RCC base, locate the RCC_AHB1ENR register at offset 0x30.
3   Enable the GPIOA peripheral clock by setting bit 0 in RCC_AHB1ENR.

4   Locate the GPIOA base address (0x40020000) from the memory map.
5   From the GPIOA base, locate the GPIOA_MODER register at offset 0x00.
6   Clear the two MODER bits corresponding to pin PA0 (bits 1:0).
7   Set the MODER bits for PA0 to 10 to configure PA0 as Alternate Function mode.

8   From the GPIOA base, locate the GPIOA_AFRL register at offset 0x20.
9   Clear AFRL bits [3:0] corresponding to PA0.
10  Write AF1 (0001) into AFRL bits [3:0] to connect PA0 to TIM2_CH1.

---------------------------------------------------------------------------------------------------
SYSTICK CONFIGURATION (1 ms TIME BASE FOR DELAY)
---------------------------------------------------------------------------------------------------
11  Locate the SysTick control and status register (SYST_CSR).
12  Locate the SysTick reload value register (SYST_RVR).
13  Locate the SysTick current value register (SYST_CVR).

14  Calculate reload value:
        Reload = (16,000,000 / 1000) - 1 = 15999
    This produces a 1 ms SysTick interrupt period.

15  Load the calculated value into SYST_RVR.
16  Clear SYST_CVR to reset the counter.
17  Enable SysTick and select processor clock by setting:
        - ENABLE bit (bit 0)
        - CLKSOURCE bit (bit 2) in SYST_CSR.

---------------------------------------------------------------------------------------------------
TIM2 CONFIGURATION FOR PWM GENERATION
---------------------------------------------------------------------------------------------------
18  From the RCC base, locate the RCC_APB1ENR register at offset 0x40.
19  Enable the TIM2 peripheral clock by setting bit 0 in RCC_APB1ENR.

20  Locate the TIM2 base address (0x40000000) from the memory map.
21  From the TIM2 base, locate the TIM2_PSC register at offset 0x28.
22  Write the prescaler value 15 into TIM2_PSC.
        Timer clock = 16 MHz / (15 + 1) = 1 MHz (1 µs per timer tick).

23  Locate the TIM2_ARR register at offset 0x2C.
24  Write 1000 into TIM2_ARR.
        PWM period = 1000 ticks × 1 µs = 1 ms
        PWM frequency = 1 kHz.

25  Locate the TIM2_CCR1 register at offset 0x38.
26  Initialize TIM2_CCR1 to 0 to start with 0% duty cycle.

---------------------------------------------------------------------------------------------------
PWM MODE CONFIGURATION (CHANNEL 1)
---------------------------------------------------------------------------------------------------
27  Locate the TIM2_CCMR1 register at offset 0x18.
28  Clear bits [7:4] of TIM2_CCMR1 to reset OC1M and OC1PE configuration.
29  Set OC1M bits [6:4] to 110 to select PWM Mode 1.
        Output is HIGH when CNT < CCR1, LOW otherwise.
30  Set OC1PE (bit 3) to enable preload for CCR1.
        Duty-cycle updates occur safely on update events.

31  Locate the TIM2_CCER register at offset 0x20.
32  Set CC1E (bit 0) to enable TIM2 Channel 1 output on PA0.

---------------------------------------------------------------------------------------------------
TIMER STARTUP SEQUENCE
---------------------------------------------------------------------------------------------------
33  Locate the TIM2_CR1 register at offset 0x00.
34  Set ARPE (bit 7) to enable auto-reload preload for ARR.

35  Locate the TIM2_EGR register at offset 0x14.
36  Set UG (bit 0) to generate an update event.
        This transfers PSC, ARR, and CCR preload values into active registers.

37  Set CEN (bit 0) in TIM2_CR1 to start the timer.
        PWM signal is now generated on PA0.

---------------------------------------------------------------------------------------------------
MAIN LOOP OPERATION
---------------------------------------------------------------------------------------------------
38  Initialize duty_cycle variable to 0.
39  In the infinite loop:
        a) Calculate CCR1 value as:
               CCR1 = (ARR × duty_cycle) / 100
           This converts duty percentage into timer counts.
        b) Wait 500 ms using SysTick-based delay.
        c) Increase duty_cycle by 20%.
        d) If duty_cycle reaches or exceeds 100%, reset it to 0%.

40  This results in LED brightness increasing every 500 ms:
        0% → 20% → 40% → 60% → 80% → 100% → 0% → repeat.

---------------------------------------------------------------------------------------------------
END OF PROGRAM FLOW
-------------------------------------------------------------------------------------------------*/


#include <stdint.h>

/* ===================== SysTick ===================== */
#define SYST_CSR   (*(volatile uint32_t *)0xE000E010)
#define SYST_RVR   (*(volatile uint32_t *)0xE000E014)
#define SYST_CVR   (*(volatile uint32_t *)0xE000E018)

#define CPU_CLK    16000000UL
#define SYST_LOAD  (CPU_CLK / 1000 - 1)

/* ===================== RCC ===================== */
#define RCC_BASE       0x40023800UL
#define RCC_AHB1ENR    (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB1ENR    (*(volatile uint32_t *)(RCC_BASE + 0x40))

/* ===================== GPIOA ===================== */
#define GPIOA_BASE     0x40020000UL
#define GPIOA_MODER    (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_AFRL     (*(volatile uint32_t *)(GPIOA_BASE + 0x20))

/* ===================== TIM2 ===================== */
#define TIM2_BASE      0x40000000UL
#define TIM2_CR1       (*(volatile uint32_t *)(TIM2_BASE + 0x00))
#define TIM2_CCMR1     (*(volatile uint32_t *)(TIM2_BASE + 0x18))
#define TIM2_CCER      (*(volatile uint32_t *)(TIM2_BASE + 0x20))
#define TIM2_PSC       (*(volatile uint32_t *)(TIM2_BASE + 0x28))
#define TIM2_ARR       (*(volatile uint32_t *)(TIM2_BASE + 0x2C))
#define TIM2_CCR1      (*(volatile uint32_t *)(TIM2_BASE + 0x34))
#define TIM2_EGR       (*(volatile uint32_t *)(TIM2_BASE + 0x14))

/* ===================== Functions ===================== */

void SysTick_Init(void)
{
    SYST_RVR = SYST_LOAD;
    SYST_CVR = 0;
    SYST_CSR = (1 << 0) | (1 << 2);   // ENABLE | CPU CLK
}

void delay_ms(uint32_t ms)
{
    while (ms--)
    {
        while ((SYST_CSR & (1 << 16)) == 0);
    }
}

void GPIOA_Init(void)
{
    RCC_AHB1ENR |= (1 << 0);          // GPIOA clock

    GPIOA_MODER &= ~(3 << 0);
    GPIOA_MODER |=  (2 << 0);         // PA0 AF mode

    GPIOA_AFRL  &= ~(0xF << 0);
    GPIOA_AFRL  |=  (1 << 0);         // AF1 = TIM2_CH1
}

void TIM2_PWM_Init(void)
{
    RCC_APB1ENR |= (1 << 0);          // TIM2 clock

    TIM2_PSC = 15;                    // 16MHz / 16 = 1MHz
    TIM2_ARR = 1000;                  // 1kHz PWM
    TIM2_CCR1 = 0;

    TIM2_CCMR1 = 0;
    TIM2_CCMR1 |= (6 << 4);           // PWM mode 1
    TIM2_CCMR1 |= (1 << 3);           // preload enable

    TIM2_CCER  = 0;
    TIM2_CCER |= (1 << 0);            // enable CH1

    TIM2_CR1  |= (1 << 7);            // ARPE
    TIM2_EGR   = (1 << 0);            // update
    TIM2_CR1  |= (1 << 0);            // start timer
}

/* ===================== MAIN ===================== */

int main(void)
{
    uint8_t duty = 0;

    SysTick_Init();
    GPIOA_Init();
    TIM2_PWM_Init();

    while (1)
    {

        TIM2_CCR1 = (TIM2_ARR * duty) / 100;

        delay_ms(50);

        duty += 1;
        if (duty > 100)
            duty = 0;
    }
}
