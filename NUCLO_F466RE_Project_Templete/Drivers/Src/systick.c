/**
 * @file    systick.c
 * @author  Prakash Jaybhaye
 * @brief   SysTick driver implementation
 */

#include "systick.h"

/**
 * @brief  Initializes SysTick timer
 */
void SysTick_Timer_Init(uint32_t tick_hz)
{
    /* Compute reload value for required tick frequency */
    uint32_t load = (CPU_CLK / tick_hz) - 1;

    /* Load value into reload register */
    SYST_RVR = load;

    /* Clear current counter */
    SYST_CVR = 0;

    /*
     * CTRL register configuration:
     * Bit 0 -> ENABLE    : Start SysTick
     * Bit 2 -> CLKSOURCE : Processor clock
     * Bit 1 -> TICKINT   : Disabled (polling mode)
     */
    SYST_CSR = (1U << 0) | (1U << 2);
}

/**
 * @brief  Blocking delay using SysTick COUNTFLAG
 */
void SysTick_Timer_DelayMs(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++)
    {
        /*
         * COUNTFLAG (bit 16):
         * - Set when timer reaches 0
         * - Cleared on read
         */
        while ((SYST_CSR & (1U << 16)) == 0);
    }
}
