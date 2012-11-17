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
#    include "sht.h"
#endif

#if IRCA1_ENABLED
#    include "irca1.h"
#    include "irca1_proxy.h"
#endif

#if SENSING_MODE == FUZZY_DRIVEN
#    include "fuzzy.h"
#    include "rules.h"
#    pragma idata engine
DECLARE_ENGINE(engine);
#    pragma idata
#endif

/* Declare one SHT sensor for temperature */
#if SHT_ENABLED
#    pragma idata sht
DECLARE_SHT(SHT_ID, sht);
#    pragma idata
#endif

/* Declare one IRC-A1 gas sensor */
#if IRCA1_ENABLED
#    pragma idata irca
DECLARE_IRCA(IRCA1_ID, irca);
#    pragma idata
#endif

static Payload payload;
static UINT8 sensorIdentifiers;

static void SensorProxy_composeSensorIdentifiers(void);

#if SENSING_MODE == FUZZY_DRIVEN
/* Risk level */
static UINT8 risk;

/* Init fuzzy rules and sensors */
static void SensorProxy_initFuzzy(void);

/* Measure sensor board and check for alert condition */
static void SensorProxy_fuzzy(void);

static void SensorProxy_initFuzzy(void) {
    Fuzzy_initRules(&engine);
#    if SHT_ENABLED
    Fuzzy_initSht(&sht);
#    endif
#    if IRCA1_ENABLED
    Fuzzy_initIrca(&irca);
#    endif
}

/* Measure sensor board and check for alert condition */
static void SensorProxy_fuzzy(void) {
    // Sense
    SensorProxy_sense();
#    if SHT_ENABLED
    Sht11_setFuzzyInputs(&sht);
#    endif
#    if IRCA1_ENABLED
    IrcA1_prepareFuzzyInputs(&irca);
#    endif
    RuleEngine_run(&engine);
}
#endif

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
#if SENSING_MODE == FUZZY_DRIVEN
    SensorProxy_initFuzzy();
#endif
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
#if SENSING_MODE == FUZZY_DRIVEN
    SensorProxy_fuzzy();
#endif
}

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
#if SENSING_MODE == FUZZY_DRIVEN
    // Add Risk level
    Payload_putByte(&payload, risk);
#endif
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