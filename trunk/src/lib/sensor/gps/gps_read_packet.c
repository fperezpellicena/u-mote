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

#include "nmea_output.h"
#include "nmea_command.h"
#include "hw_serial.h"

static void Gps_processData(NMEAOutputPacket* packet, UINT8 data);

static void Gps_processData(NMEAOutputPacket* packet, UINT8 data) {
    switch (packet->rxState) {
	case NMEA_PACKET_PREAMBLE: //$
	    if (data == NMEA_PREAMBLE) {
		packet->rxState = NMEA_PACKET_DATA;
	    }
	    break;
	case NMEA_PACKET_DATA:
	    packet->data[packet->length++] = data;
	    // Checksum is x'or-ed
	    packet->chkCalculated ^= data;
	    if (data == NMEA_CHK_CHAR) {
		packet->rxState = NMEA_PACKET_CRC_1;
	    }
	    break;
	case NMEA_PACKET_CRC_1:
	    // High chk byte
	    packet->chkRead = (data << 8);
	    packet->rxState = NMEA_PACKET_CRC_2;
	    break;
	case NMEA_PACKET_CRC_2:
	    // Low chk byte
	    packet->chkRead += data;
	    if (packet->chkRead == packet->chkCalculated) {
		packet->rxState = NMEA_PACKET_OK;
	    }
	    packet->rxState = NMEA_PACKET_OK;
	    break;
    }
}

void Gps_readPacket(NMEAOutputPacket* packet) {
    NMEAOutput_resetPacket(packet);
    while (Serial_available()) {
	// FIXME This call is suposed to be faster than uart receiving next byte
	Gps_processData(packet, Serial_read());
    }
}

void Gps_readByte(NMEAOutputPacket* packet) {
    UINT8 data = Serial_read();
    Gps_processData(packet, data);
    Serial_ackInterrupt();
}
