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

/* Send XBee packet */
BOOL XBeeProxy_sendPacket(XBeePacket * const packet) {
    UINT8* p = (UINT8*) packet;
    XBeeSerial_send(START_DELIMITER);
    // send the most significant bit
    XBeeSerial_send((packet->length >> 8) & 0xFF);
    // then the LSB
    XBeeSerial_send(packet->length & 0xFF);
    // just in case it hasn't been initialized.
    packet->checksum = 0;
    //Generalizando para cualquier paquete
    while (packet->length--) {
        XBeeSerial_send(*p);
        packet->checksum += *p++;
    }
    XBeeSerial_send((0xFF - packet->checksum));
    // Bugfix: Flush buffer
    XBeeSerial_send(NULL);
    return TRUE;
}