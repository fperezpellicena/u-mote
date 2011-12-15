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

#pragma config WDTEN = OFF          /* Disabled - Controlled by SWDTEN bit                                                       */

void switchChange(void);
void debounceSwitch(void);
int debounce(int bit);

#pragma interruptlow switchChange

void switchChange() {
    char reg;
    if (INTCONbits.RBIF) {
        reg = PORTB;
        _asm nop _endasm
                INTCONbits.RBIF = 0;
        // Configure Timer0 OVF int.
        INTCONbits.TMR0IE = 1;
        // Start periodic sampling
        T0CONbits.TMR0ON = 1;
    }
}

#pragma interrupt debounceSwitch

void debounceSwitch(void) {
    if (INTCONbits.TMR0IF) {
        INTCONbits.TMR0IF = 0;
        if (debounce(LATBbits.LATB4)) {
            // Stop timer
            T0CONbits.TMR0ON = 0;
            TMR0H = 0; //clear timer
            TMR0L = 0; //clear timer
            // Toggle
            LATEbits.LATE0 = ~LATEbits.LATE0;
        }
    }
}

#pragma code HIGH_INTERRUPT_VECTOR = 0x08

void high_ISR(void) {
    _asm goto debounceSwitch _endasm
}
#pragma code

#pragma code LOW_INTERRUPT_VECTOR = 0x18

void low_ISR(void) {
    _asm goto switchChange _endasm
}
#pragma code

int debounce(int portBit) {
    static unsigned long state = 0;
    state = state << 1 | !portBit | 0xE000;
    if (state > 0xF000) {
        return 1;
    }
    return 0;
}

void main(void) {
    TRISEbits.TRISE0 = 0;
    LATEbits.LATE0 = 0;
    TRISBbits.TRISB4 = 1;

    // Configure Timer0
    T0CONbits.T0PS = 7;
    T0CONbits.T08BIT = 1;
    T0CONbits.T0CS = 0;
    TMR0H = 0; //clear timer
    TMR0L = 0; //clear timer


    INTCONbits.RBIE = 1;
    INTCON2bits.RBIP = 0;
    INTCON2bits.TMR0IP = 1;

    RCONbits.IPEN = 1;
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;

    T0CONbits.TMR0ON = 0;
    INTCONbits.RBIF = 0;
    INTCONbits.TMR0IF = 0;

    while (1);
}