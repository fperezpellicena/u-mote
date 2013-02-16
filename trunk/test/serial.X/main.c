/*
 * File:   main.c
 * Author: FRANCISCO
 *
 * Created on 1 de mayo de 2012, 16:08
 */

#include <p18cxxx.h>
#include <usart.h>

#pragma config WDTEN = OFF          //WDT disabled (enabled by SWDTEN bit)
#pragma config WDTPS = 1024         //1:1024 (4 seconds)
#pragma config DSWDTOSC = INTOSCREF //DSWDT uses INTOSC/INTRC as clock
#pragma config DSWDTEN = ON         //Enabled
#pragma config DSWDTPS = K524       //9 minutes

#pragma config STVREN = ON          //stack overflow/underflow reset enabled
#pragma config XINST = OFF          //Extended instruction set disabled
#pragma config CP0 = OFF            //Program memory is not code-protected

#pragma config OSC = INTOSCPLL      //Internal OSC(8mhz), PLL enabled by soft
#pragma config PLLDIV = 2           //Divide By 2(4 Mhz) to generate 96Mhz
#pragma config CPUDIV = OSC4_PLL6   //CPU system clock divide 6

#pragma config FCMEN = OFF          //Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF           //Two-Speed Start-up disabled

#pragma config RTCOSC = INTOSCREF   //RTCC uses INTRC
#pragma config DSBOREN = OFF        //Zero-Power BOR disabled in Deep Sleep

#pragma config IOL1WAY = OFF        //IOLOCK bit can be set and cleared
#pragma config MSSP7B_EN = MSK7     //7 Bit address masking
#pragma config WPFP = PAGE_1        //Write Protect Program Flash Page 0
#pragma config WPEND = PAGE_0       //Start protection at page 0
#pragma config WPCFG = OFF          //Write/Erase last page protect Disabled
#pragma config WPDIS = OFF          //WPFP[5:0], WPEND, and WPCFG bits ignored


/* HIGH LEVEL INTERRUPTION METHOD */
#pragma interrupt ISR_hi

void ISR_hi(void) {
    char c = RCREG;
    TXREG = c;
    PIR1bits.RCIF = 0;
}

/*..........................................................................*/
#pragma code intVector_hi = 0x08

void intVector_hi(void) {
    _asm goto ISR_hi _endasm
}

void main(void) {
    
    TRISCbits.TRISC7 = 1;
    TRISCbits.TRISC6 = 0;

    TXSTAbits.SYNC = 0;
    TXSTAbits.TXEN = 1;
    TXSTAbits.BRGH = 0;

    RCSTAbits.SPEN = 1;
    RCSTAbits.CREN = 1;

    SPBRG = 12;  // 9600bps
    
    PIE1bits.RC1IE = 1;
    PIE1bits.TXIE = 0;
    PIR1bits.RC1IF = 0;

    IPR1bits.RCIP = 1;
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;
    
    while(1) {
    }
}
