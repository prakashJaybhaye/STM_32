#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

/**
 * @file    systick.h
 * @author  Prakash Jaybhaye
 * @brief   SysTick driver (register + API definition)
 * @note    Bare-metal STM32 Cortex-M4 SysTick implementation
 */

/* ============================================================
 * SysTick Register Definitions (Cortex-M4 Core Peripheral)
 * ============================================================
 */

/** Control and Status Register */
#define SYST_CSR (*(volatile uint32_t *)(0xE000E010UL))

/** Reload Value Register */
#define SYST_RVR (*(volatile uint32_t *)(0xE000E014UL))

/** Current Value Register */
#define SYST_CVR (*(volatile uint32_t *)(0xE000E018UL))


/* ============================================================
 * System Configuration
 * ============================================================
 */

/** System clock frequency (HSI = 16 MHz for STM32F4) */
#define CPU_CLK 16000000UL


/* ============================================================
 * Driver APIs
 * ============================================================
 */

/**
 * @brief  Initializes SysTick timer for periodic tick generation
 * @param  tick_hz: Tick frequency (e.g., 1000 = 1ms tick)
 * @retval None
 */
void SysTick_Timer_Init(uint32_t tick_hz);

/**
 * @brief  Blocking delay using SysTick COUNTFLAG
 * @param  ms: Delay in milliseconds
 * @retval None
 */
void SysTick_Timer_DelayMs(uint32_t ms);

#endif /* SYSTICK_H */
