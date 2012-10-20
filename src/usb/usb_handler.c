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
#include "usb_handler.h"

#if USB_ENABLED

#include <string.h>
#include "GenericTypeDefs.h"
#include "util.h"
#include "hw_adc.h"
#include "usb_handler.h"
#include "usb_config.h"
#include "usb_device.h"
#include "usb.h"
#include "usb_function_cdc.h"

#if RTCC_ENABLED
#include "rtc.h"
#endif

#if SHT_ENABLED
#include "sht.h"
extern Sht sht;
#endif

#include "digi_api.h"

#pragma udata buffers
Payload usbInputBuffer;
Payload usbOutputBuffer;
#pragma



/*...........................................................................*/

/** Procesamiento de informaci�n USB  */
void USB_process(void) {
    rtccTimeDate* timestamp;
    static char RTCC_CONF[] = "rtccconfig";
    static char RTCC_TEST[] = "rtcctest";
    static char XBEE_JOIN[] = "xbeejoin";
    static char ADC_TEST[] = "adctest";
    // Sht-11 test commands
    static char SHT[] = "shttest";

    BYTE numBytesRead;

    // Init payloads
    Payload_init(&usbInputBuffer);
    Payload_init(&usbOutputBuffer);

    // User Application USB tasks
    if ((USBDeviceState < CONFIGURED_STATE) || (USBSuspendControl == 1)) {
        return;
    }
    // Recibe un buffer de tama�o determinado
    numBytesRead = getsUSBUSART(usbOutputBuffer.data, 64);

    // Si ha le�do datos
    if (numBytesRead != 0) {
        if (strncmp(usbOutputBuffer.data, RTCC_CONF, strlen(RTCC_CONF)) == 0) {
            Rtc_readInputStream(&usbOutputBuffer);
            Rtc_writeFormattedTimestamp(&usbInputBuffer);
        } else if (strncmp(usbOutputBuffer.data, RTCC_TEST, strlen(RTCC_TEST)) == 0) {
            Rtc_readTimestamp();
            Rtc_writeFormattedTimestamp(&usbInputBuffer);
        } else if (strncmp(usbOutputBuffer.data, XBEE_JOIN, strlen(XBEE_JOIN)) == 0) {
            XBee_join();
            Payload_putString(&usbInputBuffer, (const MEM_MODEL rom char*) "Join request sent");
        } else if (strncmp(usbOutputBuffer.data, ADC_TEST, strlen(ADC_TEST)) == 0) {
            PAYLOAD_PUT_STRING_WITH_ARGS(&usbInputBuffer,
                    "ADC channel 1: %u\n\r", Adc_testChannelOne());
        } else if (strncmp(usbOutputBuffer.data, SHT, strlen(SHT)) == 0) {
#if SHT_ENABLED
            Sht11_measure(&sht);
            Sht11_addMeasuresCalculatedToPayload(&sht, &usbInputBuffer);
#else
            Payload_putString((const MEM_MODEL rom char*) "SHT11 not installed");
#endif
        } else {
            // Si el comando es err�neo, muestra un mensaje de error
            Payload_putString(&usbInputBuffer, (const MEM_MODEL rom char*) "Comando desconocido");
        }
        // Si est� preparado para enviar datos
        if (USBUSARTIsTxTrfReady() && usbInputBuffer.size != 0) {
            putUSBUSART(usbInputBuffer.data, (BYTE) usbInputBuffer.size);
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

#endif
