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
#include "usb_function_cdc.h"

/** Init uart 2 in non-interrupt mode */
void Serial_init(UINT8 baudrate) {
    Register_remap((UINT8*)&RPINR16, USART2_RX_RP);
    Register_remap((UINT8*)&RPOR1, USART2_TX_RP);

    TXSTA2 = 0; // Reset USART registers to POR state
    RCSTA2 = 0;

    RCSTA2bits.CREN = 1;
    TXSTA2bits.BRGH = 1;
    PIE3bits.RC2IE = 1;

    SPBRG2 = baudrate;

    TXSTA2bits.TXEN = 1; // Enable transmitter
    RCSTA2bits.SPEN = 1; // Enable receiver

    BAUDCON2bits.BRG16 = 1;
    BAUDCON2bits.WUE = 1;
}

void Serial_send(UINT8 value) {
    while (!TXSTA2bits.TRMT);
    TXREG2 = value;
    while (!TXSTA2bits.TRMT);
}

void Serial_sendArray(UINT8* values, UINT8 size) {
    UINT8 i = 0;
    while (i < size) {
        Serial_send(values[i++]);
    }
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
    return (BOOL) PIR3bits.RC2IF;
}

void Serial_close(void) {
    RCSTA2 &= 0b01001111; // Disable the receiver
    TXSTA2bits.TXEN = 0; // and transmitter
    PIE3 &= 0b11001111; // Disable both interrupts
}

/** Interrupt handler functions */
BOOL Serial_checkInterrupt(void) {
    return (BOOL) PIR3bits.RC2IF;
}

void Serial_ackInterrupt(void) {
    PIR3bits.RC2IF = 0;
    BAUDCON2bits.ABDOVF = 0;
    BAUDCON2bits.WUE = 1;
}