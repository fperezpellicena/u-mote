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

#ifndef digi_serial_h
#define digi_serial_h

#include "GenericTypeDefs.h"

void XBeeSerial_init(UINT8 baudrate);

void XBeeSerial_send(UINT8 value);

void XBeeSerial_sendArray(UINT8* values, UINT8 size);

UINT8 XBeeSerial_read();

BOOL XBeeSerial_available();

void XBeeSerial_close();

BOOL XBeeSerial_checkInterrupt();

void XBeeSerial_ackInterrupt();

#endif