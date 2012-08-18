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



#pragma udata
char USB_In_Buffer[64];
char USB_Out_Buffer[64];
#pragma

/**
 * Implementación de la función de procesamiento de información USB
 */
void USB_process(void) {
    unsigned char length;
    static char RTCC_CONF[] = "rtccconfig";
    static char RTCC_TEST[] = "rtcctest";
    static char XBEE_JOIN[] = "xbeejoin";
    static char ADC_TEST[] = "adctest";
    static char SENSOR_TEST[] = "sensortest";
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
            Rtc_usbParse(USB_Out_Buffer);
        } else if (strncmp(USB_Out_Buffer, RTCC_TEST, strlen(RTCC_TEST)) == 0) {
            length = Rtc_usbReadTest(USB_In_Buffer);
        } else if (strncmp(USB_Out_Buffer, XBEE_JOIN, strlen(XBEE_JOIN)) == 0) {
            XBeeProxy_usbJoin(USB_In_Buffer);
        } else if (strncmp(USB_Out_Buffer, ADC_TEST, strlen(ADC_TEST)) == 0) {
            length = Adc_usbTest(USB_In_Buffer);
        } else if (strncmp(USB_Out_Buffer, SENSOR_TEST, strlen(SENSOR_TEST)) == 0) {
            length = SensorProxy_usbTest(USB_In_Buffer);
        } else {
            // Si el comando es erróneo, muestra un mensaje de error
            length = USB_ERROR_MSG_LEN;
            Util_str2ram((UINT8 rom*)USB_ERROR_MSG, (UINT8*) USB_In_Buffer);
        }
        // Si está preparado para enviar datos
        if (USBUSARTIsTxTrfReady() && length != 0) {
            putUSBUSART(USB_In_Buffer, length);
            length = 0;
        }
    }

    CDCTxService();
}
