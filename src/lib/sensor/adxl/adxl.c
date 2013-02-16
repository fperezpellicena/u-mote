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

#include "adxl.h"
#include "timer.h"
#include "hw_adc.h"
#include "bsp.h"

void Adxl_init(void) {
    Adc_init();
    // FIXME Move out these sentences
    Timer0_init();
//    FallTimerInterrupt_install();
}

/** Start and read both x and y accelerations */
void Adxl_convertAcceleration(Acceleration* acceleration) {
    acceleration->x = Adxl_convertAccelerationX();
    acceleration->y = Adxl_convertAccelerationY();
}

/** Start conversion and read x axis acceleration */
UINT8 Adxl_convertAccelerationX(void) {
    return Adc_convert(ADXL_X_AXIS);
}

/** Start conversion and read y axis acceleration */
UINT8 Adxl_convertAccelerationY(void) {
    return Adc_convert(ADXL_Y_AXIS);
}

/** Interrupt mode */
/** Start X axis acceleration conversion */
void Adxl_startAccelerationX(void) {
    Adc_startConversion(ADXL_X_AXIS);
}

/** Start Y axis acceleration conversion */
void Adxl_startAccelerationY(void) {
    Adc_startConversion(ADXL_Y_AXIS);
}

/** Read axis acceleration after conversion */
UINT8 Adxl_readAcceleration(void) {
    return Adc_readValue();
}
