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

static UINT8 Sht_sense(List* measures);
static UINT8 Irca_sense(List* measures);

#pragma udata mf
DECLARE_MF(lowTemp, -40, 0, 20);
DECLARE_MF(midTemp, 0, 20, 40);
DECLARE_MF(highTemp, 30, 40, 125);

DECLARE_MF(lowCo2, 0, 0, 50);
DECLARE_MF(midCo2, 0, 50, 100);
DECLARE_MF(highCo2, 50, 100, 100);

DECLARE_MF(lowRisk, 0, 0, 50);
DECLARE_MF(midRisk, 0, 50, 100);
DECLARE_MF(highRisk, 50, 100, 100);
#pragma udata

#pragma udata rp
DECLARE_RT(ifHighTemp, &highTemp);
DECLARE_RT(andHighCo2, &highCo2);
DECLARE_RT(thenHighRisk, &highRisk);

DECLARE_RT(andLowCo2, &lowCo2);
DECLARE_RT(thenMidRisk, &midRisk);
#pragma udata

#pragma udata rule
DECLARE_RULE(ifHighTempAndHighCo2ThenHighRisk, &thenHighRisk, 2, &ifHighTemp, &andHighCo2);
DECLARE_RULE(ifHighTempAndlowCo2ThenMidRisk, &thenMidRisk, 2, &ifHighTemp, &andLowCo2);
#pragma udata

#pragma udata ruleEngine
DECLARE_ENGINE(engine, 2, &ifHighTempAndHighCo2ThenHighRisk, &ifHighTempAndlowCo2ThenMidRisk);
#pragma udata

/* Declare one SHT sensor */
#pragma udata sensors
DECLARE_FUZZY_SHT(SHT_ID, sht, &Sht_sense, 1, &ifHighTemp);
DECLARE_FUZZY_IRCA(IRCA1_ID, irca, &Irca_sense, 2, &andHighCo2, &andLowCo2);
DECLARE_SENSOR_VECTOR(sensors, 2, &sht, &irca);
#pragma udata

static List measures;

/*...........................................................................*/
static UINT8 Sht_sense(List* measures) {
    // Measure
    Sht11_measure(&sht);
    // Put measures into payload
    List_add(measures, sht.data->humidity.i);
    List_add(measures, sht.data->temperature.i);
    // Return temperature
    return sht.data->temperature.i;
}

/*...........................................................................*/
static UINT8 Irca_sense(List* measures) {
    // Measure values
    IrcA1Proxy_measure(&irca);
    // Calculate CO2
    IrcA1_calculate(&irca);
    // Put data into measures
    List_add(measures, round(irca.data->x)); // Float to int routine
}

/*..........................................................................*/
void SensorProxy_init(void) {
    List_init(&measures);
    ShtProxy_init(); /* Init SHT resources */
    IrcA1Proxy_init(); /* Init IRCA resources */
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
    for (i = 0; i < sensors.size; i++) {
        sensors.sensors[i]->sense(&measures);
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
    if (sensors.size == 0) {
        return FALSE;
    }
    // Turn on sensor board
    SensorProxy_powerOn();
    // Empty previous measures
    List_empty(&measures);
    for (i = 0; i < sensors.size; i++) {
        measure = sensors.sensors[i]->sense(&measures);
        for (j = 0; j < sensors.sensors[i]->ruleTermsSize; j++) {
            sensors.sensors[i]->ruleTerms[j]->input = measure;
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
    UINT8 byte = 0;
    UINT8 i;
    if (sensors.size == 0) {
        return 0;
    }
    for (i = 0; i < sensors.size; i++) {
        byte |= sensors.sensors[i]->id; // Sensor id's OR'ed
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
