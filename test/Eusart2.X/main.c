#include <p18cxxx.h>
#include <pps.h>
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

//
//int data;
//
///* HIGH LEVEL INTERRUPTION METHOD */
//#pragma interrupt ISR_hi
//
///** Internally, clears GIE bit, so it is not reentrant */
//void ISR_hi(void) {
//    PIR3bits.RC2IF = 0;
//    data = RCREG2;
//}
//
///*..........................................................................*/
//#pragma code intVector_hi = 0x08
//
//void intVector_hi(void) {
//    _asm goto ISR_hi _endasm
//}

void xmitc(char data) {
    while (PIR1bits.TX1IF == 0); // wait until transmit buffer is ready for data
    TXREG1 = data;
}

void xmits(const rom char* data) {
    while (*data != 0) {
        xmitc(*data++);
    }
}

void main(void) {
    
//    PPSUnLock();
//    PPSOutput(PPS_RP17, PPS_TX2CK2); // TX2 RP17/RC6
//    PPSInput(PPS_RX2DT2, PPS_RP18); // RX2 RP18/RC7
//    PPSLock();
//
//    TXSTA2bits.SYNC = 0;
//    TXSTA2bits.BRGH = 0;
//    RCSTA2bits.SREN2 = 1;
//
//    PIE3bits.RC2IE = 1;
//    PIE3bits.TX2IE = 0;
//    PIR3bits.RC2IF = 0;
//    PIR3bits.TX2IF = 0;
//        RCSTA2bits.SPEN = 1;
//    TXSTA2bits.TXEN = 1;
//
//    SPBRG2 = 5; // 9600bps -> 12

     // I/O config.
    TRISCbits.TRISC7 = 1;
    TRISCbits.TRISC6 = 0;
    // Tx status
    TXSTA1bits.SYNC = 0;
    TXSTA1bits.TXEN = 1;
    TXSTA1bits.BRGH = 0;
    // Rx status
    RCSTA1bits.SPEN = 1;
    RCSTA1bits.CREN = 1;
    // Baudrate
    SPBRG1 = 12; // 9600bps -> 12

    LATDbits.LATD7 = 1; // LED output
    TRISDbits.TRISD7 = 0;

    xmitc('2');
    xmits(": Hello from PIC EUSART2!\n");

    // now go into echo mode...
    while (1) {
        if (PIR1bits.RC1IF) {
            xmitc(RCREG2);
            LATDbits.LATD7 ^= 1;
            PIR1bits.RC1IF = 0;
        }
    }
}

