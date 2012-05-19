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
//#pragma config PLLDIV = 3           //Divide by 3 (12 MHz oscillator input)
#pragma config STVREN = ON          //stack overflow/underflow reset enabled
#pragma config XINST = ON           //Extended instruction set disabled
#pragma config CPUDIV = OSC1        //No CPU system clock divide
#pragma config CP0 = OFF            //Program memory is not code-protected
//#pragma config OSC = HSPLL          //HS oscillator, PLL enabled, HSPLL used by USB

#pragma config OSC = INTOSCPLL      // Internal OSC(8mhz), PLL enabled by soft
#pragma config PLLDIV = 2           // Divide By 2(4 Mhz) to generate 96Mhz

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
#include "hw_profile.h"
#include "isr.h"

// Fuentes de interrupción asíncronas
#include "gps_isr.h"
// Clase de servicio
#include "service.h"

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
    BSP_initializeSystem();
    // Install all system interrupts
    BSP_installInterrupts();

    while (1) {
#ifdef USB_ENABLED
        // Comprueba el terminal que indica la conexi?n USB al inicio o al reset
        if (USB_PLUGGED) {
            // TODO Probar si funciona activar el PLL cuando se conecta el terminal USB
            BSP_enablePLL();
            // Si no se ha activado el USB, lo activa
            if ((USBGetDeviceState() == DETACHED_STATE)) {
                USBDeviceAttach();
            } else {
                // Si ya se ha activado, realiza las tareas USB
                USB_process();
                // FIXME En la PCB no hay leds para el estado del USB, habrá que ponerlos??.
                // AL usar low speed usb, los intervalos son muy largos
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
