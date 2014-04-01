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

static void BSP_clearInterrupts(void);

static void BPS_initOSC(void);

/*...........................................................................*/
void BSP_init(void) {
    // Init default INTOSC
    BPS_initOSC();
    // Default all pins to digital
    BSP_initIO();
    // Clear interrupts
    BSP_clearInterrupts();
//#if SLEEP_MODE == SLEEP
//    // Init interrupt vectors
//    InterruptHandler_initVectors();
//#endif
//#if USB_ENABLED
//    BSP_enablePLL();
//    ENABLE_USB_ATTACH;
//    // Initializes USB module SFRs and firmware variables to known states
//    USBDeviceInit();
//#endif
}

void BPS_initOSC(void) {
    // INTOSC drives clock directly
    OSCCONbits.IRCF = IRCF_8MHZ;
    // INTRC/INTOSC derived
    OSCCONbits.SCS = SCS_INTRC;
}

void BSP_clearInterrupts(void) {
    // Clear peripheral priorities
    IPR1 = 0x00;
    IPR2 = 0x00;
    IPR3 = 0x00;
    // Clear interrupt flags
    PIR1 = 0x00;
    PIR2 = 0x00;
    PIR3 = 0x00;
    INTCON = 0x00;

    RCONbits.IPEN = 1;
    INTCONbits.PEIE = 1;
}
/*...........................................................................*/
// FIXME Corregido pero no probado:
//      Enable PLL on USB PLUGIN (power saving)

void BSP_enablePLL(void) {
    // Enable PLL path
    OSCCONbits.SCS = 0;
    //Enable the PLL and wait 2+ms until the PLL locks before enabling USB module
    {
	unsigned int pll_startup_counter = 600;
	OSCTUNEbits.PLLEN = 1;
	while (pll_startup_counter--);
    }
}

/*...........................................................................*/
void BSP_initIO(void) {
    ADCON1 |= 0x0F;
    ANCON0 = 0xFF;
    ANCON1 = 0xFF;
}

/*...........................................................................*/
void BSP_clearMclrFlags(void) {
    DSWAKELbits.DSMCLR = 0;
    DSCONLbits.RELEASE = 0;
}

void BSP_clearDSWakeUpFlags(void) {
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
#if RTCC_ENABLED
    Rtc_enable();
#endif
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    Power_sleep();
}

