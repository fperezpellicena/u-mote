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

#include "sensor_proxy.h"

/* Installed sensors */
static SensorVector sensorVector;
static List measures;

/*..........................................................................*/
void SensorProxy_init(void) {
    List_init(&measures);
    sensorVector.size = 0;
    SENSORS_PWR_DDR = 0; /* Sensor power on/off pin as output */
}

/*..........................................................................*/

/* Adds a sensor to board */
void SensorProxy_add(Sensor* sensor) {
    if (sensorVector.size < MAX_SENSORS) {
        sensorVector.sensors[sensorVector.size].sense = sensor->sense;
        sensorVector.sensors[sensorVector.size].checkAlert = sensor->checkAlert;
        sensorVector.size++;
    }
}

/*..........................................................................*/

/* Turns on sensor board, sense every sensor installed and turns off */
void SensorProxy_sense(void) {
    UINT8 i;
    // Turn on sensor board
    SensorProxy_powerOn();
    // Empty previous measures
    List_empty(&measures);
    // For each sensor installed, put measures into payload
    for (i = 0; i < sensorVector.size; i++) {
        sensorVector.sensors[i].sense(&measures);
    }
    // Turn off sensor board
    SensorProxy_powerOff();
}

/*..........................................................................*/

/* Measure sensor board and check for alert condition */
BOOL SensorProxy_alert(void) {
    UINT8 i;
    BOOL alert = FALSE;
    if(sensorVector.size == 0) {
        return FALSE;
    }
    // Turn on sensor board
    SensorProxy_powerOn();
    // Empty previous measures
    List_empty(&measures);
    for (i = 0; i < sensorVector.size; i++) {
        if (sensorVector.sensors[i].checkAlert(&measures) == TRUE) {
            alert = TRUE;
        }
    }
    // Turn off sensor board
    SensorProxy_powerOff();
    return alert;
}

/*..........................................................................*/
/* Returns sensor byte identification based on sensor id attribute */
UINT8 SensorProxy_getSensorByte(void) {
    UINT8 byte = 0;
    UINT8 i;
    if(sensorVector.size == 0) {
        return 0;
    }
    for (i = 0; i < sensorVector.size; i++) {
        byte |= sensorVector.sensors[i].id; // Sensor id's OR'ed
    }
    return byte;
}

/*..........................................................................*/

/* Return measures */
List* SensorProxy_getMeasures(void) {
    return &measures;
}

/*...........................................................................*/

/* Power on sensor board */
void SensorProxy_powerOn(void) {
    SENSORS_PWR = 1;
}

/*...........................................................................*/

/* Power off sensor board */
void SensorProxy_powerOff(void) {
    SENSORS_PWR = 0;
}
