#include <msp430.h>
#include "adc.h"

// use BRCLK
// determine sampling rate based on frequency content of sensor voltage
// Nyquist rate
void configureADC(void){

    // 16 S&H cycles, can be adjusted to output impedance if need be
    ADC12CTL0 = ADC12SHT0_2 | ADC12ON;
    // 15-12    ADC12SHT1x  sample-and-hold time                        0000b -
    // 11-8 ADC12SHT0x  sample-and-hold time (different registers)      0010b - ADCSHT0_2 = 16 ADC12CLK cycles
    // 7    ADC12MSC    multiple sample and conversion                  0b -
    // 6-5  reserved                                                    00b -
    // 4    ADC12ON     on signal                                       1b - ADC12ON
    // 3-2  reserved                                                    00b -
    // 1    ADC12ENC    enable conversion                               0b -
    // 0    ADC12SC     start conversion                                0b -

    // use sampling timer
    ADC12CTL1 = ADC12SHP;
    // 15   reserved                                                    0b -
    // 14-13    ADC12PDIV   predivider                                  00b -
    // 12-10    ADC12SHSx   sample-and-hold source select               000b -
    // 9    ADC12SHP    sample-and-hold pulse mode select               1b - ADC12SHP = SAMPCON signal is sourced from the sampling timer
    // 8    ADC12ISSH   invert signal sample-and-hold                   0b -
    // 7-5  ADC12DIVx   clock divider                                   000b -
    // 4-3  ADC12SSELx  clock source select                             00b -
    // 2-1  ADC12CONSEQx    conversion sequence mode select             00b -
    // 0    ADC12BUSY   busy signal                                     0b -

    // 12-bit conversion results
    ADC12CTL2 = ADC12RES_2;
    // 15-6 reserved                                                    0000000000b -
    // 5-4  ADC12RES    conversion result resolution                    10b - ADC12RES_2 = 12 bit resolution
    // 3    ADC12DF     data read-back format                           0b -
    // 2-1  reserved                                                    00b -
    // 0    ADC12PWRMD  low-power mode enable                           0b -

    // A10 ADC input select (need to add more inputs);
    // Vref = AVCC
    ADC12MCTL0 |= ADC12INCH_10;
    // 15   reserved                                                    0b -
    // 14   ADC12WINC   comparator window enable                        0b -
    // 13   ADC12DIF    differential mode                               0b -
    // 12   reserved                                                    0b -
    // 11-8 ADC12VRSEL  combos of Vr+ and Vr-                           000b -
    // 7    ADC12EOS    end of sequence                                 0b -
    // 6-5  reserved                                                    00b -
    // 4-0  ADC12INCHx  input channel select                            01010b - ADC12INCH_10 = Input channel 10

    // enable ADC conversion complete interrupt
    ADC12IER0 |= ADC12IE0;
    // 15-0 ADC12IEx    enables interrupt request for ADC12IFGx bit     0000000000000001b - ADC12IE0 = interrupt enabled for ADC12IFG0 bit
}




