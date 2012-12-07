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