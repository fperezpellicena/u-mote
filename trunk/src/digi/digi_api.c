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

#include "digi_api.h"
#include <string.h>

// FIXME Revisar las longitudes de los paquetes recibidos
// FIXME Revisar los tipos pointer to pointer (**)


/*..........................................................................*/

/* UTIL METHODS */

void XBee_resetPacket(XBeePacket * const packet) {
	packet->dataPtr = (UINT8*) packet;
	packet->checksum = 0;
	packet->rxState = XBEE_PACKET_RX_START;
	packet->length = 0;
	packet->index = 0;
	packet->apiId = 0;
	memset(packet->frame.payload, 0, 100); //FIXME Magic number
}

UINT8 XBee_escape(UINT8 value) {
	if (value == START_DELIMITER || value == XON
			|| value == XOFF || value == ESCAPE) {
		return value ^ 0x20;
	}
	return value;
}

/*..........................................................................*/

/* TX METHODS */

void XBee_createCompleteATCommandPacket(XBeePacket* packet, UINT8 frameId,
		UINT8* command, UINT8* params, UINT8 paramsLength) {
	UINT8* p = packet->frame.atCommand.command;
	UINT8 i = paramsLength;
	*p++ = *command++;
	*p++ = *command;
	p = packet->frame.atCommand.value;
	while (i--) {
		*p++ = *params++;
	}
	packet->apiId = AT_COMMAND;
	packet->frame.atCommand.frameId = frameId;
	packet->length = AT_COMMAND_MINLENGTH + paramsLength;
}

/*..........................................................................*/

void XBee_createATCommandPacket(XBeePacket* packet, UINT8 frameId,
		UINT8* command) {
	UINT8* p = packet->frame.atCommand.command;
	*p++ = *command++;
	*p++ = *command;
	packet->apiId = AT_COMMAND;
	packet->frame.atCommand.frameId = frameId;
	packet->length = AT_COMMAND_MINLENGTH;
}

/*..........................................................................*/

void XBee_createTransmitRequestPacket(XBeePacket* packet, UINT8 frameId,
		UINT8* destinationAddress, UINT8 radious, UINT8 options,
		UINT8* payload, UINT8 dataLength) {
	UINT8* p = packet->frame.transmitRequest.destinationAddress;
	UINT8 i = MAC_ADDRESS_BYTES_LENGTH;
	while (i--) {
		*p++ = *destinationAddress++;
	}
	p = packet->frame.transmitRequest.payload;
	i = dataLength;
	while (i--) {
		*p++ = *payload++;
	}
	p = packet->frame.transmitRequest.reserved;
	*p++ = TRANSMIT_REQUEST_RESERVED_H;
	*p++ = TRANSMIT_REQUEST_RESERVED_L;
	packet->frame.transmitRequest.bcastRadious = radious;
	packet->frame.transmitRequest.options = options;
	packet->apiId = TRANSMIT_REQUEST;
	packet->frame.transmitRequest.frameId = frameId;
	packet->length = TRANSMIT_REQUEST_COMMAND_MINLENGTH + dataLength;
}

/*..........................................................................*/

void XBee_createExplicitAddressingPacket(XBeePacket* packet, UINT8 frameId,
		UINT8* destinationAddress, UINT8 sourceEndpoint,
		UINT8 destinationEndpoint, UINT8* clusterId, UINT8* profileId,
		UINT8 radious, UINT8 options, UINT8* payload, UINT8 dataLength) {
	UINT8* p = packet->frame.explicitAddressing.destinationAddress;
	UINT8 i = MAC_ADDRESS_BYTES_LENGTH;
	while (i--) {
		*p++ = *destinationAddress++;
	}
	p = packet->frame.explicitAddressing.payload;
	i = dataLength;
	while (i--) {
		*p++ = *payload++;
	}
	p = packet->frame.explicitAddressing.reserved;
	*p++ = TRANSMIT_REQUEST_RESERVED_H;
	*p++ = TRANSMIT_REQUEST_RESERVED_L;
	packet->frame.explicitAddressing.sourceEndpoint = sourceEndpoint;
	packet->frame.explicitAddressing.destinationEndpoint = destinationEndpoint;
	p = packet->frame.explicitAddressing.clusterId;
	*p++ = *clusterId++;
	*p++ = *clusterId++;
	p = packet->frame.explicitAddressing.profileId;
	*p++ = *profileId++;
	*p++ = *profileId++;
	packet->frame.explicitAddressing.bcastRadious = radious;
	packet->frame.explicitAddressing.options = options;
	packet->apiId = TRANSMIT_REQUEST;
	packet->frame.explicitAddressing.frameId = frameId;
	packet->length = EXPLICIT_ADDRESS_COMMAND_MINLENGTH + dataLength;
}

/*..........................................................................*/

