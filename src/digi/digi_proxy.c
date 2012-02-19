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

#include "digi_proxy.h"

/*..........................................................................*/

/* XBee proxy constructor */
void XBeeProxy_create(XBeeProxy * const proxy, Serial * const serial, XBee * const xbee) {
    proxy->serial = serial;
    proxy->xbee = xbee;
}

/*..........................................................................*/

/* Send XBee packet */
boolean XBeeProxy_sendPacket(XBeeProxy * const proxy, XBeePacket * const packet) {
    uint8_t* p = (uint8_t*) packet;
    Serial_send(proxy->serial, START_DELIMITER);
    // send the most significant bit
    Serial_send(proxy->serial, (packet->length >> 8) & 0xFF);
    // then the LSB
    Serial_send(proxy->serial, packet->length & 0xFF);
    // just in case it hasn't been initialized.
    packet->checksum = 0;
    //Generalizando para cualquier paquete
    while (packet->length--) {
        Serial_send(proxy->serial, *p);
        packet->checksum += *p++;
    }
    Serial_send(proxy->serial, (0xFF - packet->checksum));
    return true;
}

/*..........................................................................*/

/* Read XBee packet(generalized)*/
boolean XBeeProxy_readPacket(XBeeProxy * const proxy, XBeePacket * const packet) {
    uint8_t data;
    XBee_resetPacket(packet);
    while (Serial_available(proxy->serial)) {
        data = Serial_read(proxy->serial);
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
                    return false; //LENGTH Error
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
                    return true; //Everything OK!
                } else {
                    return false; //CRC Error
                }
        }
    }
    return false;
}

/*..........................................................................*/

/* Read atCommandResponse */
XBeeProxy_readAtCommandResponsePacket(XBeePacket* packet, uint8_t* frameId,
        uint8_t** command, uint8_t* status, uint8_t* value) {

}
