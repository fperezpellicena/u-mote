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
#include <stdio.h>

#if SHT_ENABLED
#include "sht.h"
#endif
#if IRCA1_ENABLED
#include "irca1.h"
#include "irca1_proxy.h"
#endif

#if SENSING_MODE == FUZZY_DRIVEN
#include "fuzzy.h"

/* Declare one SHT sensor for temperature */
#if SHT_ENABLED
#   pragma udata sht
#   if SENSING_MODE == FUZZY_DRIVEN
        extern RuleTerm ifHighTemp;
        extern RuleTerm ifLowTemp;
        extern RuleTerm ifMidTemp;
        DECLARE_FUZZY_SHT(SHT_ID, sht, 1, &ifHighTemp);
#   else
        DECLARE_SHT(SHT_ID, sht);
#   endif
#   pragma udata
#endif

/* Declare one IRC-A1 gas sensor */
#if IRCA1_ENABLED
#   pragma udata irca
#   if SENSING_MODE == FUZZY_DRIVEN
        extern RuleTerm ifHighCo2;
        DECLARE_FUZZY_IRCA(IRCA1_ID, irca, 1, &ifHighCo2);
#   else
        DECLARE_IRCA(IRCA1_ID, irca);
#   endif
#   pragma udata
#endif

extern RuleEngine engine;

#endif

static Payload payload;
static UINT8 sensorIdentifiers;

static void SensorProxy_composeSensorIdentifiers(void);

/*..........................................................................*/
void SensorProxy_init(void) {
    SENSOR_BOARD_CTRL_INIT();
    Payload_init(&payload);
#if SHT_ENABLED
    Sht11_init();
#endif
#if IRCA1_ENABLED
    IrcA1Proxy_init();
#endif
    SensorProxy_composeSensorIdentifiers();
}

/*..........................................................................*/

/* Turns on sensor board, sense every sensor installed and turns off */
void SensorProxy_sense(void) {
    // Turn on sensor board
    SENSOR_BOARD_ON();
    // Sense installed sensors
#if SHT_ENABLED
    Sht11_measure(&sht);
    Sht11_addMeasuresToPayload(&sht, &payload);
#endif
#if IRCA1_ENABLED
    IrcA1Proxy_measure(&irca);
    IrcA1Proxy_addMeasuresToPayload(&irca, &payload);
#endif
    // Turn off sensor board
    SENSOR_BOARD_OFF();
}

/*..........................................................................*/

#if SENSING_MODE == FUZZY_DRIVEN

/* Measure sensor board and check for alert condition */
UINT8 SensorProxy_fuzzy(void) {
    // Sense
    SensorProxy_sense();
#if SHT_ENABLED
    Sht11_prepareFuzzyInputs(&sht);
#endif
#if IRCA1_ENABLED
    IrcA1_prepareFuzzyInputs(&irca);
#endif
    return RuleEngine_run(&engine);
}
#endif

/*..........................................................................*/

/* Put sensor byte identification based on sensor id attribute */
void SensorProxy_addSensorIdentifiersToPayload(Payload* payload) {
    Payload_putByte(payload, 0);
    Payload_putByte(payload, sensorIdentifiers);
    Payload_putByte(payload, 0);
    Payload_putByte(payload, 0);
}

/*..........................................................................*/

/* Put measures to payload */
void SensorProxy_addMeasuresToPayload(Payload* p) {
    Payload_append(p, &payload);
}

/*..........................................................................*/

/* Compose sensor identifier array by OR'ing each identifier supposed uniques */
// FIXME Sensor vector should be statically defined because it never changes one programmed
static void SensorProxy_composeSensorIdentifiers(void) {
    sensorIdentifiers = 0;
#if SHT_ENABLED
    sensorIdentifiers |= sht.id;
#endif
#if IRCA1_ENABLED
    sensorIdentifiers |= irca.id;
#endif
}