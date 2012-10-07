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

#include "bsp.h"
#include "sensor_proxy.h"
#include "util.h"
#include <stdio.h>

#if SHT_ENABLED
#   include "sht.h"
#endif
#if IRCA1_ENABLED
#   include "irca1.h"
#   include "irca1_proxy.h"
#endif

#if SENSING_MODE == FUZZY_DRIVEN
    #include "fuzzy.h"

    #pragma udata tmp_mf
    DECLARE_MF(lowTemp, -40, 0, 20);
    DECLARE_MF(midTemp, 0, 20, 40);
    DECLARE_MF(highTemp, 30, 40, 255);
    DECLARE_RT(ifHighTemp, &highTemp);
    DECLARE_RT(ifMidTemp, &midTemp);
    DECLARE_RT(ifLowTemp, &lowTemp);
    #pragma udata

    #pragma udata risk_mf
    DECLARE_MF(lowRisk, 0, 0, 50);
    DECLARE_MF(midRisk, 0, 50, 100);
    DECLARE_MF(highRisk, 50, 100, 255);
    #pragma udata

    #pragma udata rp
    DECLARE_RT(thenHighRisk, &highRisk);
    DECLARE_RT(thenMidRisk, &midRisk);
    DECLARE_RT(thenLowRisk, &lowRisk);
    #pragma udata

    #pragma udata rules
    DECLARE_RULE(ifHighTempThenHighRisk, &thenHighRisk, 1, &ifHighTemp);
    DECLARE_RULE(ifMidTempThenMidRisk, &thenMidRisk, 1, &ifLowTemp);
    DECLARE_RULE(ifLowTempThenLowRisk, &thenLowRisk, 1, &ifMidTemp);
    //DECLARE_RULE(ifHighTempAndHighCo2ThenHighRisk, &thenHighRisk, 2, &ifHighTemp, &andHighCo2);
    //DECLARE_RULE(ifHighTempAndlowCo2ThenMidRisk, &thenMidRisk, 2, &ifHighTemp, &andLowCo2);
    #pragma udata

    #pragma udata ruleEngine
    DECLARE_ENGINE(engine, 3, &ifHighTempThenHighRisk,
            &ifMidTempThenMidRisk, &ifLowTempThenLowRisk);
    //DECLARE_ENGINE(engine, 2, &ifHighTempAndHighCo2ThenHighRisk, &ifHighTempAndlowCo2ThenMidRisk);
    #pragma udata
#endif

#pragma udata sensors_data
Sensors sensors;
#pragma udata

/* Declare one SHT sensor for temperature */
#if SHT_ENABLED
#   pragma udata sht
#       if SENSING_MODE == FUZZY_DRIVEN
            DECLARE_FUZZY_SHT(SHT_ID, sht, &Sht11_measureTmp, 1, &ifHighTemp);
#       else
            DECLARE_SHT(SHT_ID, sht, &Sht11_measureTmp);
#       endif
#   pragma udata
#endif
            
/* Declare one IRC-A1 gas sensor */
#if IRCA1_ENABLED
#   pragma udata irca
#       if SENSING_MODE == FUZZY_DRIVEN
            DECLARE_FUZZY_IRCA(IRCA1_ID, irca, &IrcA1Proxy_sense, 1, &ifHighCO2);
#       else
            DECLARE_IRCA(IRCA1_ID, irca, &IrcA1Proxy_sense);
#       endif
#   pragma udata
#endif

static Payload measures;

/*..........................................................................*/
void SensorProxy_init(void) {
    SENSOR_BOARD_CTRL_INIT();
    Sensors_init(&sensors);
    Payload_init(&measures);
#if SHT_ENABLED
    SensorProxy_add(&sensors, sht.sensor);
    Sht11_init();
#endif
#if IRCA1_ENABLED
    SensorProxy_add(&sensors, irca.sensor);
    IrcA1Proxy_init();
#endif
}

/*..........................................................................*/
void SensorProxy_add(Sensors* sensors, Sensor* sensor) {
    if (sensors->size < SENSORS) {
        sensors->sensors[sensors->size++] = sensor;
    }
}

/*..........................................................................*/

/* Turns on sensor board, sense every sensor installed and turns off */
void SensorProxy_sense(void) {
    UINT8 i;
    // Turn on sensor board
    SENSOR_BOARD_ON();
    // Empty previous measures
    Payload_empty(&measures);
    // For each sensor installed, put measures into payload
    for (i = 0; i < sensors.size; i++) {
        sensors.sensors[i]->sense(&measures);
    }
    // Turn off sensor board
    SENSOR_BOARD_OFF();
}

/*..........................................................................*/

#if SENSING_MODE == FUZZY_DRIVEN
    /* Measure sensor board and check for alert condition */
    UINT8 SensorProxy_fuzzy(void) {
        UINT8 i;
        UINT8 j;
        UINT8 measure;
        // Turn on sensor board
        SensorProxy_powerOn();
        // Empty previous measures
        Payload_empty(&measures);
        // Put measures into rule terms
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
#endif

/*..........................................................................*/

/* Put sensor byte identification based on sensor id attribute */
void SensorProxy_addSensorsToPayload(Payload* payload) {
    UINT32 sensors = 0; //NO_SENSORS << 24 | NO_SENSORS << 16 | sht.sensor->id << 8 | NO_SENSORS;
    Payload_add(payload, (UINT8) sensors);
    Payload_add(payload, (UINT8) sensors >> 8);
    Payload_add(payload, (UINT8) sensors >> 16);
    Payload_add(payload, (UINT8) sensors >> 24);
}

/*..........................................................................*/
/* Put measures to payload */
void SensorProxy_addMeasuresToPayload(Payload* payload) {
    Payload_append(payload, &measures);
}
