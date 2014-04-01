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

/** Procesamiento de información USB  */
void USB_process(void) {
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
    // Recibe un buffer de tamaño determinado
    numBytesRead = getsUSBUSART((char*)usbOutputBuffer.data, 64);

    // Si ha leído datos
    if (numBytesRead != 0) {
        if (strncmp((char*)usbOutputBuffer.data, RTCC_CONF, strlen(RTCC_CONF)) == 0) {
            Rtc_readInputStream(&usbOutputBuffer);
            Rtc_writeFormattedTimestamp(&usbInputBuffer);
        } else if (strncmp((char*)usbOutputBuffer.data, RTCC_TEST, strlen(RTCC_TEST)) == 0) {
            Rtc_readTimestamp();
            Rtc_writeFormattedTimestamp(&usbInputBuffer);
        } else if (strncmp((char*)usbOutputBuffer.data, XBEE_JOIN, strlen(XBEE_JOIN)) == 0) {
            XBee_join();
            Payload_putString(&usbInputBuffer, (UINT8*) "Join request sent");
        } else if (strncmp((char*)usbOutputBuffer.data, ADC_TEST, strlen(ADC_TEST)) == 0) {
           // TODO
            Payload_putString(&usbInputBuffer, (UINT8*) "Adc test received");
        } else if (strncmp((char*)usbOutputBuffer.data, SHT, strlen(SHT)) == 0) {
#if SHT_ENABLED
            Sht11_measure(&sht);
            Sht11_addMeasuresCalculatedToPayload(&sht, &usbInputBuffer);
#else
            Payload_putString(&usbInputBuffer, (UINT8*) "SHT11 not installed");
#endif
        } else {
            // Si el comando es erróneo, muestra un mensaje de error
            Payload_putString(&usbInputBuffer, (UINT8*) "Comando desconocido");
        }
        // Si está preparado para enviar datos
        if (USBUSARTIsTxTrfReady() && usbInputBuffer.size != 0) {
            putUSBUSART((char*)usbInputBuffer.data, (BYTE) usbInputBuffer.size);
        }
    }

    CDCTxService();
}

#endif
