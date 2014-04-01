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

#include "gps_packet.h"
#include "nmea_command.h"
#include "nmea_output.h"
#include <stdlib.h>
#include <string.h>

BOOL Gps_isValidPacket(NMEAOutputPacket* packet) {
    return packet->rxState == NMEA_PACKET_OK;
}

void Gps_readPacket(NMEAOutputPacket* packet) {
    UINT8 data;
    UINT8 chkString[3];
    UINT8 length = 0;
    while(length != packet->length) {
        data = packet->data[length++];
        switch (packet->rxState) {
            case NMEA_PACKET_PREAMBLE: //$
                if (data == NMEA_PREAMBLE) {
                    packet->rxState = NMEA_PACKET_DATA;
                }
                break;
            case NMEA_PACKET_DATA:
                if (data == NMEA_CHK_CHAR) {
                    packet->rxState = NMEA_PACKET_CRC_1;
                } else {
                    // Checksum is x'or-ed
                    packet->chkCalculated ^= data;
                }
                break;
            case NMEA_PACKET_CRC_1:
                // High chk byte
                packet->chkRead[0] = data;
                packet->rxState = NMEA_PACKET_CRC_2;
                break;
            case NMEA_PACKET_CRC_2:
                // Low chk byte
                packet->chkRead[1] = data;
                if (strcmp(itoa(packet->chkCalculated, (char*)chkString), (char*)packet->chkRead)) {
                    packet->rxState = NMEA_PACKET_CR;
                } else {
                    packet->rxState = NMEA_PACKET_ERROR;
                }
                break;
            case NMEA_PACKET_CR:
                packet->rxState = NMEA_PACKET_LF;
                break;
            case NMEA_PACKET_LF:
                packet->rxState = NMEA_PACKET_OK;
                break;
            default:
                packet->rxState = NMEA_PACKET_PREAMBLE;
        }
    }
}
