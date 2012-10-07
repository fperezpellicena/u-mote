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

#ifndef digi_api_h
#define digi_api_h

#include "GenericTypeDefs.h"

/*..........................................................................*/

/* CONSTANTS */
/*..........................................................................*/
#define START_DELIMITER                         0x7E
#define MAC_ADDRESS_BYTES_LENGTH                0x08
/*..........................................................................*/
#define MIN_PACKET_SIZE                         0x06
#define MAX_PACKET_SIZE                         0x64
#define AT_COMMAND_MINLENGTH                    0x04
#define TRANSMIT_REQUEST_COMMAND_MINLENGTH      0x0E
#define EXPLICIT_ADDRESS_COMMAND_MINLENGTH      0x14
#define REMOTE_AT_COMMAND_MINLENGTH             0x10
/*..........................................................................*/
#define XON                                     0x11
#define XOFF                                    0x13
#define ESCAPE                                  0x7D
/*..........................................................................*/
#define TRANSMIT_REQUEST_RESERVED_H             0xFF
#define TRANSMIT_REQUEST_RESERVED_L             0xFE
/*..........................................................................*/
#define AT_COMMAND_RESPONSE_OK                  0x00
#define AT_COMMAND_RESPONSE_ERROR               0x01
#define AT_COMMAND_RESPONSE_INVALID_COMMAND     0x02
#define AT_COMMAND_RESPONSE_INVALID_PARAMETER   0x03
/*..........................................................................*/
#define MODEM_STATUS_HARDWARE_RESET             0x00
#define MODEM_STATUS_WD_TIMER_RESET             0x01
#define MODEM_STATUS_NETWORK_WOKE_UP            0x0B
#define MODEM_STATUS_NETWORK_SLEEP              0x0C
/*..........................................................................*/
#define TX_STATUS_DELIVERY_SUCCESS              0x00
#define TX_STATUS_DELIVERY_MAC_ACK_FAILURE      0x01
#define TX_STATUS_DELIVERY_INVALID_ENDPOINT     0x15
#define TX_STATUS_DELIVERY_NETWORK_ACK_FAILURE  0x21
#define TX_STATUS_DELIVERY_ROUTE_NOT_FOUND      0x25
/*..........................................................................*/
#define TX_STATUS_DISCOVERY_NO_ROUTE_OVERHEAD   0x00
#define TX_STATUS_DISCOVERY_ROUTE_DISCOVERY     0x02
/*..........................................................................*/
#define PACKET_ACKNOWLEGED                      0x01
#define PACKET_BROADCAST                        0x02


/*..........................................................................*/

/* FRAME TYPES */
enum XBeePacketFrameType {
    AT_COMMAND = 0x08,
    AT_COMMAND_QUEUE = 0x09,
    TRANSMIT_REQUEST = 0x10,
    EXPLICIT_ADDRESSING = 0x11,
    REMOTE_AT_COMMAND = 0x17,
    AT_COMMAND_RESPONSE = 0x88,
    MODEM_STATUS = 0x8A,
    TRANSMIT_STATUS = 0x8B,
    RECEIVE_PACKET = 0x90,
    EXPLICIT_RX_INDICATOR = 0x91,
    REMOTE_COMMAND_RESPONSE = 0x97
};

/* RX STATES */
enum XBeePacketRxState {
    XBEE_PACKET_RX_START = 0,
    XBEE_PACKET_RX_LENGTH_1 = 1,
    XBEE_PACKET_RX_LENGTH_2 = 2,
    XBEE_PACKET_RX_PAYLOAD = 3,
    XBEE_PACKET_RX_CRC = 4
};

/*..........................................................................*/

/* AT COMMAND*/
typedef struct AtCommand AtCommand;

struct AtCommand {
    UINT8 frameId; /* Frame id */
    UINT8 command[2]; /* AT Command */
    UINT8 value[97]; /* Command value(optional)*/
};

/*..........................................................................*/

/* TRANSMIT REQUEST */
typedef struct TransmitRequest TransmitRequest;

struct TransmitRequest {
    UINT8 frameId; /* Frame id */
    UINT8 destinationAddress[8]; /* 64 bit destination address */
    UINT8 reserved[2];
    UINT8 bcastRadious; /* Broadcast radious */
    UINT8 options; /* Transmit options */
    UINT8 payload[97]; /* Data sent to the destination device */
};

/*..........................................................................*/

/* EXPLICIT ADDRESSING COMMAND FRAME */
typedef struct ExplicitAddressing ExplicitAddressing;

struct ExplicitAddressing {
    UINT8 frameId; /* Frame id */
    UINT8 destinationAddress[8]; /* 64 bit destination address */
    UINT8 reserved[2];
    UINT8 sourceEndpoint; /* Source endpoint for the transmission */
    UINT8 destinationEndpoint; /* Destination endpoint for the transmission */
    UINT8 clusterId[2]; /* Cluster ID used in the transmission */
    UINT8 profileId[2]; /* Profile ID used in the transmission */
    UINT8 bcastRadious; /* Broadcast radious */
    UINT8 options; /* Transmit options */
    UINT8 payload[97]; /* Data sent to the destination device */
};

/*..........................................................................*/

/* REMOTE AT COMMAND */
typedef struct RemoteAtCommand RemoteAtCommand;

struct RemoteAtCommand {
    UINT8 frameId; /* Frame id */
    UINT8 destinationAddress[8]; /* 64 bit destination address */
    UINT8 reserved[2];
    UINT8 options; /* Remote command options */
    UINT8 command[2]; /* AT command */
    UINT8 parameter; /* Command parameter */
};

