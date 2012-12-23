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
#include "hw_serial.h"
#include "register.h"
#include <string.h>

/** Init uart in non-interrupt mode */
void Serial_init(UINT8 baudrate) {
    Register_unLockIO();
    // PPS - Configure RX2 RA0 (RP0)
    RPINR16 = 1;
    // PPS - Configure TX2 RA1 (RP1)
    RPOR1 = 5;
    Register_lockIO();
    
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 0;

    TXSTA2bits.SYNC = 0;
    TXSTA2bits.TXEN = 1;
    TXSTA2bits.BRGH = 0;

    RCSTA2bits.SPEN2 = 1;
    RCSTA2bits.CREN2 = 1;

    SPBRG2 = baudrate; // 9600bps -> 12
}

/** Interrupt driven uart */
void Serial_initInterrupt(UINT8 baudrate) {
    Serial_init(baudrate);
    // Interrupt config
    PIE3bits.RC2IE = 1;
    PIE3bits.TX2IE = 0;
    PIR3bits.RC2IF = 0;
    // High priority defatult
    IPR3bits.RC2IP = 1; 
}

void Serial_send(UINT8 value) {
    while (!TXSTA2bits.TRMT);
    TXREG2 = value;
}

void Serial_sendArray(UINT8* values, UINT8 size) {
    UINT8 i = 0;
    while (i < size) {
        Serial_send(values[i++]);
    }
    //Serial_send(NULL);
}

void Serial_sendROMArray(UINT8 rom* values, UINT8 size) {
    UINT8 i = 0;
    while (i < size) {
        Serial_send(values[i++]);
    }
}

UINT8 Serial_read(void) {
    return RCREG2;
}

BOOL Serial_available(void) {
    if (PIR3bits.RC2IF) {
        return TRUE; // Data is available, return TRUE
    }
    return FALSE;
}

void Serial_close(void) {
    RCSTA2 &= 0b01001111; // Disable the receiver
    TXSTA2bits.TXEN = 0; // and transmitter
    PIE3 &= 0b11001111; // Disable both interrupts
}

/** Interrupt handler functions */
BOOL Serial_checkInterrupt(void) {
    return Serial_available();
}

void Serial_ackInterrupt(void) {
    PIR3bits.RC2IF = 0;
}