void XBee_createRemoteAtCommandPacket(XBeePacket* packet, UINT8 frameId,
		UINT8* command, UINT8 param, UINT8* destinationAddress,
		UINT8 options) {
	UINT8* p = packet->frame.remoteAtCommand.command;
	UINT8 i = MAC_ADDRESS_BYTES_LENGTH;
	*p++ = *command++;
	*p++ = *command;
	p = packet->frame.remoteAtCommand.destinationAddress;
	while (i--) {
		*p++ = *destinationAddress++;
	}
	p = packet->frame.explicitAddressing.reserved;
	*p++ = TRANSMIT_REQUEST_RESERVED_H;
	*p++ = TRANSMIT_REQUEST_RESERVED_L;
	packet->frame.remoteAtCommand.parameter = param;
	packet->frame.remoteAtCommand.options = options;
	packet->apiId = AT_COMMAND;
	packet->frame.remoteAtCommand.frameId = frameId;
	packet->length = REMOTE_AT_COMMAND_MINLENGTH;
}


/*..........................................................................*/

/* RX METHODS */
BOOL XBee_readAtCommandResponsePacket(XBeePacket* packet, UINT8* frameId,
		UINT8** command, UINT8* status, UINT8* value) {
	UINT8* p;
	if (packet->apiId != AT_COMMAND_RESPONSE) {
		return FALSE;
	}
	if (frameId) {
		*frameId = packet->frame.atCommandResponse.frameId;
	}
	if (command) {
		*command = packet->frame.atCommandResponse.command;
	}
	if (status) {
		*status = packet->frame.atCommandResponse.status;
	}
	if (value) {
		UINT8 *dataPtr = packet->frame.atCommandResponse.value;
		int i;
		int datalength = packet->length - 5;
		*value = 0;
		for (i = 0; i < datalength; i++) {
			*value <<= 8;
			*value += *dataPtr++;
		}
	}
	p = packet->frame.explicitAddressing.reserved;
	*p++ = TRANSMIT_REQUEST_RESERVED_H;
	*p++ = TRANSMIT_REQUEST_RESERVED_L;
	return TRUE;
}

/*..........................................................................*/

BOOL XBee_readModemStatusPacket(XBeePacket* packet, UINT8* status) {
	UINT8* p;
	if (packet->apiId != MODEM_STATUS) {
		return FALSE;
	}
	if (status) {
		*status = packet->frame.modemStatus.status;
	}
	p = packet->frame.explicitAddressing.reserved;
	*p++ = TRANSMIT_REQUEST_RESERVED_H;
	*p++ = TRANSMIT_REQUEST_RESERVED_L;
	return TRUE;
}

/*..........................................................................*/

BOOL XBee_readTransmitStatusPacket(XBeePacket* packet, UINT8* frameId,
		UINT8* retryCount, UINT8* deliveryStatus, UINT8* discoveryStatus) {
	UINT8* p;
	if (packet->apiId != TRANSMIT_STATUS) {
		return FALSE;
	}
	if (frameId) {
		*frameId = packet->frame.transmitStatus.frameId;
	}
	if (retryCount) {
		*retryCount = packet->frame.transmitStatus.retryCount;
	}
	if (deliveryStatus) {
		*deliveryStatus = packet->frame.transmitStatus.deliveryStatus;
	}
	if (discoveryStatus) {
		*discoveryStatus = packet->frame.transmitStatus.discoveryStatus;
	}
	p = packet->frame.transmitStatus.reserved;
	*p++ = TRANSMIT_REQUEST_RESERVED_H;
	*p++ = TRANSMIT_REQUEST_RESERVED_L;
	return TRUE;
}

/*..........................................................................*/

BOOL XBee_readReceivePacket(XBeePacket* packet, UINT8* frameId,
		UINT8** sourceAddress, UINT8* options, UINT8** payload,
		UINT8* length) {
	UINT8* p;
	if (packet->apiId != RECEIVE_PACKET) {
		return FALSE;
	}
	if (frameId) {
		*frameId = packet->frame.receivePacket.frameId;
	}
	if (sourceAddress) {
		*sourceAddress = (UINT8*)packet->frame.receivePacket.sourceAddress;
	}
	if (options) {
		*options = packet->frame.receivePacket.options;
	}
	if (payload) {
		*payload = packet->frame.receivePacket.payload;
	}
	if (length) {
		*length = packet->length - 11;
	}

}

/*..........................................................................*/

BOOL XBee_readRemoteCommandResponsePacket(XBeePacket* packet, UINT8* frameId,
		UINT8** sourceAddress, UINT8** command, UINT8* status, UINT8* value) {
	UINT8* p;
	if (packet->apiId != REMOTE_COMMAND_RESPONSE) {
		return FALSE;
	}
	if (frameId) {
		*frameId = packet->frame.remoteCommandResponse.frameId;
	}
	if (sourceAddress) {
		*sourceAddress = packet->frame.remoteCommandResponse.sourceAddress;
	}
	if (command) {
		*command = (UINT8*) packet->frame.remoteCommandResponse.command;
	}
	if (status) {
		*status = packet->frame.remoteCommandResponse.status;
	}
	if (value) {
		UINT8 *dataPtr = packet->frame.remoteCommandResponse.value;
		int i;
		int datalength = packet->length - 5;
		*value = 0;
		for (i = 0; i < datalength; i++) {
			*value <<= 8;
			*value += *dataPtr++;
		}
	}
	p = packet->frame.explicitAddressing.reserved;
	*p++ = TRANSMIT_REQUEST_RESERVED_H;
	*p++ = TRANSMIT_REQUEST_RESERVED_L;
	return TRUE;
}