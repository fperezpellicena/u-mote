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

#ifndef sensor_proxy_h
#define sensor_proxy_h

#include "sensor.h"
#include "list.h"

/*..........................................................................*/
/* Sensor vector */
typedef struct SensorVector SensorVector;

struct SensorVector {
    Sensor sensors[MAX_SENSORS];
    UINT8 size;
};

/*..........................................................................*/
void SensorProxy_init(void);

/*..........................................................................*/
void SensorProxy_add(Sensor* sensor);

/*..........................................................................*/
void SensorProxy_sense(void);

/*..........................................................................*/
BOOL SensorProxy_alert(void);

/*..........................................................................*/
UINT8 SensorProxy_getSensorByte(void);

/*..........................................................................*/
List* SensorProxy_getMeasures(void);

/*..........................................................................*/
void SensorProxy_powerOn(void);

/*..........................................................................*/
void SensorProxy_powerOff(void);

#endif /* sensor_proxy_h*/

