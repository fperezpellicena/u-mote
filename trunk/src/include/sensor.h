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

#ifndef sensor_h
#define sensor_h

#include "GenericTypeDefs.h"
#include "bsp.h"
#include "list.h"

/*..........................................................................*/
/* Sense function prototype */
typedef UINT8 rom (*Sense)(List* list);

/*..........................................................................*/
/* Check alert function prototype */
typedef BOOL rom (*CheckAlert)(List* list);

/*..........................................................................*/
/* Sensor class */
typedef struct Sensor Sensor;

struct Sensor {
    /* Sense function */
    Sense sense;
    /* Check alert function */
    CheckAlert checkAlert;
};

/*..........................................................................*/
void Sensor_create(Sensor* sensor, Sense senseFunction,
        CheckAlert checkAlertFunction);

#endif /* sensor_h */
