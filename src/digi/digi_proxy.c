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

#include "digi_proxy.h"

static boolean XBeeProxy_sendPacket(XBeePacket* packet, uint8_t size) {
	uint8_t* p = (uint8_t*) packet;
	Serial_send(START_DELIMITER);
	// send the most significant bit
	Serial_send((size >> 8) & 0xFF);
	// then the LSB
	Serial_send(size & 0xFF); 
	// just in case it hasn't been initialized.
	packet->checksum = 0; 
	//Generalizando para cualquier paquete
	while (size--) {
		Serial_send(*p);
		packet->checksum += *p++;
	}
	Serial_send(0xFF - packet->checksum);
	return true;
}

boolean XBeeProxy_sendAtCommand(XBeePacket* packet, uint8_t length) {
	int size = 0;
	if(packet->apiId == AT_COMMAND || packet->apiId == AT_COMMAND_QUEUE) {
		size = (length > 0) ? 8 : 4; // Asume que el comando se aplica
		return XBeeProxy_sendPacket(packet, size);
	}
	return false;
}

boolean XBeeProxy_sendTransmitRequest(XBeePacket* packet, uint8_t length) {
	int size = length;
	if(packet->apiId == TRANSMIT_REQUEST) {
		size += 14;
		return XBeeProxy_sendPacket(packet, size);
	}
	return false;
}

boolean XBeeProxy_sendExplicitAddressing(XBeePacket* packet, uint8_t length) {
	int size = length;
	if(packet->apiId == EXPLICIT_ADDRESSING) {
		size += 20;
		return XBeeProxy_sendPacket(packet, size);
	}
	return false;
}

boolean XBeeProxy_sendRemoteAtCommand(XBeePacket* packet, uint8_t length) {
	int size = length;
	if(packet->apiId == REMOTE_AT_COMMAND) {
		size += 16;
		return XBeeProxy_sendPacket(packet, size);
	}
	return false;
}
