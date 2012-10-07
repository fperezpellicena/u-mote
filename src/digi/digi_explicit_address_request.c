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