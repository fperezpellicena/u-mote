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

//FIXME Revisar las longitudes de los paquetes recibidos
/*..........................................................................*/

/* TX METHODS */

void XBee_CreateATCommandPacket(XBeePacket* packet, uint8_t frameId,
		uint8_t* command, uint8_t* params, uint8_t length) {
	uint8_t* p = packet->frame.atCommand.command;
	*p++ = *command++;
	*p++ = *command;
	p = packet->frame.atCommand.value;
	while (length--) {
		*p++ = *params++;
	}
	packet->apiId = AT_COMMAND;
	packet->frame.atCommand.frameId = frameId;
}

/*..........................................................................*/

void XBee_CreateTransmitRequestPacket(XBeePacket* packet, uint8_t frameId,
		uint8_t* destinationAddress, uint8_t radious, uint8_t options,
		uint8_t* payload, uint8_t length) {
	uint8_t* p = packet->frame.transmitRequest.destinationAddress;
	uint8_t i = 0;
	while (i < MAC_ADDRESS_BYTES_LENGTH) {
		*p++ = *destinationAddress++;
	}
	p = packet->frame.transmitRequest.payload;
	while (length--) {
		*p++ = *payload++;
	}
	p = packet->frame.transmitRequest.reserved;
	*p++ = TRANSMIT_REQUEST_RESERVED_H;
	*p++ = TRANSMIT_REQUEST_RESERVED_L;
	packet->frame.transmitRequest.bcastRadious = radious;
	packet->frame.transmitRequest.options = options;
	packet->apiId = TRANSMIT_REQUEST;
	packet->frame.transmitRequest.frameId = frameId;
}

/*..........................................................................*/

void XBee_CreateExplicitAddressingPacket(XBeePacket* packet, uint8_t frameId,
		uint8_t* destinationAddress, uint8_t sourceEndpoint,
		uint8_t destinationEndpoint, uint8_t clusterId, uint8_t profileId,
		uint8_t radious, uint8_t options, uint8_t* payload, uint8_t length) {
	uint8_t* p = packet->frame.explicitAddressing.destinationAddress;
	uint8_t i = 0;
	while (i < MAC_ADDRESS_BYTES_LENGTH) {
		*p++ = *destinationAddress++;
	}
	p = packet->frame.explicitAddressing.payload;
	while (length--) {
		*p++ = *payload++;
	}
	p = packet->frame.explicitAddressing.reserved;
	*p++ = TRANSMIT_REQUEST_RESERVED_H;
	*p++ = TRANSMIT_REQUEST_RESERVED_L;
	packet->frame.explicitAddressing.sourceEndpoint = sourceEndpoint;
	packet->frame.explicitAddressing.destinationEndpoint = destinationEndpoint;
	packet->frame.explicitAddressing.clusterId = clusterId;
	packet->frame.explicitAddressing.profileId = profileId;
	packet->frame.explicitAddressing.bcastRadious = radious;
	packet->frame.explicitAddressing.options = options;
	packet->apiId = TRANSMIT_REQUEST;
	packet->frame.explicitAddressing.frameId = frameId;
}

/*..........................................................................*/

void XBee_CreateRemoteAtCommandPacket(XBeePacket* packet, uint8_t frameId,
		uint8_t* command, uint8_t param, uint8_t* destinationAddress,
		uint8_t options) {
	uint8_t i = 0;
	uint8_t* p = packet->frame.remoteAtCommand.command;
	*p++ = *command++;
	*p++ = *command;
	p = packet->frame.remoteAtCommand.destinationAddress;
	while (i < MAC_ADDRESS_BYTES_LENGTH) {
		*p++ = *destinationAddress++;
	}
	p = packet->frame.explicitAddressing.reserved;
	*p++ = TRANSMIT_REQUEST_RESERVED_H;
	*p++ = TRANSMIT_REQUEST_RESERVED_L;
	packet->frame.remoteAtCommand.parameter = param;
	packet->frame.remoteAtCommand.options = options;
	packet->apiId = AT_COMMAND;
	packet->frame.remoteAtCommand.frameId = frameId;
}


/*..........................................................................*/

/* RX METHODS */
boolean XBee_ReadAtCommandResponsePacket(XBeePacket* packet, uint8_t* frameId,
		uint8_t** command, uint8_t* status, uint8_t* value) {
	uint8_t* p;
	if (packet->apiId != AT_COMMAND_RESPONSE) {
		return false;
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
		uint8_t *dataPtr = packet->frame.atCommandResponse.value;
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
	return true;
}

/*..........................................................................*/

boolean XBee_ReadModemStatusPacket(XBeePacket* packet, uint8_t* status) {
	uint8_t* p;
	if (packet->apiId != MODEM_STATUS) {
		return false;
	}
	if (status) {
		*status = packet->frame.modemStatus.status;
	}
	p = packet->frame.explicitAddressing.reserved;
	*p++ = TRANSMIT_REQUEST_RESERVED_H;
	*p++ = TRANSMIT_REQUEST_RESERVED_L;
	return true;
}

/*..........................................................................*/

boolean XBee_ReadTransmitStatusPacket(XBeePacket* packet, uint8_t* frameId,
		uint8_t* retryCount, uint8_t* deliveryStatus, uint8_t* discoveryStatus) {
	uint8_t* p;
	if (packet->apiId != TRANSMIT_STATUS) {
		return false;
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
	return true;
}

/*..........................................................................*/

boolean XBee_ReadReceivePacket(XBeePacket* packet, uint8_t* frameId,
		uint8_t** sourceAddress, uint8_t* options, uint8_t** payload,
		uint8_t* length) {
	uint8_t* p;
	if (packet->apiId != RECEIVE_PACKET) {
		return false;
	}
	if (frameId) {
		*frameId = packet->frame.receivePacket.frameId;
	}
	if (sourceAddress) {
		*sourceAddress = (uint8_t*)packet->frame.receivePacket.sourceAddress;
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

boolean XBee_ReadRemoteCommandResponsePacket(XBeePacket* packet, uint8_t* frameId,
		uint8_t** sourceAddress, uint8_t** command, uint8_t* status, uint8_t* value) {
	uint8_t* p;
	if (packet->apiId != REMOTE_COMMAND_RESPONSE) {
		return false;
	}
	if (frameId) {
		*frameId = packet->frame.remoteCommandResponse.frameId;
	}
	if (sourceAddress) {
		*sourceAddress = packet->frame.remoteCommandResponse.sourceAddress;
	}
	if (command) {
		*command = (uint8_t*) packet->frame.remoteCommandResponse.command;
	}
	if (status) {
		*status = packet->frame.remoteCommandResponse.status;
	}
	if (value) {
		uint8_t *dataPtr = packet->frame.remoteCommandResponse.value;
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
	return true;
}