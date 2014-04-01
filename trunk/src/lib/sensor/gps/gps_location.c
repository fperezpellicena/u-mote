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

#include "gps_location.h"
#include "nmea_output.h"
#include "rtc.h"
#include <string.h>

#define STATUS_INDICATOR_INDEX      18
#define LATITUDE_INDEX              30
#define LONGITUDE_INDEX             43
#define LATITUDE_COORD_INDEX        20
#define LONGITUDE_COORD_INDEX       32

void Gps_initLocation(Location* location) {
    location->latitude = 'U';
    location->longitude = 'U';
    location->timestamp.f.hour = 1;
    location->timestamp.f.mday = 2;
    location->timestamp.f.min = 3;
    location->timestamp.f.mon = 4;
    location->timestamp.f.sec = 6;
    location->timestamp.f.year = 8;
    memset(location->latitudeCoordinate, 'A', NMEA_COORD_LENGTH);
    memset(location->longitudeCoordinate, 'B', NMEA_COORD_LENGTH);
    location->latitudeCoordinate[NMEA_COORD_LENGTH - 1] = '\0';
    location->longitudeCoordinate[NMEA_COORD_LENGTH - 1] = '\0';
}

BOOL Gps_isValidLocation(NMEAOutputPacket* packet) {
    return packet->data[STATUS_INDICATOR_INDEX] == VALID;
}

void Gps_readLocation(NMEAOutputPacket* packet, Location* location) {
    location->latitude = packet->data[LATITUDE_INDEX];
    strncpy((char *) location->latitudeCoordinate,
            (char *) &packet->data[LATITUDE_COORD_INDEX],
            (size_t) NMEA_COORD_LENGTH - 1);
    location->longitude = packet->data[LONGITUDE_INDEX];
    strncpy((char *) location->longitudeCoordinate,
            (char *) &packet->data[LONGITUDE_COORD_INDEX],
            (size_t) NMEA_COORD_LENGTH - 1);
}

void Gps_addLocationToPayload(Payload* payload, Location* location) {
    Payload_putRAMString(payload, (UINT8*)location->longitudeCoordinate);
    Payload_putByte(payload, location->longitude);
    Payload_putRAMString(payload, (UINT8*)location->latitudeCoordinate);
    Payload_putByte(payload, location->latitude);
}