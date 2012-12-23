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
#include "power.h"

#if USB_ENABLED
#    include "usb_device.h"
#endif
#if RTCC_ENABLED
#    include "rtc.h"
#endif
#if SLEEP_MODE == SLEEP
#    include "isr.h"
#endif

/*...........................................................................*/
void BSP_init(void) {
    // Default all pins to digital
    BSP_defaultIO();
#if SLEEP_MODE == SLEEP
    // Init interrupt vectors
    InterruptHandler_initVectors();
#endif
#if USB_ENABLED
    BSP_enablePLL();
    ENABLE_USB_ATTACH;
    // Initializes USB module SFRs and firmware variables to known states
    USBDeviceInit();
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
void BSP_defaultIO(void) {
    ADCON1 |= 0x0F;
    ANCON0 = 0xFF;
    ANCON1 = 0xFF;
}

/*...........................................................................*/
void BSP_clearMclrFlags(void) {
    DSWAKELbits.DSMCLR = 0;
    DSCONLbits.RELEASE = 0;
}

void BSP_clearWakeUpFlags(void) {
    WDTCONbits.DS = 0;
    DSWAKEHbits.DSINT0 = 0;
    DSCONLbits.RELEASE = 0;
}

/*...........................................................................*/
void BSP_deepSleep(void) {
    // Disable PLL
    OSCTUNEbits.PLLEN = 0;
#if RTCC_ENABLED
    Rtc_enable();
#endif
    Power_deepSleep();
}

/*...........................................................................*/
void BSP_sleep(void) {
    // Disable PLL
    OSCTUNEbits.PLLEN = 0;
#if RTCC_ENABLED
    Rtc_enable();
#endif
    INTCONbits.GIEH = 1;
    Power_sleep();
}

