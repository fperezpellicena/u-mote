/**
 * This file is part of uMote.
 *
 *  uMote is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *  uMote is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with uMote.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <p18f46j50.h>
#include <portb.h>

#pragma config WDTEN = OFF          /* Disabled - Controlled by SWDTEN bit                                                       */
#pragma config PLLDIV = 2           /* Divide by 2 (8 MHz oscillator input)                                                      */
#pragma config STVREN = ON          /* Reset on stack overflow/underflow enabled                                                 */
#pragma config XINST = OFF          /* Instruction set extension and Indexed Addressing mode disabled (Legacy mode)              */
#pragma config CPUDIV = OSC1        /* No CPU system clock divide                                                                */
#pragma config CP0 = OFF            /* Program memory is not code-protected                                                      */
#pragma config OSC = INTOSCPLL      /* INTOSC with PLL enabled(S/W control), port function on RA6 and RA7, INTOSCPLL used by USB */
#pragma config T1DIG = OFF          /* Secondary Oscillator clock source may not be selected                                     */
#pragma config LPT1OSC = OFF        /* Timer1 oscillator configured for higher power operation                                   */
#pragma config FCMEN = OFF          /* Fail-Safe Clock Monitor disabled                                                          */
#pragma config IESO = OFF           /* Two-Speed Start-up disabled                                                               */
#pragma config WDTPS = 1            /* 1:1                                                                                       */
#pragma config DSWDTOSC = INTOSCREF /* DSWDT uses INTOSC/INTRC as reference clock                                                */
#pragma config RTCOSC = T1OSCREF    /* RTCC uses T1OSC/T1CKI as reference clock                                                  */
#pragma config DSBOREN = OFF        /* Zero-Power BOR disabled in Deep Sleep (does not affect operation in non-Deep Sleep modes) */
#pragma config DSWDTEN = ON         /* Deep Sleep Watchdog Timer Enabled                                                         */
#pragma config DSWDTPS = 8192       /* 1:8,192 (8.5 seconds)                                                                     */
#pragma config IOL1WAY = OFF        /* The IOLOCK bit (PPSCON<0>) can be set and cleared as needed                               */
#pragma config MSSP7B_EN = MSK7     /* 7 Bit address masking                                                                     */
#pragma config WPFP = PAGE_31      /* Write Protect Program Flash Page 31                                                       */
#pragma config WPEND = PAGE_WPFP    /* Write/Erase protect Flash Memory pages starting at page WPFP[5:0]                         */
#pragma config WPCFG = OFF          /* Write/Erase Protection of last page Disabled                                              */
#pragma config WPDIS = OFF          /* WPFP[5:0], WPEND, and WPCFG bits ignored */

char reg;

void toggle_light(void);

#pragma code HIGH_INTERRUPT_VECTOR = 0x08

void high_ISR(void) {
    _asm goto toggle_light _endasm
}
#pragma code

#pragma interrupt toggle_light

void toggle_light(void) {
    if(INTCONbits.RBIF == 1) {
        reg = PORTB;
        _asm nop _endasm
        INTCONbits.RBIF = 0;
        /* Toggle led */
LATEbits.LATE0 = ~LATEbits.LATE0;
}
}

void main(void) {
    /* Output led */
    TRISEbits.TRISE0 = 0;
    /* Input switch */
    TRISBbits.TRISB4 = 1;
    /* Input interrupt enable */
    INTCONbits.RBIE = 1;
    /* Portb high interrupt priority */
    INTCON2bits.RBIP = 1;
    /* enable all high priority interrupts */
    RCONbits.IPEN = 1;
    INTCONbits.GIEH = 1;
    /* Clear flag */
    INTCONbits.RBIF = 0;

    while (1) {
        WDTCONbits.REGSLP = 1;
        UCONbits.SUSPND = 1;
        OSCCONbits.IDLEN = 0;
        DSCONHbits.DSEN = 0;
        Sleep();
        Nop();
        Nop();
    }
}
