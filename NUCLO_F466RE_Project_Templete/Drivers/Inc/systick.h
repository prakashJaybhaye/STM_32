#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

/**
 * @file    systick.h
 * @brief   SysTick driver (blocking + non-blocking support)
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
 * @brief  Initializes SysTick timer
 * @param  tick_hz: Desired tick frequency (e.g., 1000 = 1ms tick)
 */
void SysTick_Timer_Init(uint32_t tick_hz);

/**
 * @brief  Returns system uptime in milliseconds
 * @note   Depends on SysTick interrupt
 */
uint32_t millis(void);

/**
 * @brief  Blocking delay using millis()
 * @param  ms: Delay in milliseconds
 */
void SysTick_Timer_DelayMs(uint32_t ms);

/**
 * @brief  Non-blocking delay checker
 * @param  last: Pointer to last execution time
 * @param  delay: Required delay in ms
 * @retval 1 if delay elapsed, else 0
 */
uint8_t SysTick_is_time_elapsed(uint32_t *last, uint32_t delay);

#endif /* SYSTICK_H */
