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

/**
 *
 * Funcionamiento:
 *	Cada mote puede estar en dos modos de funcionamiento:
 *		1.- DEPLOY
 *		2.- RUN
 *
 * MODO DEPLOY
 *
 *
 *
 * MODO RUN
 *
 *	En modo RUN el mote estÃ¡ generalmente dormido, tanto el microcontrolador
 *	como el transceptor XBee.
 *	PeriÃ³dicamente de acuerdo con los parÃ¡metros de configuraciÃ³n del transceptor
 *	(SP y ST), el transceptor XBee se despertarÃ¡ activando el terminal ON/SLEEP.
 *	Esta activaciÃ³n capturada en una interrupciÃ³n del microcontrolador, provoca
 *	la entrada en modo activo del mismo.
 *	Durante el modo activo, el microcontrolador debe enviar la trama de datos
 *	correspondiente a las mediciones realizadas e inmediatamente despuÃ©s
 *	volver al modo de sueÃ±o.
 *	Debido a que el tiempo que estÃ¡ despierto el transceptor XBee estÃ¡ fijado
 *	por la configuraciÃ³n de la red, y que Ã©s el Ãºnico tiempo disponible para
 *	transmitir, puede ser conveniente que la trama estÃ© previamente preparada
 *	para envÃ­o, pudiendo minimizar el tiempo en que el XBee estÃ¡ despierto.
 *
 *	Para ello:
 *		- Se podrÃ­a despertar al micro con anticipaciÃ³n(timer), y preparar la
 *		trama para el siguiente envÃ­o (trabajando con intervalos de sueÃ±o de varios
 *		minutos, no creo que sea relevante hablar de un desfase de varios segundos
 *		o incluso minutos entre el instante de envÃ­o real de la trama y el instante
 *		en que se ha realizado la mediciÃ³n).
 *
 *		- Se podrÃ­a realizar la operaciÃ³n de sensado tras enviar la trama, lo que
 *		significa que se envÃ­a siempre la medida correspondiente a la activaciÃ³n
 *		anterior(esto no es un problema). De esta segunda forma nos ahorramos
 *		el timer y los cambios de contexto de la interrupciÃ³n asociada.
 *
 *			**¿¿ES NECESARIO REVISAR SI EL ENVÃ�O HA SIDO CORRECTO??**
 *			**SI EL ENVÃ�O HA SIDO FALLIDO, QUÉ SE HAR�?A?**
 *
 * Cuando el transceptor XBee se despierta en sincronismo con la red,
 * pone en alto el pin ON/SLEEP(Diego me comenta que no funciona y por ello
 * han empleado el terminal /CTS).
 * El cambio de estado en el transceptor XBee identificador por el cambio en
 * la salida de terminal correspondiente, se puede recoger en un terminal
 * del microcontrolador.
 *
 * Si el micro estÃ¡ en modo Deep Sleep:
 *		Este es el modo de menor consumo en el que la CPU está completamente
 *		desactivada debido a que el regulador propio se apaga.
 *		Sólo se puede salir de este estado si ocurre un hard reset, una alarma
 *		del RTCC, una interrupciÃ³n en INT0, un ULPWU o watchdog reset.
 *		No es necesario un manejador de interrupciones para INT0 y de hecho
 *		es necesario que las interrupciones globales(GIE) estén deshabilitadas.
 *
 * Para configurar el modo Deep Sleep:
 *	- Set REGSLP en WDTCON
 *	- Clear IDLEN
 *	- Clear GIE
 *	- Set DSEN en DSCONH
 *	- Execute Sleep()
 *
 * En el caso de despertar mediante cambio en INT0:
 *	- Set INTEDG0 for a rise edge change or clear for a falling edge
 *	- Set INT0IE to enable INT0 interrupt 
 */

/** CONFIGURATION **************************************************/
#pragma config WDTEN = OFF          //WDT disabled (enabled by SWDTEN bit)
#pragma config PLLDIV = 3           //Divide by 3 (12 MHz oscillator input)
#pragma config STVREN = ON            //stack overflow/underflow reset enabled
#pragma config XINST = OFF          //Extended instruction set disabled
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

/** I N C L U D E S **********************************************************/

#include <string.h>
#include <rtcc.h>
#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "usb_config.h"
#include "usb_device.h"
#include "usb_function_cdc.h"
#include "usb.h"

#include "usb_rtcc_parser.h"

#include "HardwareProfile.h"

/** V A R I A B L E S ********************************************************/
#pragma udata
char USB_In_Buffer[64];
char USB_Out_Buffer[64];

