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
#include "fuzzy.h"
#include "sht.h"
#include "irca1.h"
#include "util.h"
#include "sht_proxy.h"
#include "irca1_proxy.h"

static List measures;

UINT8 Sht_sense(List* measures);

//static UINT8 Irca_sense(List* measures);

#pragma udata mf
DECLARE_MF(lowTemp, -40, 0, 20);
DECLARE_MF(midTemp, 0, 20, 40);
DECLARE_MF(highTemp, 30, 40, 255);

//DECLARE_MF(lowCo2, 0, 0, 50);
//DECLARE_MF(midCo2, 0, 50, 100);
//DECLARE_MF(highCo2, 50, 100, 100);

DECLARE_MF(lowRisk, 0, 0, 50);
DECLARE_MF(midRisk, 0, 50, 100);
DECLARE_MF(highRisk, 50, 100, 255);
#pragma udata

#pragma udata rp
DECLARE_RT(ifHighTemp, &highTemp);
DECLARE_RT(ifMidTemp, &midTemp);
DECLARE_RT(ifLowTemp, &lowTemp);
//DECLARE_RT(andHighCo2, &highCo2);
DECLARE_RT(thenHighRisk, &highRisk);
DECLARE_RT(thenMidRisk, &midRisk);
DECLARE_RT(thenLowRisk, &lowRisk);

//DECLARE_RT(andLowCo2, &lowCo2);
//DECLARE_RT(thenMidRisk, &midRisk);
#pragma udata

#pragma udata rule
DECLARE_RULE(ifHighTempThenHighRisk, &thenHighRisk, 1, &ifHighTemp);
DECLARE_RULE(ifMidTempThenMidRisk, &thenMidRisk, 1, &ifMidTemp);
DECLARE_RULE(ifLowTempThenLowRisk, &thenLowRisk, 1, &ifLowTemp);
//DECLARE_RULE(ifHighTempAndHighCo2ThenHighRisk, &thenHighRisk, 2, &ifHighTemp, &andHighCo2);
//DECLARE_RULE(ifHighTempAndlowCo2ThenMidRisk, &thenMidRisk, 2, &ifHighTemp, &andLowCo2);
#pragma udata

#pragma udata ruleEngine
DECLARE_ENGINE(engine, 3, &ifHighTempThenHighRisk,
        &ifMidTempThenMidRisk, &ifLowTempThenLowRisk);
//DECLARE_ENGINE(engine, 2, &ifHighTempAndHighCo2ThenHighRisk, &ifHighTempAndlowCo2ThenMidRisk);
#pragma udata

/* Declare one SHT sensor */
#pragma udata sensors
DECLARE_FUZZY_SHT(SHT_ID, sht, &Sht_sense, 1, &ifHighTemp);
//DECLARE_FUZZY_IRCA(IRCA1_ID, irca, &Irca_sense, 2, &andHighCo2, &andLowCo2);
Sensor* sensors[SENSORS];
#pragma udata

/*...........................................................................*/
UINT8 Sht_sense(List* measures) {
    // Measure
    Sht11_measure(&sht);
    List_add(measures, sht.data->temperature.i);
#if SHT_HUM_ENABLED
    // Put measures into payload
    List_add(measures, sht.data->humidity.i);
#endif
    // Return temperature
    return (UINT8) sht.data->temperature.f;
}

/*...........................................................................*/
//static UINT8 Irca_sense(List* measures) {
//    // Measure values
//    IrcA1Proxy_measure(&irca);
//    // Calculate CO2
//    IrcA1_calculate(&irca);
//    // Put data into measures
//    List_add(measures, round(irca.data->x)); // Float to int routine
//}

/*..........................................................................*/
void SensorProxy_init(void) {
    sensors[0] = sht.sensor;
    List_init(&measures);
    ShtProxy_init(); /* Init SHT resources */
    //    IrcA1Proxy_init(); /* Init IRCA resources */
    SensorProxy_powerOff(); /* Sensor power on/off pin as output */
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
    //sht.sensor->sense(&measures);
    for (i = 0; i < SENSORS; i++) {
        sensors[i]->sense(&measures);
    }
    // Turn off sensor board
    SensorProxy_powerOff();
}

/*..........................................................................*/

/* Measure sensor board and check for alert condition */
UINT8 SensorProxy_fuzzy(void) {
    UINT8 i;
    UINT8 j;
    UINT8 measure;
    // Turn on sensor board
    SensorProxy_powerOn();
    // Empty previous measures
    List_empty(&measures);
    //    measure = sht.sensor->sense(&measures);
    //    for (j = 0; j < sht.sensor->ruleTermsSize; j++) {
    //        sht.sensor->ruleTerms[j]->input = measure;
    //    }
    for (i = 0; i < SENSORS; i++) {
        measure = sensors[i]->sense(&measures);
        for (j = 0; j < sensors[i]->ruleTermsSize; j++) {
            sensors[i]->ruleTerms[j]->input = measure;
        }
    }
    // Turn off sensor board
    SensorProxy_powerOff();
    // Run fuzzy engine
    return RuleEngine_run(&engine);
}

/*..........................................................................*/

/* Returns sensor byte identification based on sensor id attribute */
UINT8 SensorProxy_getSensorByte(void) {
    return sht.sensor->id;
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

/*...........................................................................*/
UINT8 SensorProxy_usbTest(char* usbOutBuffer) {
    List* list = NULL;

    SensorProxy_sense();
    list = SensorProxy_getMeasures();
    Util_strCopy(list->data, (UINT8*) usbOutBuffer, list->size);
    return list->size;

}
