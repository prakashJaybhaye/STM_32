#ifndef USART2_H
#define USART2_H

#include <stdint.h>

void USART2_Init(void);

void USART2_SendChar(char c);

void USART2_SendString(const char *str);

uint8_t USART2_ReceiveChar(unsigned char *c);

uint8_t USART2_ReceiveString(char *buffer, uint32_t max_length);

#endif