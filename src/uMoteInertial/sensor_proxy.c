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
#include "bsp_inertial.h"
#include "gps.h"
#include "gps_api.h"
#include "adxl.h"

DECLARE_GPS(GPS_ID, gps);

DECLARE_ADXL(ADXL_ID, adxl);

static UINT8 sensorIdentifiers;

static void SensorProxy_composeSensorIdentifiers(void);

/*..........................................................................*/
void SensorProxy_init(void) {
    Gps_init();
    Gps_setOutputConfig();
}

/*..........................................................................*/
void SensorProxy_sense(void) {
    return;
}

/*..........................................................................*/
void SensorProxy_addSensorIdentifiersToPayload(Payload* payload) {
    Payload_putByte(payload, 0);
    Payload_putByte(payload, sensorIdentifiers);
    Payload_putByte(payload, 0);
    Payload_putByte(payload, 0);
}

/*..........................................................................*/
void SensorProxy_addMeasuresToPayload(Payload* payload) {
}

static void SensorProxy_composeSensorIdentifiers(void) {
    sensorIdentifiers = 0;
    sensorIdentifiers |= gps.id;
    sensorIdentifiers |= adxl.id;
}

