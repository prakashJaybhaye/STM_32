#ifndef STM32F446XX_TYPES_H
#define STM32F446XX_TYPES_H

#include <stdint.h>

/* =========================================================
 * Standard Type Definitions
 * =========================================================
 * These typedefs provide fixed-width integer types with
 * shorter and more readable names for embedded development.
 * ========================================================= */

/* ================= Normal Types ================= */

/* 32-bit unsigned integer */
typedef uint32_t u32;

/* 16-bit unsigned integer */
typedef uint16_t u16;

/* 8-bit unsigned integer */
typedef uint8_t  u8;


/* =========================================================
 * Volatile Types (Register Access)
 * =========================================================
 * "volatile" prevents compiler optimization because these
 * variables map directly to hardware registers that can
 * change outside program control.
 * ========================================================= */

/* 32-bit volatile register */
typedef volatile uint32_t vu32;

/* 16-bit volatile register */
typedef volatile uint16_t vu16;

/* 8-bit volatile register */
typedef volatile uint8_t  vu8;


#endif /* STM32F446XX_TYPES_H */
