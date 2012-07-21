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

#include "ircA1_proxy.h"
#include "sensor.h"
#include "sensor_proxy.h"
#include "bsp.h"
#include "hw_adc.h"

static float ircaExpCal[] = {-0.058, -0.199, -0.698};  // e^(-b*x^c)

static IrcA1 ircA1;

/*...........................................................................*/
/* Init ADC and I/O */
void IrcA1Proxy_init(void) {
    Sensor_create(&ircA1.sensor, (Sense)&IrcA1Proxy_sense,
            (CheckAlert)&IrcA1Proxy_checkAlert);
    SensorProxy_add(&ircA1.sensor);
    Adc_init();
    IrcA1_init();
}

/*...........................................................................*/
void IrcA1Proxy_calibrateZero(void) {
    // Measure
    IrcA1Proxy_measure();
    // Calculate zero after 20-30 minutes powered in gas absence
    ircA1.cal.zero = ircA1.data.act / ircA1.data.ref;
}

/*...........................................................................*/
void IrcA1Proxy_calibrateSpan(void) {
    UINT8 abs;
    // Measure
    IrcA1Proxy_measure();
    // Absorbance
    abs = 1 - (ircA1.data.act / (ircA1.data.act * ircA1.cal.zero));
    // Calculate span
    ircA1.cal.span = abs / 1 - ircaExpCal[IRCA1_MODEL];
}

/*...........................................................................*/
void IrcA1Proxy_measure(void) {
    // Sense
    ircA1.data.ref = Adc_convert(IRCA1_REF);
    ircA1.data.act = Adc_convert(IRCA1_ACT);
    ircA1.data.tmp = Adc_convert(IRCA1_TMP);
}

/*...........................................................................*/
void IrcA1Proxy_sense(List* measures) {
    // Measure values
    IrcA1Proxy_measure();
    // Calculate CO2
    IrcA1_calculate(&ircA1);
    // Put data into measures
    List_add(measures, (UINT8)ircA1.data.x);   // FIXME float to int routine
}

/*...........................................................................*/
BOOL IrcA1Proxy_checkAlert(List* measures) {
    IrcA1Proxy_sense(measures);
    if(ircA1.data.x >= IRCA1_X_THR){
        return TRUE;
    }
    return FALSE;
}
