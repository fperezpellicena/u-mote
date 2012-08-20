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
/* Pragmas */
#pragma config WDTEN = OFF          //WDT disabled (enabled by SWDTEN bit)
#pragma config STVREN = ON          //stack overflow/underflow reset enabled
#pragma config XINST = OFF          //Extended instruction set disabled
#pragma config CPUDIV = OSC4_PLL6   //CPU system clock divide 6
#pragma config CP0 = OFF            //Program memory is not code-protected
#pragma config OSC = INTOSCPLL      //Internal OSC(8mhz), PLL enabled by soft
#pragma config PLLDIV = 2           //Divide By 2(4 Mhz) to generate 96Mhz
#pragma config FCMEN = OFF          //Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF           //Two-Speed Start-up disabled
#pragma config WDTPS = 32768        //1:32768
#pragma config DSWDTOSC = INTOSCREF //DSWDT uses INTOSC/INTRC as clock
#pragma config RTCOSC = INTOSCREF   //RTCC uses INTRC
#pragma config DSBOREN = OFF        //Zero-Power BOR disabled in Deep Sleep
#pragma config DSWDTEN = OFF        //Disabled
#pragma config DSWDTPS = 8192       //1:8,192 (8.5 seconds)
#pragma config IOL1WAY = OFF        //IOLOCK bit can be set and cleared
#pragma config MSSP7B_EN = MSK7     //7 Bit address masking
#pragma config WPFP = PAGE_1        //Write Protect Program Flash Page 0
#pragma config WPEND = PAGE_0       //Start protection at page 0
#pragma config WPCFG = OFF          //Write/Erase last page protect Disabled
#pragma config WPDIS = OFF          //WPFP[5:0], WPEND, and WPCFG bits ignored  

/*...........................................................................*/
/* Includes */
#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "bsp.h"
#include "isr.h"
#include "rtc.h"
#include <delays.h>

#include "sht.h"

#include "digi_proxy.h"

#ifdef USB_ENABLED
#include "usb_device.h"
#include "usb_handler.h"
#include "power.h"
#endif

/*...........................................................................*/
/* Main */
#pragma code

void main(void) {
    // Init basic system
    BSP_init();
    while (1) {
#ifdef USB_ENABLED
        // Comprueba el terminal que indica la conexi?n USB al inicio o al reset
        if (USB_PLUGGED) {
            // Si no se ha activado el USB, lo activa
            if ((USBGetDeviceState() == DETACHED_STATE)) {
                USBDeviceAttach();
            } else {
                // Si ya se ha activado, realiza las tareas USB
                USB_process();
                //USB_blinkStatus();
            }
        } else {
#if SLEEP_MODE == DEEP_SLEEP
            // Comprueba
            //  1.- Si se ha despertado tras un estado de deep sleep
            //  2.- Y ha sido causado por INT0 --> XBEE INT PIN
            if (XBEE_ON_SLEEP_AWAKE) {
#ifdef __18F46J50_H
                TRISEbits.TRISE0 = 0;
                LATEbits.LATE0 = !LATEbits.LATE0;
                TRISDbits.TRISD1 = 0;
#endif
                WDTCONbits.DS = 0;
                DSWAKEHbits.DSINT0 = 0;
                DSCONLbits.RELEASE = 0;
                // Ejecuta la interrupción que ha despertado al sistema
                XBeeProxy_handleBottomHalveInterrupt();
            } else if (ON_MCLR) {
                // On reset push, join xbee
                DSWAKELbits.DSMCLR = 0;
                DSCONLbits.RELEASE = 0;
                // Bugfix: Es necesario un delay tras el reset
                Delay10KTCYx(5);
                XBeeProxy_join();
#ifdef __18F46J50_H
                TRISDbits.TRISD1 = 0;
                LATDbits.LATD1 = 1;
#endif
                // Set low power mode
                //Sht11_writeStatusRegister(LOW_POWER_STAT_REG);
                //                Sht11_reset();
            } else {
                // Enable RTCC
                Rtc_init();

            }
            // Si no está conectado el terminal USB, entra en modo de bajo consumo
            if ((USBGetDeviceState() == ATTACHED_STATE)) {
                USBDeviceDetach();
            }
            BSP_prepareSleep();
            deepSleep();
#elif SLEEP_MODE == SLEEP
            // Si no está conectado el terminal USB, entra en modo de bajo consumo
            if ((USBGetDeviceState() == ATTACHED_STATE)) {
                USBDeviceDetach();
            }
            BSP_prepareSleep();
            sleep();
            InterruptHandler_handleActiveInterrupt();

#endif
        }
#else
#if SLEEP_MODE == DEEP_SLEEP
        // Comprueba
        //  1.- Si se ha despertado tras un estado de deep sleep
        //  2.- Y ha sido causado por INT0 --> XBEE INT PIN
        if (XBEE_ON_SLEEP_AWAKE) {
            TRISEbits.TRISE0 = 0;
            LATEbits.LATE0 = !LATEbits.LATE0;
            WDTCONbits.DS = 0;
            DSCONLbits.RELEASE = 0;
            // Ejecuta la interrupción que ha despertado al sistema
            XBeeProxy_handleBottomHalveInterrupt();
        }
        // Si no está conectado el terminal USB, entra en modo de bajo consumo
        if ((USBGetDeviceState() == ATTACHED_STATE)) {
            USBDeviceDetach();
        }
        BSP_prepareSleep();
        deepSleep();
#elif SLEEP_MODE == SLEEP
        // Si no está conectado el terminal USB, entra en modo de bajo consumo
        if ((USBGetDeviceState() == ATTACHED_STATE)) {
            USBDeviceDetach();
        }
        BSP_prepareSleep();
        sleep();
        InterruptHandler_handleActiveInterrupt();

#endif
#endif
    }
}
