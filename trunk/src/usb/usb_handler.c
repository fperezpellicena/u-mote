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

#include <string.h>
#include "bsp.h"
#include "GenericTypeDefs.h"

#include "util.h"
#include "rtc.h"
#include "hw_adc.h"

#include "sensor_proxy.h"
#include "digi_proxy.h"

#include "usb_handler.h"
#include "usb_config.h"
#include "usb_device.h"
#include "usb.h"
#include "usb_function_cdc.h"

#pragma udata buffers
char USB_In_Buffer[64];
char USB_Out_Buffer[64];
#pragma


/*...........................................................................*/

/* Welcome message */
void USB_welcome(void) {
    UINT8 length = sprintf((char*) USB_In_Buffer, "Welcome!");
    // Si está preparado para enviar datos
    if (USBUSARTIsTxTrfReady()) {
        putUSBUSART(USB_In_Buffer, length);
    }
    CDCTxService();
}

/*...........................................................................*/

/** Procesamiento de información USB  */
void USB_process(void) {
    UINT8 length;
    rtccTimeDate timestamp;
    static char RTCC_CONF[] = "rtccconfig";
    static char RTCC_TEST[] = "rtcctest";
    static char XBEE_JOIN[] = "xbeejoin";
    static char ADC_TEST[] = "adctest";
    // Sht-11 test commands
    static char SHT_TMP[] = "shttemp";
    static char SHT_HUM[] = "shthum";

    BYTE numBytesRead;
    // User Application USB tasks
    if ((USBDeviceState < CONFIGURED_STATE) || (USBSuspendControl == 1)) {
        return;
    }
    // Recibe un buffer de tamaño determinado
    numBytesRead = getsUSBUSART(USB_Out_Buffer, 64);

    // Si ha leído datos
    if (numBytesRead != 0) {
        if (strncmp(USB_Out_Buffer, RTCC_CONF, strlen(RTCC_CONF)) == 0) {
            Rtc_usbParse(USB_Out_Buffer, USB_In_Buffer);
            length = sprintf(USB_In_Buffer, "Hora establecida \n\r");
            mLED_3_Toggle();
        } else if (strncmp(USB_Out_Buffer, RTCC_TEST, strlen(RTCC_TEST)) == 0) {
            //length = Rtc_usbReadTest(USB_In_Buffer);
            // Lee la fecha/hora
            Rtc_read(&timestamp);
             // Devuelve el tamaño del contenido
            length = sprintf(USB_In_Buffer, "Hora/fecha: %2u:%2u:%2u %2u/%2u/%4u \n\r", timestamp.f.hour,
                    timestamp.f.min, timestamp.f.sec, timestamp.f.mday, timestamp.f.mon,
                    timestamp.f.year);
        } else if (strncmp(USB_Out_Buffer, XBEE_JOIN, strlen(XBEE_JOIN)) == 0) {
            XBeeProxy_usbJoin(USB_In_Buffer);
        } else if (strncmp(USB_Out_Buffer, ADC_TEST, strlen(ADC_TEST)) == 0) {
            length = Adc_usbTest(USB_In_Buffer);
        } else if (strncmp(USB_Out_Buffer, SHT_TMP, strlen(SHT_TMP)) == 0) {
            length = SensorProxy_usbShtTemperature(USB_In_Buffer);
        } else if (strncmp(USB_Out_Buffer, SHT_HUM, strlen(SHT_HUM)) == 0) {
            length = SensorProxy_usbShtHumidity(USB_In_Buffer);
        }
        //        else {
        //            // Si el comando es erróneo, muestra un mensaje de error
        //            length = USB_ERROR_MSG_LEN;
        //            Util_str2ram((UINT8 rom*)USB_ERROR_MSG, (UINT8*) USB_In_Buffer);
        //        }
        // Si está preparado para enviar datos
        if (USBUSARTIsTxTrfReady() && length != 0) {
            mLED_4_Toggle();
            putUSBUSART(USB_In_Buffer, length);
            length = 0;
        }
    }

    CDCTxService();
}

void USB_blinkStatus(void) {
    static WORD led_count = 0;

    if (led_count == 0)led_count = 2000U; // Ajustado para USB low speed
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
