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