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

/*...........................................................................*/
/* Includes */
#include "bsp_inertial.h"
#include "isr.h"

#if USB_ENABLED
#include "usb_device.h"
#include "usb_handler.h"
#endif

/*...........................................................................*/
/* Main */
#pragma code

void main(void) {
    BSP_inertialInit();
    while (1) {
#if USB_ENABLED
        // Comprueba el terminal que indica la conexi�n USB al inicio o al reset
        if (USB_PLUGGED) {
            // Si no se ha activado el USB, lo activa
            if ((USBGetDeviceState() == DETACHED_STATE)) {
                USBDeviceAttach();
            } else {
                // Si ya se ha activado, realiza las tareas USB
                USB_process();
            }
#ifdef __18F46J50_H
            USB_blinkStatus();
#endif
        }
#endif
        BSP_mainProcess();
    }
}
