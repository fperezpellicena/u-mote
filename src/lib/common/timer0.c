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

#include "timer.h"

void Timer0_init(void) {
    T0CONbits.TMR0ON = 0;
    T0CONbits.T08BIT = 1;   // 8 bit mode
    T0CONbits.T0CS = 0;
    T0CONbits.PSA = 0;
    T0CONbits.T0PS = 6;     // Prescale x128
    TMR0H = 0; 
    TMR0L = 0;
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    INTCON2bits.T0IP = 0;
}

void Timer0_start(void) {
    T0CONbits.TMR0ON = 1;
}

void Timer0_stop(void) {
    T0CONbits.TMR0ON = 0;
    INTCONbits.TMR0IE = 0;
}

BOOL Timer0_checkInterrupt(void) {
    return INTCONbits.TMR0IF;
}

void Timer0_ackInterrupt(void) {
    INTCONbits.TMR0IF = 0;
}

void Timer0_enableInterrupt(void) {
    INTCONbits.TMR0IE = 1;
}

void Timer0_disableInterrupt(void) {
    INTCONbits.TMR0IE = 0;
}
