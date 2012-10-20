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

#include "payload.h"

/*..........................................................................*/
void SensorProxy_init(void);

/*..........................................................................*/
void SensorProxy_sense(void);

/*..........................................................................*/

#if SENSING_MODE == FUZZY_DRIVEN
    /* Measure sensor board and check for alert condition */
    UINT8 SensorProxy_fuzzy(void);
#endif

/*..........................................................................*/
void SensorProxy_addSensorIdentifiersToPayload(Payload* payload);

/*..........................................................................*/
void SensorProxy_addMeasuresToPayload(Payload* payload);


#endif /* sensor_proxy_h*/

