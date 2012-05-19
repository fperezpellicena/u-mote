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

#include "gps_proxy.h"
#include "gps_api.h"

void GpsProxy_create(GpsProxy * const proxy, Serial * const serial) {
    proxy->serial = serial;
}

BOOL GpsProxy_readOutput(GpsProxy * const proxy, NMEAOutput* packet) {
     UINT8 data;
    while (Serial_available(proxy->serial)) {
        data = Serial_read(proxy->serial);
        switch(packet->rxState) {
            case NMEA_PACKET_PREAMBLE:  //$
                if(data == NMEA_PREAMBLE) {
                    packet->rxState = NMEA_PACKET_DATA;
                }
                break;
            case NMEA_PACKET_DATA:
                packet->data[packet->length] = data;
                packet->length += 1;
                // Checksum is x'or-ed
                packet->chkCalculated ^= data;
                if(data == NMEA_CHK_CHAR) {
                    packet->rxState = NMEA_PACKET_CRC_1;
                }
                break;
            case NMEA_PACKET_CRC_1:
                // High chk byte
                packet->chkRead = (data << 8);
                break;
            case NMEA_PACKET_CRC_2:
                // Low chk byte
                packet->chkRead += data;
                if(packet->chkRead == packet->chkCalculated) {
                    return TRUE;
                }
                return FALSE;
                break;
        }
    }
}

void GpsProxy_sendCommand(GpsProxy * const proxy, NMEACommandPacket* packet) {

}