/*..........................................................................*/

/* AT COMMAND RESPONSE */
typedef struct AtCommandResponse AtCommandResponse;

struct AtCommandResponse {
    UINT8 frameId; /* Frame id */
    UINT8 command[2]; /* AT command */
    UINT8 status; /* Command status */
    UINT8 value[97]; /* Command value(optional)*/
};

/*..........................................................................*/

/* MODEM STATUS */
typedef struct ModemStatus ModemStatus;

struct ModemStatus {
    UINT8 status; /* Modem status */
};

/*..........................................................................*/

/* TRANSMIT STATUS */
typedef struct TransmitStatus TransmitStatus;

struct TransmitStatus {
    UINT8 frameId; /* Frame ID */
    UINT8 reserved[2]; /* Reserved */
    UINT8 retryCount; /* Number of retransmissions */
    UINT8 deliveryStatus; /* Delivery status */
    UINT8 discoveryStatus; /* Discovery status */
};

/*..........................................................................*/

/* RECEIVE PACKET */
typedef struct ReceivePacket ReceivePacket;

struct ReceivePacket {
    UINT8 frameId; /* Frame id */
    UINT8 sourceAddress[8]; /* 64 bit source address */
    UINT8 reserved[2];
    UINT8 options; /* Receive options */
    UINT8 payload[97]; /* Data received */
};

/*..........................................................................*/

/* EXPLICIT RX INDICATOR */
typedef struct ExplicitRxIndicator ExplicitRxIndicator;

struct ExplicitRxIndicator {
    UINT8 frameId; /* Frame id */
    UINT8 sourceAddress[8]; /* 64 bit source address */
    UINT8 reserved[2];
    UINT8 sourceEndpoint; /* Source endpoint for the transmission */
    UINT8 destinationEndpoint; /* Destination endpoint for the transmission */
    UINT8 clusterId; /* Cluster ID used in the transmission */
    UINT8 profileId; /* Profile ID used in the transmission */
    UINT8 options; /* Receive options */
    UINT8 payload[97]; /* Data received */
};

/*..........................................................................*/

/* REMOTE COMMAND RESPONSE */
typedef struct RemoteCommandResponse RemoteCommandResponse;

struct RemoteCommandResponse {
    UINT8 frameId; /* Frame id */
    UINT8 sourceAddress[8]; /* 64 bit source(remote) address */
    UINT8 reserved[2];
    UINT8 command[2]; /* AT command */
    UINT8 status; /* AT command status */
    UINT8 value[97]; /* Command data */
};

/*..........................................................................*/

/* XBEE PACKET*/
typedef struct XBeePacket XBeePacket;

struct XBeePacket {
    UINT8 apiId; /* Frame type identifier */
    union {
        UINT8 payload[100];
        AtCommand atCommand;
        TransmitRequest transmitRequest;
	ExplicitAddressing explicitAddressing;
	RemoteAtCommand remoteAtCommand;
	AtCommandResponse atCommandResponse;
	ModemStatus modemStatus;
	TransmitStatus transmitStatus;
	ReceivePacket receivePacket;
	ExplicitRxIndicator explicitRxIndicator;
	RemoteCommandResponse remoteCommandResponse;
    }frame;
    UINT16 length; /* Number of bytes between the length and the checksum */
    UINT8 checksum; /* Checksum calculated as FF - sum(fields)*/
    UINT8 rxState; /* Reception state */
    UINT8 *dataPtr; /* Ponter to structure itself */
    UINT8 index;
    UINT8 lastByte;
};

/*..........................................................................*/
/* UTIL METHODS */

void XBee_resetPacket(XBeePacket * const packet);

UINT8 XBee_escape(UINT8 value);

/*..........................................................................*/

/* TX METHODS */

void XBee_createCompleteATCommandPacket(XBeePacket* packet, UINT8 frameId,
	UINT8* command,UINT8* params, UINT8 paramsLength);

void XBee_createATCommandPacket(XBeePacket* packet, UINT8 frameId,
		UINT8* command);

void XBee_createTransmitRequestPacket(XBeePacket* packet, UINT8 frameId,
	UINT8* destinationAddress, UINT8 radious, UINT8 options,
	UINT8* payload, UINT8 dataLength);

void XBee_createExplicitAddressingPacket(XBeePacket* packet, UINT8 frameId,
	UINT8* destinationAddress, UINT8 sourceEndpoint,
        UINT8 destinationEndpoint, UINT8* clusterId, UINT8* profileId,
        UINT8 radious, UINT8 options,UINT8* payload, UINT8 dataLength);

void XBee_createRemoteAtCommandPacket(XBeePacket* packet, UINT8 frameId,
	UINT8* command, UINT8 param, UINT8* destinationAddress,
        UINT8 options);

/*..........................................................................*/

/* RX METHODS */
BOOL XBee_readAtCommandResponsePacket(XBeePacket* packet, UINT8* frameId,
		UINT8** command, UINT8* status, UINT8* value);

BOOL XBee_readModemStatusPacket(XBeePacket* packet, UINT8* status);

BOOL XBee_readTransmitStatusPacket(XBeePacket* packet, UINT8* frameId,
        UINT8* retryCount, UINT8* deliveryStatus, UINT8* discoveryStatus);

BOOL XBee_readReceivePacket(XBeePacket* packet, UINT8* frameId,
        UINT8** sourceAddress, UINT8* options, UINT8** payload,
        UINT8* length);

BOOL XBee_readRemoteCommandResponsePacket(XBeePacket* packet,UINT8* frameId,
        UINT8** sourceAddress,UINT8** command, UINT8* status, UINT8* value);

#endif
