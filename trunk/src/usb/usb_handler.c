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
#include "usb_handler.h"
#include "usb_config.h"
#include "usb_device.h"
#include "usb.h"
#include "usb_function_cdc.h"
#include "usb_rtcc_handler.h"
#include "usb_gps_handler.h"
#include "usb_digi_handler.h"
#include "usb_sht_handler.h"
#include "util.h"


#pragma udata
char USB_In_Buffer[64];
char USB_Out_Buffer[64];
#pragma

/**
 * Implementación de la función de procesamiento de información USB
 */
void USB_process(void) {
    unsigned char length;
    char RTCC_CONF[] = "rtccconfig";
    char RTCC_TEST[] = "rtcctest";
    char SHT_TEST[] = "shttest";
    char GPS_CONF[] = "gpsconfig";
    char GPS_TEST[] = "gpsconfig";
    //    char SENSOR_LIST[] = "sensorlist";
    //    char SENSOR_TEST[] = "sensortest";
    char XBEE_JOIN[] = "xbeejoin";
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
            USBRtccHandler_parseRTCCData(USB_Out_Buffer);
        } else if (strncmp(USB_Out_Buffer, RTCC_TEST, strlen(RTCC_TEST)) == 0) {
            length = USBRtccHandler_testRTCC(USB_In_Buffer);
        } else if (strncmp(USB_Out_Buffer, GPS_CONF, strlen(GPS_CONF)) == 0) {
            USBGpsHandler_parseGPSData(USB_Out_Buffer);
        } else if (strncmp(USB_Out_Buffer, SHT_TEST, strlen(SHT_TEST)) == 0) {
            length = USBShtHandler_sht11Test(USB_Out_Buffer);
        } else if (strncmp(USB_Out_Buffer, GPS_TEST, strlen(GPS_TEST)) == 0) {
            length = USBGpsHandler_testGPS(USB_In_Buffer);
        } else if (strncmp(USB_Out_Buffer, XBEE_JOIN, strlen(XBEE_JOIN)) == 0) {
            USBXBeeHandler_join(USB_In_Buffer);
        }/* else if (strncmp(USB_Out_Buffer, SENSOR_LIST, strlen(SENSOR_LIST)) == 0) {
            listSensors(USB_In_Buffer);
        } else if (strncmp(USB_Out_Buffer, SENSOR_TEST, strlen(SENSOR_TEST)) == 0) {
            testSensors(USB_In_Buffer);
        } */ else {
            // Si el comando es erróneo, muestra un mensaje de error
            length = USB_ERROR_MSG_LEN;
            Util_str2ram(USB_ERROR_MSG, USB_In_Buffer);
        }
        // Si está preparado para enviar datos
        if (USBUSARTIsTxTrfReady() && length != 0) {
            putUSBUSART(USB_In_Buffer, length);
            length = 0;
        }
    }

    CDCTxService();
}


void USB_blinkStatus(void) {
    static WORD led_count = 0;

    if (led_count == 0)led_count = 100U;    // Ajustado para USB low speed
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
