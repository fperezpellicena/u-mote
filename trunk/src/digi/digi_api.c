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

#include "bsp.h"
#include "digi_api.h"
#include <string.h>

void XBee_resetPacket(XBeePacket * const packet) {
    packet->dataPtr = (UINT8*) packet;
    packet->checksum = 0;
    packet->rxState = XBEE_PACKET_RX_START;
    packet->length = 0;
    packet->index = 0;
    packet->apiId = 0;
    memset(packet->frame.payload, 0, MAX_PAYLOAD);
}

UINT8 XBee_escape(UINT8 value) {
    if (value == START_DELIMITER || value == XON
            || value == XOFF || value == ESCAPE) {
        return value ^ 0x20;
    }
    return value;
}
