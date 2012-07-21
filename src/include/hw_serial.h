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

#ifndef hw_serial_h
#define hw_serial_h

#include "GenericTypeDefs.h"

typedef struct Serial Serial;

struct Serial {
    UINT8 uart;
};

void Serial_init(Serial * const serial, UINT8 uart, UINT8 baudrate);

void Serial_close(Serial * const serial);

void Serial_send(Serial * const serial, UINT8 value);

void Serial_sendArray(Serial * const serial, UINT8* values, UINT8 size);

UINT8 Serial_read(Serial * const serial);

BOOL Serial_available(Serial * const serial);

/** Interrupt handler functions */
BOOL Serial_checkInterrupt(Serial * const serial);

void Serial_ackInterrupt(Serial * const serial);

#endif          /* hw_serial_h */
