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
        *sourceAddress = (UINT8*) packet->frame.receivePacket.sourceAddress;
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
	return TRUE;
}