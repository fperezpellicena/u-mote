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
#include "GenericTypeDefs.h"
#include "hw_profile.h"
#include "usb_config.h"
#include "usb_device.h"
#include "usb.h"
#include "usb_function_cdc.h"
#include "usb_rtcc_parser.h"


#pragma udata
char USB_In_Buffer[64];
char USB_Out_Buffer[64];

/* Definición de la función para procesamiento de información USB */
void processUSBData(void);
void blinkUSBStatus(void);

/**
 * Implementación de la función de procesamiento de información USB
 */
void processUSBData(void) {
    char RTCC[] = "rtcc";
    char GPS[] = "gps";
    BYTE numBytesRead;
    // User Application USB tasks
    if ((USBDeviceState < CONFIGURED_STATE) || (USBSuspendControl == 1)) {
        return;
    }
    // Si está preparado para recibir datos
    if (mUSBUSARTIsTxTrfReady()) {
        // Recibe un buffer de tama?o determinado
        numBytesRead = getsUSBUSART(USB_Out_Buffer, 64);
        // Si ha leído datos
        if (numBytesRead != 0) {
            // FIXME Usar constantes
            if (strncmp(USB_Out_Buffer, RTCC, strlen(RTCC)) == 0) {
                parseRTCCData(USB_Out_Buffer);
            } else if(strncmp(USB_Out_Buffer, GPS, strlen(GPS)) == 0) {
                parseGPSData(USB_Out_Buffer);
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