/** P R I V A T E  P R O T O T Y P E S ***************************************/
static void InitializeSystem(void);
void processUSBData(void);
void USBDeviceTasks(void);
void YourHighPriorityISRCode();
void YourLowPriorityISRCode();
void blinkUSBStatus(void);
void UserInit(void);

/** DECLARATIONS ***************************************************/
#pragma code

void main(void) {
    InitializeSystem();

    while (1) {
        // Comprueba el terminal que indica la conexi�n USB al inicio o al reset
        if (PORTBbits.RB4 == 1) {
            // Si no se ha activado el USB, lo activa
            if ((USBGetDeviceState() == DETACHED_STATE)) {
                USBDeviceAttach();
            } else {
                // Si ya se ha activado, realiza las tareas USB
                // USB Tasks
                blinkUSBStatus();
                processUSBData();
            }
        } else {
            // Si no est� conectado el terminal USB, entra en modo de bajo consumo
            USBDeviceDetach();
            LATCbits.LATC0 = 0;
            OSCCONbits.IDLEN = 0;
            Sleep();
            Nop();
        }
    }//end while
}//end main

static void InitializeSystem(void) {
    ADCON1 |= 0x0F; // Default all pins to digital
    {
        unsigned int pll_startup_counter = 600;
        OSCTUNEbits.PLLEN = 1; //Enable the PLL and wait 2+ms until the PLL locks before enabling USB module
        while (pll_startup_counter--);
    }

    ANCON0 = 0xFF; // Default all pins to digital
    ANCON1 = 0xFF; // Default all pins to digital

    // Configura PORTC<0> como salida de test
    TRISCbits.TRISC0 = 0;
    // COnfigura PORTB<4> como entrada para el conector USB
    TRISBbits.TRISB4 = 1;

    UserInit();

    USBDeviceInit(); //usb_device.c.  Initializes USB module SFRs and firmware
    //variables to known states.
}//end InitializeSystem

void UserInit(void) {
    //Initialize all of the LED pins
    mInitAllLEDs();

    //Initialize the pushbuttons
    mInitAllSwitches();
}

rom const char* RTCC = "rtcc";

void processUSBData(void) {
    BYTE numBytesRead;
    // User Application USB tasks
    if ((USBDeviceState < CONFIGURED_STATE) || (USBSuspendControl == 1)) {
        return;
    }
    // Si est� preparado para recibir datos
    if (mUSBUSARTIsTxTrfReady()) {
        // Recibe un buffer de tama�o determinado
        numBytesRead = getsUSBUSART(USB_Out_Buffer, 64);
        // Si ha le�do datos
        if (numBytesRead != 0) {
            // FIXME Usar constantes
            if (strncmppgm2ram(USB_Out_Buffer, RTCC, strlen(RTCC)) == 0) {
                parseRTCCData(USB_Out_Buffer);
            }
        }
    }

    CDCTxService();
}

//Blink the LEDs according to the USB device status

void blinkUSBStatus(void) {
    static WORD led_count = 0;

    if (led_count == 0)led_count = 10000U;
    led_count--;

#define mLED_Both_Off()         {mLED_1_Off();mLED_2_Off();}
#define mLED_Both_On()          {mLED_1_On();mLED_2_On();}
#define mLED_Only_1_On()        {mLED_1_On();mLED_2_Off();}
#define mLED_Only_2_On()        {mLED_1_Off();mLED_2_On();}

    if (USBSuspendControl == 1) {
        if (led_count == 0) {
            mLED_1_Toggle();
            if (mGetLED_1()) {
                mLED_2_On();
            } else {
                mLED_2_Off();
            }
        }//end if
    } else {
        if (USBDeviceState == DETACHED_STATE) {
            mLED_Both_Off();
        } else if (USBDeviceState == ATTACHED_STATE) {
            mLED_Both_On();
        } else if (USBDeviceState == POWERED_STATE) {
            mLED_Only_1_On();
        } else if (USBDeviceState == DEFAULT_STATE) {
            mLED_Only_2_On();
        } else if (USBDeviceState == ADDRESS_STATE) {
            if (led_count == 0) {
                mLED_1_Toggle();
                mLED_2_Off();
            }//end if
        } else if (USBDeviceState == CONFIGURED_STATE) {
            if (led_count == 0) {
                mLED_1_Toggle();
                if (mGetLED_1()) {
                    mLED_2_Off();
                } else {
                    mLED_2_On();
                }
            }//end if
        }//end if(...)
    }//end if(UCONbits.SUSPND...)

}//end BlinkUSBStatus


/** EOF main.c *************************************************/

