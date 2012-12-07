
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