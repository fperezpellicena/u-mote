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
 #include "digi_serial.h"

/* Read XBee packet(generalized)*/
BOOL XBeeProxy_readPacket(XBeePacket * const packet) {
    UINT8 data;
    XBee_resetPacket(packet);
    while (XBeeSerial_available()) {
        data = XBeeSerial_read();
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
                // in case we somehow get some garbage
                if (packet->length > MAX_PACKET_SIZE) {
                    packet->rxState = XBEE_PACKET_RX_START;
                    return FALSE; //LENGTH Error
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
                    return TRUE; //Everything OK!
                } else {
                    return FALSE; //CRC Error
                }
        }
    }
    return FALSE;
}
