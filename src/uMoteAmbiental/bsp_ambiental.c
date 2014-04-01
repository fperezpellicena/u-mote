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
#include <delays.h>
#include "bsp.h"
#include "digi_api.h"
#include "digi_interrupt.h"
#include "sensor_proxy.h"
#if RTCC_ENABLED
#    include "rtc.h"
#endif

void BSP_ambientalInit(void) {
    BSP_init();
    // Initializes mote API
    XBee_init();
//    // Enable sensor board
    SensorProxy_init();
}

/*...........................................................................*/
void BSP_onPowerUp(void) {
#if RTCC_ENABLED
    // Enable RTCC
    Rtc_init();
#endif
}

/*...........................................................................*/

/* On reset push, join xbee */
void BSP_onMclr(void) {
    BSP_clearMclrFlags();
    Delay10KTCYx(10);
    XBee_join();
}

/*...........................................................................*/
void BSP_onWakeUp(void) {
    // Clear wake up flags
    BSP_clearDSWakeUpFlags();
    // Ejecuta la interrupción que ha despertado al sistema
    XBeeInterrupt_handleBottomHalve();
}

/*...........................................................................*/

/**
 * Entering this method could mean that the transceiver is malfunctioning.
 * Xbee may be down or may have lost sync with his neighbor.
 * So, try to send a reset message to the transceiver and rejoin the mote.
 * A 100 ms delay is necessary to perform a full reset before send any command.
 */
void BSP_onDsWdtWakeUp(void) {
    XBee_reset();
    Delay100TCYx(UINT_MAX);
    XBee_join();
}
