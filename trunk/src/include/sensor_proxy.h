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

#define DECLARE_SENSOR_VECTOR(name, ...)\
Sensor* name##sensors[SENSORS] = {__VA_ARGS__};\
SensorVector name = {SENSORS, name##sensors}

/*..........................................................................*/
/* Sensor vector */
typedef struct SensorVector SensorVector;

struct SensorVector {
    UINT8 size;
    Sensor* sensors[SENSORS];
};

/*..........................................................................*/
void SensorProxy_init(void);

///*..........................................................................*/
//void SensorProxy_add(Sensor* sensor);

/*..........................................................................*/
void SensorProxy_sense(void);

/*..........................................................................*/

/* Measure sensor board and check for alert condition */
UINT8 SensorProxy_fuzzy(void);

///*..........................................................................*/
//BOOL SensorProxy_alert(void);

/*..........................................................................*/
void SensorProxy_putSensors(List* list);

/*..........................................................................*/
List* SensorProxy_getMeasures(void);

/*..........................................................................*/
void SensorProxy_powerOn(void);

/*..........................................................................*/
void SensorProxy_powerOff(void);

/*..........................................................................*/
UINT8 SensorProxy_usbShtTemperature(char* usbOutBuffer);
UINT8 SensorProxy_usbShtHumidity(char* usbOutBuffer);

#endif /* sensor_proxy_h*/

