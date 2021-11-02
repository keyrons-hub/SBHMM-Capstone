#include "uart.h"

// function to initialize UART module, taken from msp430fr59xx_euscia0_uart_01.c
// baud rate: 9600, BRCLK = 8 MHz
// using back-channel UART pins P2.0 and P2.1
// how many start bits, how many stop bits, are we using parity?
void UART_Init(void){

    // Configure GPIO
    P2SEL1 |= BIT0 | BIT1;                    // USCI_A0 UART operation
    P2SEL0 &= ~(BIT0 | BIT1);

    // Startup clock system with max DCO setting ~8MHz
    CSCTL0_H = CSKEY >> 8;                    // Unlock clock register
    CSCTL1 = DCOFSEL_3 | DCORSEL;             // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
    CSCTL0_H = 0;                             // Lock CS registers

    // Configure USCI_A0 for UART mode
    UCA0CTLW0 = UCSWRST;                      // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK

    // Baud Rate calculation
    // 8000000/(16*9600) = 52.083
    // Fractional portion = 0.083
    // User's Guide Table 21-4: UCBRSx = 0x04
    // UCBRFx = int ( (52.083-52)*16) = 1
    UCA0BR0 = 52;                             // 8000000/16/9600
    UCA0BR1 = 0x00;
    UCA0MCTLW |= UCOS16 | UCBRF_1;
    UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

    /*
     * NOTE: not necessary to enter LPM3 as we've already enabled the Rx interrupt
     */
    //__bis_SR_register(LPM3_bits | GIE);       // Enter LPM3, interrupts enabled
    //__no_operation();                         // For debugger
}

// write single character into Tx buffer
void UART_WriteChar(char data){

    while(!(UCA0IFG&UCTXIFG));

    UCA0TXBUF = data;

}

// write entire string to UART, using write char function
void UART_WriteString(char *str){

    char index;

    /*
     * NOTE: this function assumes the string inputted is a null-terminated string
     * (the last character in the string equals zero)
     */
    for (index = 0; *(str + index); index++){
        UART_WriteChar(*(str + index));
    }

}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR       // tells compiler to use this function to deal with interrupt vector
__interrupt void USCI_A0_ISR(void)      // ISR definition

#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif

{
  switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
      while(!(UCA0IFG&UCTXIFG));    // when buffer is not free, don't interrupt
      UCA0TXBUF = UCA0RXBUF;
      __no_operation();
      break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}
