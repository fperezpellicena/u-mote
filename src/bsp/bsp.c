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
#include "hw_profile.h"
#include "usb_device.h"
#include "service.h"
#include "isr.h"
#include "digi_proxy.h"
#include <p18cxxx.h>

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

/* FIXME To bsp package */
void BSP_initializeSystem(void) {
    // Default all pins to digital
    ADCON1 |= 0x0F;

    ANCON0 = 0xFF; // Default all pins to digital
    ANCON1 = 0xFF; // Default all pins to digital

    // Configura PORTC<0> como salida de test
    TRISCbits.TRISC0 = 0;
    // COnfigura PORTB<4> como entrada para el conector USB
    TRISBbits.TRISB4 = 1;

    //Initialize all of the LED pins
    mInitAllLEDs();

    //Initialize the pushbuttons
    mInitAllSwitches();

    // Initializes USB module SFRs and firmware variables to known states
    USBDeviceInit();

    // Initializes mote API
    Service_initMote();
}

/*...........................................................................*/

/* FIXME To bsp package */
void BSP_prepareSleep(void) {
    // Disable PLL
    OSCTUNEbits.PLLEN = 0;
    // Pre-sleep actions
    LATCbits.LATC0 = 0;
    // Enable interrupts
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
}

/* Install interrupts */
void BSP_installInterrupts(void) {
    // Init interrupt vectors
    InterruptHandler_initVectors();
    // Install xbee ISR
    XBeeProxy_installInterrupt();
    // Install gps ISR
    //Gps_installInterrupt();
}
