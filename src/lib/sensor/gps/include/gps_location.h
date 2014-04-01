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

#ifndef gps_location_h
#define gps_location_h

#include "bsp.h"
#include "nmea_output.h"
#include "payload.h"
#include <rtcc.h>

#define NMEA_COORD_LENGTH	    10	          /* NMEA coordinates length */

/*...........................................................................*/
#define NORTH   'N'
#define SOUTH   'S'
#define EAST    'E'
#define WEST    'W'
#define VALID   'A'
#define INVALID 'V'

/*...........................................................................*/
/* Location coordinates class */
typedef struct Location Location;

struct Location {
    UINT8 longitudeCoordinate[NMEA_COORD_LENGTH];
    UINT8 longitude;
    UINT8 latitudeCoordinate[NMEA_COORD_LENGTH];
    UINT8 latitude;
    rtccTimeDate timestamp;
};

void Gps_initLocation(Location* location);

BOOL Gps_isValidLocation(NMEAOutputPacket* packet);

void Gps_readLocation(NMEAOutputPacket* packet, Location* location);

void Gps_addLocationToPayload(Payload* payload, Location* location);

#endif