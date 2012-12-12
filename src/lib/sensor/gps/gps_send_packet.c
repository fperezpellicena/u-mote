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

#include "nmea_command.h"
#include "hw_serial.h"

void Gps_sendPacket(NMEACommandPacket* packet) {
    UINT8 i;
    // Send $
    Serial_send(NMEA_PREAMBLE);
    // Send NMEA id
    Serial_sendROMArray((UINT8 rom*)NMEA_ID, NMEA_ID_LENGTH);
    // Send command number
    Serial_sendArray(packet->commandNumber, NMEA_COMMAND_LENGTH);
    //Generalized
    for (i = 0; i < packet->length; i++) {
	Serial_send(packet->data[i]);
    }
    // Send checksum
    Serial_send(NMEA_CHK_CHAR);
    Serial_sendArray(packet->checksumString, NMEA_CHECKUM_LENGTH);
    // Send CR and LF
    Serial_send(NMEA_CR);
    Serial_send(NMEA_LF);
}
