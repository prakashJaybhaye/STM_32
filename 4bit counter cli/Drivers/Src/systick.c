/**
 * @file    systick.c
 * @brief   SysTick driver implementation
 */

#include "systick.h"

/* ============================================================
 * Global Tick Counter
 * ============================================================
 * Incremented every SysTick interrupt
 * Used as system time base (in ms)
 */
volatile uint32_t ticks = 0;


/* ============================================================
 * SysTick Initialization
 * ============================================================
 */
void SysTick_Timer_Init(uint32_t tick_hz)
{
    /* Calculate reload value for desired tick frequency */
    uint32_t load = (CPU_CLK / tick_hz) - 1;

    /* Load reload value */
    SYST_RVR = load;

    /* Reset current counter */
    SYST_CVR = 0;

    /*
     * CTRL Register Configuration:
     * Bit 0 (ENABLE)    = 1 → Enable SysTick
     * Bit 1 (TICKINT)   = 1 → Enable interrupt
     * Bit 2 (CLKSOURCE) = 1 → Processor clock (AHB)
     */
    SYST_CSR = (1U << 0) | (1U << 1) | (1U << 2);
}


/* ============================================================
 * SysTick Interrupt Handler
 * ============================================================
 * Called automatically every tick (e.g., 1ms)
 */
void SysTick_Handler(void)
{
    ticks++;  // increment system time
}


/* ============================================================
 * Get Current Time
 * ============================================================
 */
uint32_t millis(void)
{
    return ticks;
}


/* ============================================================
 * Non-blocking Delay Check
 * ============================================================
 */
uint8_t SysTick_is_time_elapsed(uint32_t *last_time, uint32_t delay)
{
    uint32_t current_time = millis();

    /*
     * Check if required delay has passed
     * Uses unsigned subtraction → safe for overflow
     */
    if (current_time - *last_time >= delay)
    {
        *last_time = current_time;  // update last execution time
        return 1;                  // time elapsed
    }

    return 0;                      // not yet
}


/* ============================================================
 * Blocking Delay
 * ============================================================
 */
void SysTick_Timer_DelayMs(uint32_t ms)
{
    uint32_t start_time = millis();

    /*
     * Busy wait until required time has passed
     * CPU is blocked during this loop
     */
    while ((millis() - start_time) < ms)
    {
        /* wait */
    }
}