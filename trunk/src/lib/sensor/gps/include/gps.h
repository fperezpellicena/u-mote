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

#ifndef gps_h
#define gps_h

#include "GenericTypeDefs.h"
#include "nmea_command.h"
#include "nmea_output.h"
#include <rtcc.h>

/*...........................................................................*/
enum Longitude {
    NORTH = 'N', SOUTH = 'S'
};

enum Latitude {
    EAST = 'E', WEST = 'W'
};

enum LocationStatus {
    VALID = 'A', INVALID = 'V'
};

/*...........................................................................*/
/* Location coordinates class */
typedef struct Location Location;

struct Location {
    UINT8 longitudeCoordinate[9];
    UINT8 longitude;
    UINT8 latitudeCoordinate[9];
    UINT8 latitude;
};

/*...........................................................................*/
/* Gps class */
typedef struct Gps Gps;

struct Gps {
    UINT8 id;
    Location lastValidLocation;
    rtccTimeDate lastValidTimestamp;
};

#define DECLARE_GPS(id, name) Gps name = {id}

void Gps_init(void);

void Gps_sendPacket(NMEACommandPacket* packet);

BOOL Gps_readPacket(NMEAOutputPacket* packet);

BOOL Gps_readByte(NMEAOutputPacket* packet);

void Gps_readLocation(Gps* gps, NMEAOutputPacket* packet);

#endif
