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
#include "usb_device.h"
#include "sensor_proxy.h"
#include "sht_proxy.h"
#include "isr.h"
#include "rtc.h"


/*...........................................................................*/
void BSP_init(void) {
    // Default all pins to digital
    ADCON1 |= 0x0F;

    // TODO Probar si funciona activar el PLL cuando se conecta el terminal USB
    BSP_enablePLL();

    ANCON0 = 0xFF; // Default all pins to digital
    ANCON1 = 0xFF; // Default all pins to digital

    //Initialize all of the LED pins
    //mInitAllLEDs();
    
    // Init interrupt vectors
    InterruptHandler_initVectors();
    
    // Initializes mote API
    XBeeProxy_init();
#if AUTO_JOIN_ON_POWER_UP == TRUE
    XBeeProxy_join();
#endif
    SensorProxy_init();

#ifdef USB_ENABLED
    ENABLE_USB_ATTACH;
    // Initializes USB module SFRs and firmware variables to known states
    USBDeviceInit();
#endif

#ifdef RTCC_ENABLED
    Rtc_init();
#endif

#ifdef SHT11_ENABLED
    ShtProxy_init();
#endif
}

/*...........................................................................*/
// FIXME Corregido pero no probado:
//      Enable PLL on USB PLUGIN (power saving)
void BSP_enablePLL(void) {
    //Enable the PLL and wait 2+ms until the PLL locks before enabling USB module
    {
    unsigned int pll_startup_counter = 600;
    OSCTUNEbits.PLLEN = 1;
    while (pll_startup_counter--);
    }
}

/*...........................................................................*/
void BSP_prepareSleep(void) {
    // Disable PLL
    OSCTUNEbits.PLLEN = 0;
}
