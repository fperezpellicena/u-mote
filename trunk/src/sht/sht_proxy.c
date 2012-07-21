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

#include "sht_proxy.h"
#include "sensor_proxy.h"
#include "sht.h"

static Sht sht;

/*...........................................................................*/
void ShtProxy_init(void) {
    Sensor_create(&sht.sensor, (Sense)&ShtProxy_sense,
            (CheckAlert)&ShtProxy_checkAlert);
    SensorProxy_add(&sht.sensor);
    Sht11_init();
}

/*...........................................................................*/
void ShtProxy_sense(List* measures) {
    // Measure
    Sht11_measure(&sht);
    // Put measures into payload
    List_add(measures, sht.data.humidity.i);
    List_add(measures, sht.data.temperature.i);
}

/*...........................................................................*/
BOOL ShtProxy_checkAlert(List* measures) {
    ShtProxy_sense(measures);
    if(sht.data.temperature.i >= SHT11_TMP_THR ){
        return TRUE;
    }
    return FALSE;
}
