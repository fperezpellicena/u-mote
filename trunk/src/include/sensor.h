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

#ifndef sensor_h
#define sensor_h

#include "GenericTypeDefs.h"
#include "bsp.h"
#include "payload.h"
#include "fuzzy_rule.h"

/* Declare sensor*/
#define DECLARE_SENSOR(id, name, senseFn)\
Sensor name = {id, senseFn, NULL, 0}

/* Declare fuzzy sensor*/
#define DECLARE_FUZZY_SENSOR(id, name, senseFn, termsSize, ...)\
RuleTerm* name##terms[termsSize] = {__VA_ARGS__};\
Sensor name = {id, senseFn, name##terms, termsSize}

/*..........................................................................*/
/* Sense function prototype */
typedef UINT8 rom (*Sense)(Payload* measures);

/*..........................................................................*/
/* Check alert function prototype */
typedef BOOL rom (*CheckAlert)(Payload* list);

/*..........................................................................*/
/* Sensor class */
typedef struct Sensor Sensor;

struct Sensor {
    UINT8 id;
    Sense sense; /* Sense function */
    RuleTerm** ruleTerms; /* Rule terms associated */
    UINT8 ruleTermsSize; /* Rule terms size */
};

/*..........................................................................*/
void Sensor_create(UINT8 id, Sensor* sensor, Sense senseFunction);

/*..........................................................................*/
void Sensor_createFuzzy(UINT8 id, Sensor* sensor, Sense senseFunction,
       UINT8 ruleTermsSize, RuleTerm** ruleTerms);

/*..........................................................................*/
/* Sensor vector */
typedef struct Sensors Sensors;

struct Sensors {
    UINT8 size;
    Sensor* sensors[SENSORS];
};

/*..........................................................................*/
void Sensors_init(Sensors* sensors);

#endif /* sensor_h */
