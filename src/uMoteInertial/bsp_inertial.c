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
#include "digi_api.h"
#include "sensor_proxy.h"
#if RTCC_ENABLED
#    include "rtc.h"
#include "timer.h"
#endif

void BSP_inertialInit(void) {
    BSP_init();         // Initializes mote API
    XBee_init();
    Timer0_init();
    SensorProxy_init(); // Enable sensor board
    BSP_onPowerUp();
}

/*...........................................................................*/
void BSP_onPowerUp(void) {
#if RTCC_ENABLED
    // Enable RTCC
    Rtc_init();
#endif
    XBee_join();
}

/*...........................................................................*/
void BSP_onMclr(void) {
    return;
}

/*...........................................................................*/
void BSP_onWakeUp(void) {
    return;
}

/*...........................................................................*/
void BSP_onDsWdtWakeUp(void) {
    return;
}
