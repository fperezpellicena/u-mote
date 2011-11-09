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

#include "hw_serial.h"
#include <usart.h>

void Serial_create(Serial * const serial, uint8_t uart, uint8_t config,
		uint8_t baudrate) {
	serial->uart = uart;
	serial->config = config;
	serial->baudrate = baudrate;
}

void Serial_init(Serial * const serial) {
	if (serial->uart == 1) {
		Open1USART(serial->config, serial->baudrate);
	} else {
		Open2USART(serial->config, serial->baudrate);
	}
}

void Serial_send(Serial * const serial, uint8_t value) {
	while(Serial_busy(serial));
	if (serial->uart == 1) {
		Write1USART(value);
	} else {
		Write2USART(value);
	}
}

uint8_t Serial_read(Serial * const serial) {
	if (serial->uart == 1) {
		return Read1USART();
	} else {
		return Read2USART();
	}
}

boolean Serial_available(Serial * const serial) {
	if (serial->uart == 1 && DataRdy1USART()) {
		return true;
	} else if (serial->uart == 2 && DataRdy2USART()) {
		return true;
	}
	return false;
}

void Serial_close(Serial * const serial) {
	if (serial->uart == 1) {
		Close1USART();
	} else {
		Close2USART();
	}
}

boolean Serial_busy(Serial * const serial) {
	if (serial->uart == 1 && Busy1USART()) {
		return true;
	} else if(serial->uart == 2 && Busy2USART()){
		return true;
	}
	return false;
}
