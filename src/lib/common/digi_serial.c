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

#include "bsp.h"
#include "digi_serial.h"

void XBeeSerial_init(UINT8 baudrate) {
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
    SPBRG1 = baudrate; // 9600bps -> 12
    // Interrupts
#ifdef  EUSART1_INTERRUPT
    PIE1bits.RC1IE = 1;
    PIE1bits.TX1IE = 0;
    PIR1bits.RC1IF = 0;

    IPR1bits.RC1IP = 1; // High priority defatult
#endif
}

void XBeeSerial_send(UINT8 value) {
    while (!TXSTA1bits.TRMT);
    TXREG1 = value;
}

void XBeeSerial_sendArray(UINT8* values, UINT8 size) {
    UINT8 i = 0;
    while (i < size) {
        XBeeSerial_send(values[i++]);
    }
    XBeeSerial_send(NULL);
}

UINT8 XBeeSerial_read(void) {
    return RCREG1;
}

BOOL XBeeSerial_available(void) {
    if (PIR1bits.RC1IF) {
        return TRUE; // Data is available, return TRUE
    }
    return FALSE;
}

void XBeeSerial_close(void) {
    RCSTA1 &= 0b01001111; // Disable the receiver
    TXSTA1bits.TXEN = 0; // and transmitter
    PIE1 &= 0b11001111; // Disable both interrupts
}

/** Interrupt handler functions */
BOOL XBeeSerial_checkInterrupt(void) {
    return XBeeSerial_available();
}

void XBeeSerial_ackInterrupt(void) {
    PIR1bits.RC1IF = 0;
}