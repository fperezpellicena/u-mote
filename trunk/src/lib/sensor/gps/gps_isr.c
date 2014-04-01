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

#include "gps_isr.h"
#include "gps_packet.h"
#include "gps_location.h"
#include "gps_timestamp.h"

#include "rtc.h"

#include "nmea_command.h"
#include "gps.h"

BOOL gpsPacketReceived = FALSE;

static NMEAOutputPacket packet;

static Location location;

void GpsInterrupt_init(void) {
    NMEAOutput_resetPacket(&packet);
    Gps_initLocation(&location);
}

void GpsInterrupt_readByte(void) {
    packet.data[packet.length++] = RCREG2;
    if (GpsInterrupt_packetReceived() == TRUE) {
        gpsPacketReceived = TRUE;
    }
}

/* Check whether a valid gps packet has been received */
BOOL GpsInterrupt_packetReceived(void) {
    return packet.length > 3
            && packet.data[packet.length - 3] == NMEA_CHK_CHAR;
}

/** Check for valid nmea frame and update last valid location and time */
void GpsInterrupt_process(void) {
    Gps_readPacket(&packet);
    if (Gps_isValidPacket(&packet)) {
        if (Gps_isValidLocation(&packet)) {
            Gps_readLocation(&packet, &location);
            Gps_readTimestamp(&packet, &(location.timestamp));
        }
    }
    NMEAOutput_resetPacket(&packet);
    gpsPacketReceived = FALSE;
}

/** Add location and timestamp to payload */
void GpsInterrupt_addInfoToPayload(Payload* payload) {
    Gps_addLocationToPayload(payload, &location);
    Rtc_addTimeToPayload(payload, &(location.timestamp));
}