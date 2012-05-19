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
#include <p18f46j50.h>

/** Interrupt driven uart */

static void Serial_1_init(UINT8 baudrate);
static void Serial_2_init(UINT8 baudrate);

static void Serial_1_init(UINT8 baudrate) {
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

static void Serial_2_init(UINT8 baudrate) {
    // PPS - Configure RX2 RA5 (RP2)
    RPINR16 = 4;
    // PPS - Configure TX2 RA1 (RP1)
    RPOR1 = 5;

    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA5 = 0;

    TXSTA2bits.SYNC = 0;
    TXSTA2bits.TXEN = 1;
    TXSTA2bits.BRGH = 0;

    RCSTA2bits.SPEN2 = 1;
    RCSTA2bits.CREN2 = 1;

    SPBRG2 = baudrate; // 9600bps -> 12
#ifdef EUSART2_INTERRUPT
        PIE3bits.RC2IE = 1;
        PIE3bits.TX2IE = 0;
        PIR3bits.RC2IF = 0;

        IPR3bits.RC2IP = 1; // High priority defatult
#endif
}

void Serial_create(Serial * const serial, UINT8 uart, UINT8 baudrate) {
    serial->uart = uart;
    if (uart == EUSART1) {
        Serial_1_init(baudrate);
    } else {
        Serial_2_init(baudrate);
    }
}

void Serial_send(Serial * const serial, UINT8 value) {
    while (!TXSTA1bits.TRMT);
    if (serial->uart == EUSART1) {
        TXREG1 = value;
    } else {
        TXREG2 = value;
    }
}

UINT8 Serial_read(Serial * const serial) {
    /* Store received byte */
    if (serial->uart == EUSART1) {
        return RCREG1;
    } else {
        return RCREG2;
    }
}

BOOL Serial_available(Serial * const serial) {
    if (serial->uart == EUSART1 && PIR1bits.RC1IF) {
        return TRUE; // Data is available, return TRUE
    } else if (serial->uart == 2 && PIR3bits.RC2IF) {
        return TRUE; // Data is available, return TRUE
    }
    return FALSE;
}

void Serial_close(Serial * const serial) {
    if (serial->uart == EUSART1) {
        RCSTA1 &= 0b01001111; // Disable the receiver
        TXSTA1bits.TXEN = 0; // and transmitter
        PIE1 &= 0b11001111; // Disable both interrupts
    } else {
        RCSTA2 &= 0b01001111; // Disable the receiver
        TXSTA2bits.TXEN = 0; // and transmitter
        PIE3 &= 0b11001111; // Disable both interrupts
    }
}

/** Interrupt handler functions */
BOOL Serial_checkInterrupt(Serial * const serial) {
    return Serial_available(serial);
}

void Serial_ackInterrupt(Serial * const serial) {
    /* Store received byte */
    if (serial->uart == EUSART1) {
        PIR1bits.RC1IF = 0;
    } else {
        PIR3bits.RC2IF = 0;
    }
}