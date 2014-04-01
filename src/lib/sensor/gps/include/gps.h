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

/*...........................................................................*/
/* Gps class */
typedef struct Gps Gps;

struct Gps {
    UINT8 id;
};

#define DECLARE_GPS(id, name) Gps name = {id}

void Gps_init(void);

#endif
