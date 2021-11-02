// standard includes
#include <msp430.h> 
#include <stdio.h>

// project includes
#include "adc.h"
#include "defines.h"
#include "uart.h"

// global definitions
int UART_queue[16];

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5; // disable GPIO power-on high-impedance mode

    // initializations
    configureADC();
    UART_Init();

    while (1)
    {

        char *test = "testing a longer string\n";

        UART_WriteString(test);

    }

}

