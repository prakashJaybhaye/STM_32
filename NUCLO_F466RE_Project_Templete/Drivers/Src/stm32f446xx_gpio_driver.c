#include "stm32f446xx_types.h"
#include "stm32f446xx_rcc.h"
#include "stm32f446xx_gpio_reg.h"
#include "stm32f446xx_gpio_driver.h"

/* =========================================================
 * Enable peripheral clock for GPIO port
 * This is required before accessing GPIO registers
 * ========================================================= */
void GPIO_PeripheralClockControl(GPIO_RegDef_t *pGPIOx, u8 ENABLE)
{
    if (ENABLE)
    {
        if (pGPIOx == GPIOA)
            GPIOA_CLK_EN();
        else if (pGPIOx == GPIOB)
            GPIOB_CLK_EN();
        else if (pGPIOx == GPIOC)
            GPIOC_CLK_EN();
        else if (pGPIOx == GPIOD)
            GPIOD_CLK_EN();
        else if (pGPIOx == GPIOE)
            GPIOE_CLK_EN();
        else if (pGPIOx == GPIOF)
            GPIOF_CLK_EN();
        else if (pGPIOx == GPIOG)
            GPIOG_CLK_EN();
        else if (pGPIOx == GPIOH)
            GPIOH_CLK_EN();
    }
}

/* =========================================================
 * GPIO Initialization API
 * Configures mode, speed, pull-up/down, output type, AF
 * ========================================================= */
void GPIO_Init(GPIO_Handler_t *pGPIOHandler)
{
    u8 pin = pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber;

    /* 1. Enable GPIO peripheral clock */
    GPIO_PeripheralClockControl(pGPIOHandler->pGPIOx, 1);

    /* 2. Configure MODE (MODER register) */
    pGPIOHandler->pGPIOx->MODER &= ~(0x3 << (2 * pin));

    if (pGPIOHandler->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
    {
        if (pGPIOHandler->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_OUTPUT)
        {
            pGPIOHandler->pGPIOx->MODER |= (GPIO_MODE_OUTPUT << (2 * pin));
        }
        else if (pGPIOHandler->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_INPUT)
        {
            pGPIOHandler->pGPIOx->MODER |= (GPIO_MODE_INPUT << (2 * pin));
        }
        else if (pGPIOHandler->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
        {
            pGPIOHandler->pGPIOx->MODER |= (GPIO_MODE_ALTFN << (2 * pin));

            /* Configure Alternate Function register (AFR) */
            if (pin < 8)
            {
                pGPIOHandler->pGPIOx->AFR[0] &= ~(0xF << (4 * pin));
                pGPIOHandler->pGPIOx->AFR[0] |= (pGPIOHandler->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * pin));
            }
            else
            {
                pGPIOHandler->pGPIOx->AFR[1] &= ~(0xF << (4 * (pin - 8)));
                pGPIOHandler->pGPIOx->AFR[1] |= (pGPIOHandler->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * (pin - 8)));
            }
        }
        else if (pGPIOHandler->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ANALOG)
        {
            pGPIOHandler->pGPIOx->MODER |= (GPIO_MODE_ANALOG << (2 * pin));
        }
    }
    else if (pGPIOHandler->GPIO_PinConfig.GPIO_PinMode >= GPIO_MODE_IT_FT)
    {
        /* Interrupt mode → configure as INPUT only */
        pGPIOHandler->pGPIOx->MODER &= ~(0x3 << (2 * pin));
        pGPIOHandler->pGPIOx->MODER |= (GPIO_MODE_INPUT << (2 * pin));
    }

    /* 3. Configure output type (Push-Pull / Open-Drain) */
    pGPIOHandler->pGPIOx->OTYPER &= ~(0x1 << pin);

    if (pGPIOHandler->GPIO_PinConfig.GPIO_PinOPType == GPIO_OP_TYPE_OD)
    {
        pGPIOHandler->pGPIOx->OTYPER |= (1 << pin);
    }

    /* 4. Configure output speed */
    pGPIOHandler->pGPIOx->OSPEEDR &= ~(0x3 << (2 * pin));

    if (pGPIOHandler->GPIO_PinConfig.GPIO_PinSpeed == GPIO_SPEED_LOW)
        pGPIOHandler->pGPIOx->OSPEEDR |= (GPIO_SPEED_LOW << (2 * pin));
    else if (pGPIOHandler->GPIO_PinConfig.GPIO_PinSpeed == GPIO_SPEED_MEDIUM)
        pGPIOHandler->pGPIOx->OSPEEDR |= (GPIO_SPEED_MEDIUM << (2 * pin));
    else if (pGPIOHandler->GPIO_PinConfig.GPIO_PinSpeed == GPIO_SPEED_FAST)
        pGPIOHandler->pGPIOx->OSPEEDR |= (GPIO_SPEED_FAST << (2 * pin));
    else if (pGPIOHandler->GPIO_PinConfig.GPIO_PinSpeed == GPIO_SPEED_HIGH)
        pGPIOHandler->pGPIOx->OSPEEDR |= (GPIO_SPEED_HIGH << (2 * pin));

    /* 5. Configure Pull-up / Pull-down */
    pGPIOHandler->pGPIOx->PUPDR &= ~(0x3 << (2 * pin));

    if (pGPIOHandler->GPIO_PinConfig.GPIO_PinPuPdControl == GPIO_NO_PUPD)
        pGPIOHandler->pGPIOx->PUPDR |= (GPIO_NO_PUPD << (2 * pin));
    else if (pGPIOHandler->GPIO_PinConfig.GPIO_PinPuPdControl == GPIO_PU)
        pGPIOHandler->pGPIOx->PUPDR |= (GPIO_PU << (2 * pin));
    else if (pGPIOHandler->GPIO_PinConfig.GPIO_PinPuPdControl == GPIO_PD)
        pGPIOHandler->pGPIOx->PUPDR |= (GPIO_PD << (2 * pin));
}

/* =========================================================
 * Reset GPIO peripheral to default state
 * ========================================================= */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
    if (pGPIOx == GPIOA) GPIOA_CLK_RST();
    else if (pGPIOx == GPIOB) GPIOB_CLK_RST();
    else if (pGPIOx == GPIOC) GPIOC_CLK_RST();
    else if (pGPIOx == GPIOD) GPIOD_CLK_RST();
    else if (pGPIOx == GPIOE) GPIOE_CLK_RST();
    else if (pGPIOx == GPIOF) GPIOF_CLK_RST();
    else if (pGPIOx == GPIOG) GPIOG_CLK_RST();
    else if (pGPIOx == GPIOH) GPIOH_CLK_RST();
}

/* =========================================================
 * Write logic level to GPIO pin
 * Uses BSRR register for atomic set/reset
 * ========================================================= */
void GPIO_WritePin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, GPIO_PinState_t Value)
{
    if (Value == GPIO_PIN_SET)
        pGPIOx->BSRR = (1 << PinNumber);  /* Set pin */
    else
        pGPIOx->BSRR = (1 << (PinNumber + BSRR_PIN_RESET)); /* Reset pin */
}

/* =========================================================
 * Read input state of GPIO pin
 * ========================================================= */
GPIO_PinState_t GPIO_ReadPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
    return (GPIO_PinState_t)((pGPIOx->IDR >> PinNumber) & 1);
}

/* =========================================================
 * Toggle GPIO output pin
 * ========================================================= */
void GPIO_TogglePin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
    pGPIOx->ODR ^= (1 << PinNumber);
}
