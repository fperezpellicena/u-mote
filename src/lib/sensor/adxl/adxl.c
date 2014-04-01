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

#include <limits.h>
#include "adxl.h"
#include "timer.h"
#include "hw_adc.h"
#include "bsp.h"
#include "bsp_inertial.h"

void Adxl_init(void) {
    Adc_init();
}

/** Start conversion and read x axis acceleration */
UINT16 Adxl_accelerationX(void) {
    ADXL_X_CHANNEL_SET();
    return Adc_convert(ADXL_X_AXIS);
}

/** Start conversion and read y axis acceleration */
UINT16 Adxl_accelerationY(void) {
    ADXL_Y_CHANNEL_SET();
    return Adc_convert(ADXL_Y_AXIS);
}
