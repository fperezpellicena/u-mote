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

#ifndef adxl_h
#define adxl_h

#include "bsp.h"

/*...........................................................................*/
/* Adxl class */
typedef struct Adxl Adxl;

struct Adxl {
    UINT8 id;
};

#define DECLARE_ADXL(id, name) Adxl name = {id}

typedef struct Acceleration Acceleration;

struct Acceleration {
    UINT8 x;
    UINT8 y;
};

/** Init resources */
void Adxl_init(void);

/** Start and read both x and y accelerations */
void Adxl_convertAcceleration(Acceleration* acceleration);

/** Start conversion and read x axis acceleration */
UINT8 Adxl_convertAccelerationX(void) ;

/** Start conversion and read y axis acceleration */
UINT8 Adxl_convertAccelerationY(void);

/** Interrupt mode */
/** Start X axis acceleration conversion */
void Adxl_startAccelerationX(void);

/** Start Y axis acceleration conversion */
void Adxl_startAccelerationY(void);

/** Read acceleration after conversion */
UINT8 Adxl_readAcceleration(void);

#endif
