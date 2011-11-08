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

#include "qep_port.h"

typedef struct Serial Serial;

struct Serial {
    uint8_t uart;       /* Serial port: 1 or 2*/
    uint8_t baudrate;   /* Baudrate */
    uint8_t config;     /* Mask config */
};

void Serial_create(Serial* const serial, uint8_t uart, uint8_t baudrate);

void Serial_init(Serial* const serial);

void Serial_send(Serial* const serial, uint8_t value);

uint8_t Serial_read(Serial* const serial);

boolean Serial_available(Serial* const serial);

#endif          /* hw_serial_h */
