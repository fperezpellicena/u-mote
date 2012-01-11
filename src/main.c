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
#pragma config PLLDIV = 3           //Divide by 3 (12 MHz oscillator input)
#pragma config STVREN = ON          //stack overflow/underflow reset enabled
#pragma config XINST = ON           //Extended instruction set disabled
#pragma config CPUDIV = OSC1        //No CPU system clock divide
#pragma config CP0 = OFF            //Program memory is not code-protected
#pragma config OSC = HSPLL          //HS oscillator, PLL enabled, HSPLL used by USB
#pragma config FCMEN = OFF          //Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF           //Two-Speed Start-up disabled
#pragma config WDTPS = 32768        //1:32768
#pragma config DSWDTOSC = INTOSCREF //DSWDT uses INTOSC/INTRC as clock
#pragma config RTCOSC = T1OSCREF    //RTCC uses T1OSC/T1CKI as clock
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
#include "hw_profile.h"
#include "digi_isr.h"

#ifdef USB_ENABLED
#include "usb_device.h"
#include "power.h"
#endif

/*...........................................................................*/
/* Prototypes */
static void BSP_initializeSystem(void);
static void BSP_prepareSleep(void);
static void BSP_installInterrupts(void);

/*...........................................................................*/
/* Main */
#pragma code

void main(void) {
    // Init basic system
    BSP_initializeSystem();
    // Install all system interrupts
    BSP_installInterrupts();

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
                USB_blinkStatus();
            }
        } else {
            // Si no está conectado el terminal USB, entra en modo de bajo consumo
            if ((USBGetDeviceState() == ATTACHED_STATE)) {
                USBDeviceDetach();
            }
            BSP_prepareSleep();
            sleep();
            // Ejecuta la interrupción que ha despertado al sistema
            InterruptHandler_handleActiveInterrupt();
        }
#else
        BSP_prepareSleep();
        sleep();
        // Ejecuta la interrupción que ha despertado al sistema
        InterruptHandler_handleActiveInterrupt();
#endif
    }
}

/*...........................................................................*/

/* FIXME To bsp package */
static void BSP_initializeSystem(void) {
    // Default all pins to digital
    ADCON1 |= 0x0F;
    //Enable the PLL and wait 2+ms until the PLL locks before enabling USB module
    {
        unsigned int pll_startup_counter = 600;
        OSCTUNEbits.PLLEN = 1;
        while (pll_startup_counter--);
    }

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
}

/*...........................................................................*/

/* FIXME To bsp package */
static void BSP_prepareSleep(void) {
    // pre sleep actions
    LATCbits.LATC0 = 0;
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
}

/* Install interrupts */
static void BSP_installInterrupts(void) {
    // Init interrupt vectors
    InterruptHandler_initVectors();
    // Install xbee ISR
    XBee_installInterrupt();
}
