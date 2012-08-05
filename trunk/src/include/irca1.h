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

#ifndef irca1_h
#define irca1_h

#include "sensor.h"

#define DECLARE_IRCA(id, name, senseFn)\
DECLARE_FUZZY_SENSOR(id, name##id, senseFn);\
IrcA1Data name##data;\
Irca1Cal name##cal;\
IrcA1 name = {&name##id, &name##data, &name##cal}

#define DECLARE_FUZZY_IRCA(id, name, senseFn, temsSize, ...)\
DECLARE_FUZZY_SENSOR(id, name##id, senseFn, temsSize, __VA_ARGS__);\
IrcA1Data name##data;\
Irca1Cal name##cal;\
IrcA1 name = {&name##id, &name##data, &name##cal}


/*...........................................................................*/
/* Class IrcA1Data sensor calibration data */
typedef struct Irca1Cal Irca1Cal;

struct Irca1Cal {
    /* Reference value in gas absence */
    UINT8 zero;
    /* Proportion of radiation that impinges on the active element of the detector
     * that has the ability to be absorbed by the target gas*/
    UINT8 span;
};

/*...........................................................................*/
/* Class IrcA1Data sensor data */
typedef struct IrcA1Data IrcA1Data;

struct IrcA1Data {
    UINT8 ref;
    UINT8 act;
    UINT8 tmp;
    float x;
};

/*...........................................................................*/
/* Class IrcA1 sensor */
typedef struct IrcA1 IrcA1;

struct IrcA1 {
    Sensor* sensor;
    IrcA1Data* data;
    Irca1Cal* cal;
};

/*...........................................................................*/
/* Init sensor struct and hw associated */
void IrcA1_init(void);

/*...........................................................................*/
/* IrcA1 calculate CO2 */
void IrcA1_calculate(IrcA1* irca1) ;

#endif /* irca1_h */