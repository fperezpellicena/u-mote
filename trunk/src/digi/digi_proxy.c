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

//FIXME Esta api no termina de ser clara
void XBeeProxy_create(XBeeProxy * const proxy, Serial * const serial, XBee * const xbee) {
	proxy->serial = serial;
	proxy->xbee = xbee;
}

boolean XBeeProxy_readPacket(XBeeProxy * const proxy, XBeePacket * const packet) {
	uint8_t data;
	XBee_resetPacket(packet);
	while (Serial_available(proxy->serial)) {
		data = Serial_read(proxy->serial);
		switch (packet->rxState) {
			case XBEE_PACKET_RX_START:
				if (data == XBEE_PACKET_RX_START)
					packet->rxState = XBEE_PACKET_RX_LENGTH_1;
				break;
			case XBEE_PACKET_RX_LENGTH_1:
				packet->length = data;
				packet->length <<= 8;
				packet->rxState = XBEE_PACKET_RX_LENGTH_2;
				break;
			case XBEE_PACKET_RX_LENGTH_2:
				packet->length += data;
				if (packet->length > MAX_PACKET_SIZE) { // in case we somehow get some garbage
					packet->rxState = XBEE_PACKET_RX_START;
					return false; //LENGTH Error
				} else {
					packet->rxState = XBEE_PACKET_RX_PAYLOAD;
				}
				packet->checksum = 0;
				break;
			case XBEE_PACKET_RX_PAYLOAD:
				*packet->dataPtr++ = data;
				if (++packet->index >= packet->length) {
					packet->rxState = XBEE_PACKET_RX_CRC;
				}
				packet->checksum += data;
				break;
			case XBEE_PACKET_RX_CRC:
				packet->checksum += data;
				packet->rxState = XBEE_PACKET_RX_START;
				if (packet->checksum == 0xFF) {
					return true; //Everything OK!
				} else {
					return false; //CRC Error
				}
		}
	}
	return true;
}

static boolean XBeeProxy_sendPacket(XBeeProxy * const proxy, XBeePacket * const packet,
		uint8_t size) {
	uint8_t* p = (uint8_t*) packet;
	Serial_send(proxy->serial, START_DELIMITER);
	// send the most significant bit
	Serial_send(proxy->serial, (size >> 8) & 0xFF);
	// then the LSB
	Serial_send(proxy->serial, size & 0xFF);
	// just in case it hasn't been initialized.
	packet->checksum = 0;
	//Generalizando para cualquier paquete
	while (size--) {
		Serial_send(proxy->serial, *p);
		packet->checksum += *p++;
	}
	Serial_send(proxy->serial, (0xFF - packet->checksum));
	return true;
}

boolean XBeeProxy_sendAtCommand(XBeeProxy * const proxy,
		XBeePacket * const packet, uint8_t length) {
	int size = 0;
	if (packet->apiId == AT_COMMAND || packet->apiId == AT_COMMAND_QUEUE) {
		size = (length > 0) ? 8 : 4; // Asume que el comando se aplica
		return XBeeProxy_sendPacket(proxy, packet, size);
	}
	return false;
}

boolean XBeeProxy_sendTransmitRequest(XBeeProxy * const proxy,
		XBeePacket * const packet, uint8_t length) {
	int size = length;
	if (packet->apiId == TRANSMIT_REQUEST) {
		size += 14;
		return XBeeProxy_sendPacket(proxy, packet, size);
	}
	return false;
}

boolean XBeeProxy_sendExplicitAddressing(XBeeProxy * const proxy,
		XBeePacket * const packet, uint8_t length) {
	int size = length;
	if (packet->apiId == EXPLICIT_ADDRESSING) {
		size += 20;
		return XBeeProxy_sendPacket(proxy, packet, size);
	}
	return false;
}

boolean XBeeProxy_sendRemoteAtCommand(XBeeProxy * const proxy,
		XBeePacket * const packet, uint8_t length) {
	int size = length;
	if (packet->apiId == REMOTE_AT_COMMAND) {
		size += 16;
		return XBeeProxy_sendPacket(proxy, packet, size);
	}
	return false;
}
