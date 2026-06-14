#ifndef STM32F446XX_RCC_H
#define STM32F446XX_RCC_H

#include <stdint.h>
#include "stm32f446xx_types.h"

#define RCC_BASE_ADDR 0x40023800UL

typedef struct
{
    vu32 CR;              /* RCC_BASE + 0x00 = 0x40023800 */
    vu32 PLLCFGR;         /* RCC_BASE + 0x04 = 0x40023804 */
    vu32 CFGR;            /* RCC_BASE + 0x08 = 0x40023808 */
    vu32 CIR;             /* RCC_BASE + 0x0C = 0x4002380C */

    vu32 AHB1RSTR;        /* RCC_BASE + 0x10 = 0x40023810 */
    vu32 AHB2RSTR;        /* RCC_BASE + 0x14 = 0x40023814 */
    vu32 AHB3RSTR;        /* RCC_BASE + 0x18 = 0x40023818 */
    u32 RESERVED0;            /* RCC_BASE + 0x1C = 0x4002381C */

    vu32 APB1RSTR;        /* RCC_BASE + 0x20 = 0x40023820 */
    vu32 APB2RSTR;        /* RCC_BASE + 0x24 = 0x40023824 */
    u32 RESERVED1[2];         /* RCC_BASE + 0x28 = 0x40023828 */
                                   /* RCC_BASE + 0x2C = 0x4002382C */

    vu32 AHB1ENR;         /* RCC_BASE + 0x30 = 0x40023830 */
    vu32 AHB2ENR;         /* RCC_BASE + 0x34 = 0x40023834 */
    vu32 AHB3ENR;         /* RCC_BASE + 0x38 = 0x40023838 */
    u32 RESERVED2;            /* RCC_BASE + 0x3C = 0x4002383C */

    vu32 APB1ENR;         /* RCC_BASE + 0x40 = 0x40023840 */
    vu32 APB2ENR;         /* RCC_BASE + 0x44 = 0x40023844 */
    u32 RESERVED3[2];         /* RCC_BASE + 0x48 = 0x40023848 */
                                   /* RCC_BASE + 0x4C = 0x4002384C */

    vu32 AHB1LPENR;       /* RCC_BASE + 0x50 = 0x40023850 */
    vu32 AHB2LPENR;       /* RCC_BASE + 0x54 = 0x40023854 */
    vu32 AHB3LPENR;       /* RCC_BASE + 0x58 = 0x40023858 */
    u32 RESERVED4;            /* RCC_BASE + 0x5C = 0x4002385C */

    vu32 APB1LPENR;       /* RCC_BASE + 0x60 = 0x40023860 */
    vu32 APB2LPENR;       /* RCC_BASE + 0x64 = 0x40023864 */
    u32 RESERVED5[2];         /* RCC_BASE + 0x68 = 0x40023868 */
                                   /* RCC_BASE + 0x6C = 0x4002386C */

    vu32 BDCR;            /* RCC_BASE + 0x70 = 0x40023870 */
    vu32 CSR;             /* RCC_BASE + 0x74 = 0x40023874 */
    u32 RESERVED6[2];         /* RCC_BASE + 0x78 = 0x40023878 */
                                   /* RCC_BASE + 0x7C = 0x4002387C */

    vu32 SSCGR;           /* RCC_BASE + 0x80 = 0x40023880 */
    vu32 PLLI2SCFGR;      /* RCC_BASE + 0x84 = 0x40023884 */
    vu32 PLLSAICFGR;      /* RCC_BASE + 0x88 = 0x40023888 */
    vu32 DCKCFGR;         /* RCC_BASE + 0x8C = 0x4002388C */
    vu32 CKGATENR;        /* RCC_BASE + 0x90 = 0x40023890 */
    vu32 DCKCFGR2;        /* RCC_BASE + 0x94 = 0x40023894 */

} RCC_RegDef_t;

/*
 * RCC Peripheral Base Address = 0x40023800
 *
 * Example:
 * RCC->AHB1ENR
 *
 * Address Calculation:
 * RCC Base Address      : 0x40023800
 * AHB1ENR Offset        : 0x30
 * Actual Register Addr  : 0x40023830
 *
 * Compiler calculates:
 * ((RCC_RegDef_t *)0x40023800)->AHB1ENR
 *
 * Similar examples:
 * RCC->AHB1RSTR = 0x40023810 (Offset 0x10)
 * RCC->APB1ENR  = 0x40023840 (Offset 0x40)
 */

#define RCC ((RCC_RegDef_t *) RCC_BASE_ADDR) 


#endif // STM32F446XX_RCC_H
