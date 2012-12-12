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
#include "nmea_command.h"

DECLARE_GPS(GPS_ID, gps);

static NMEACommandPacket packet;

/** Enable RMC output, all other disabled */
static NMEAOutputConfig defaultNMEAOutput
	= {'0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};

static UINT8 sensorIdentifiers;

static void SensorProxy_composeSensorIdentifiers(void);

/*..........................................................................*/
void SensorProxy_init(void) {
    Gps_init();
    NMEACommand_createSetOutput(&packet, &defaultNMEAOutput);
    Gps_sendPacket(&packet);
}

/*..........................................................................*/
void SensorProxy_sense(void) {
    // Read packet
    Gps_readPacket(&packet);
    // Extract location
    Gps_readLocation(&gps, &packet);
    
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
}

