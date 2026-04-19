# STM32F4 USART2 Bare-Metal Project

## 1. Project Overview
This project demonstrates UART transmission using **USART2** on STM32F4 series microcontroller using direct register-level programming (bare-metal approach).

The code continuously transmits the character **'A'** over USART2.

---

## 2. Hardware Details
- MCU: STM32F446RE (NUCLEO board)
- USART Used: USART2
- TX Pin: PA2 (Alternate Function AF7)
- Communication Mode: Asynchronous UART
- Baud Rate: 9600 bps

---

## 3. Register Selection and Usage

### RCC (Reset and Clock Control)
- RCC_AHB1ENR → Enables GPIOA clock
- RCC_APB1ENR → Enables USART2 clock

### GPIOA Configuration
- GPIO_MODER → Set PA2 to Alternate Function mode (10)
- GPIO_AFRL → Select AF7 for USART2

### USART2 Registers
- USART_SR → Status register (TXE flag used)
- USART_DR → Data register (transmission buffer)
- USART_BRR → Baud rate register
- USART_CR1 → Control register (USART enable + TX enable)

---

## 4. Baud Rate Calculation

Assumption:
- PCLK1 = 16 MHz

USART formula:
```
USARTDIV = Fck / (16 × Baud Rate)
```

Substituting:
```
USARTDIV = 16,000,000 / (16 × 9600)
         = 104.1667
```

- Mantissa = 104 (0x68)
- Fraction = 0.1667 × 16 ≈ 3

Final BRR value:
```
0x0683
```

---

## 5. Pin Configuration
- PA2 → USART2_TX (AF7)
- PA3 → USART2_RX (not used)

---

## 6. Working Explanation
1. Enable GPIOA and USART2 clocks
2. Configure PA2 as Alternate Function
3. Select AF7 (USART2)
4. Set baud rate in BRR register
5. Enable USART + transmitter
6. Continuously send 'A'

---

## 7. Expected Output
- Continuous transmission of character **'A'**
- Observe via:
  - ST-LINK Virtual COM Port (VCP)
  - External USB-to-UART converter

---

## 8. Output Screenshot
_Add your terminal / oscilloscope image here_

```
<img width="1365" height="350" alt="image" src="https://github.com/user-attachments/assets/ebce1a81-2f04-428d-954b-ebd843bead80" />


```
