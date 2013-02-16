#include <p18cxxx.h>

#pragma config WDTEN = OFF          //WDT disabled (enabled by SWDTEN bit)
#pragma config STVREN = ON          //stack overflow/underflow reset enabled
#pragma config XINST = OFF          //Extended instruction set disabled
#pragma config CPUDIV = OSC4_PLL6   //CPU system clock divide 6
#pragma config CP0 = OFF            //Program memory is not code-protected
#pragma config OSC = INTOSCPLL      //Internal OSC(8mhz), PLL enabled by soft
#pragma config PLLDIV = 2           //Divide By 2(4 Mhz) to generate 96Mhz
#pragma config FCMEN = OFF          //Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF           //Two-Speed Start-up disabled
#pragma config WDTPS = 32768        //1:32768
#pragma config DSWDTOSC = INTOSCREF //DSWDT uses INTOSC/INTRC as clock
#pragma config RTCOSC = INTOSCREF   //RTCC uses INTRC
#pragma config DSBOREN = OFF        //Zero-Power BOR disabled in Deep Sleep
#pragma config DSWDTEN = OFF        //Disabled
#pragma config DSWDTPS = 8192       //1:8,192 (8.5 seconds)
#pragma config IOL1WAY = OFF        //IOLOCK bit can be set and cleared
#pragma config MSSP7B_EN = MSK7     //7 Bit address masking
#pragma config WPFP = PAGE_1        //Write Protect Program Flash Page 0
#pragma config WPEND = PAGE_0       //Start protection at page 0
#pragma config WPCFG = OFF          //Write/Erase last page protect Disabled
#pragma config WPDIS = OFF          //WPFP[5:0], WPEND, and WPCFG bits ignored

void main(void) {
    // Period
    PR2 = 255;
    // Duty (50%)
    CCPR1L = 127;
    // Unlock registers
    EECON2 = 0x55;
    EECON2 = 0xAA;
    PPSCONbits.IOLOCK = ACCESS;
    // Configure RP5 as CCP1 output
    RPOR13 = 14;
    // Lock registers
    EECON2 = 0x55;
    EECON2 = 0xAA;
    PPSCONbits.IOLOCK = BANKED;
    // Output
    TRISCbits.TRISC2 = 0;
    // Timer 2 prescale 16
    T2CON |= 0b00000010;
    // Timer 2 on
    T2CONbits.TMR2ON = 1;
     // PWM mode
    CCP1CON |= 0b00001100;
    while(1);

    return;
}
