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
#include "usb_device.h"
#include "sensor_proxy.h"
#include "sht_proxy.h"
#include "isr.h"
#include "rtc.h"
#include "digi_proxy.h"
#include <delays.h>

/*...........................................................................*/
void BSP_init(void) {
    // TODO Probar si funciona activar el PLL cuando se conecta el terminal USB
    BSP_enablePLL();
    // Default all pins to digital
    ADCON1 |= 0x0F;
    ANCON0 = 0xFF;
    ANCON1 = 0xFF;
    //Initialize all of the LED pins
    mInitAllLEDs();
    // Init interrupt vectors
    InterruptHandler_initVectors();
    // Initializes mote API
    XBeeProxy_init();
    // Enable sensor board
    SensorProxy_init();
#ifdef USB_ENABLED
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
void BSP_onPowerUp(void) {
    // Enable RTCC
    Rtc_init();
}

/*...........................................................................*/
void BSP_onMclr(void) {
    // On reset push, join xbee
    DSWAKELbits.DSMCLR = 0;
    DSCONLbits.RELEASE = 0;
    // Bugfix: Es necesario un delay tras el reset
    Delay10KTCYx(10);
    XBeeProxy_join();
#ifdef __18F46J50_H
    TRISDbits.TRISD1 = 0;
    LATDbits.LATD1 = 1;
#endif
}

/*...........................................................................*/
void BSP_onWakeUp(void) {
#ifdef __18F46J50_H
    TRISEbits.TRISE0 = 0;
    LATEbits.LATE0 = !LATEbits.LATE0;
    TRISDbits.TRISD1 = 0;
#endif
    // Clear wake up flags
    WDTCONbits.DS = 0;
    DSWAKEHbits.DSINT0 = 0;
    DSCONLbits.RELEASE = 0;
    // Enable sensor board
    SensorProxy_init();
    // Ejecuta la interrupción que ha despertado al sistema
    XBeeProxy_handleBottomHalveInterrupt();
}

/*...........................................................................*/
void BSP_deepSleep(void) {
    // Disable PLL
    OSCTUNEbits.PLLEN = 0;
    Rtc_enable();
    // Go to sleep
    deepSleep();
}

/*...........................................................................*/
void BSP_sleep(void) {
    // Disable PLL
    OSCTUNEbits.PLLEN = 0;
    Rtc_enable();
    // Go to sleep
    sleep();
}
