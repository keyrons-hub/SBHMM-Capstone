#ifndef USCI_UART_H
#define USCI_UART_H
#include <msp430.h>

void UART_Init(void);
void UART_WriteChar(char data);
void UART_WriteString(char *str);
  
#endif /* USCI_UART_H */
