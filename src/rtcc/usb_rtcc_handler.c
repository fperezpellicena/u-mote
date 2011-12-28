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
#include <stdlib.h>
#include <rtcc.h>
#include "usb_rtcc_handler.h"

rom const char* SEPARATOR = "#";

/**
 * Recupera la fecha/hora del buffer USB y la establece en el sistema.
 *
 * @param usbBuffer
 */
void USBRtccHandler_parseRTCCData(char* usbBuffer) {
    rtccTimeDate timestamp;
    char* result = NULL;
    result = strtokpgmram(usbBuffer, (rom const char far*)SEPARATOR);
    result = strtokpgmram(NULL, (rom const char far*)SEPARATOR);
    timestamp.f.hour = atoi(result);
    result = strtokpgmram(NULL, (rom const char far*)SEPARATOR);
    timestamp.f.min = atoi(result);
    // TODO Continuar con el resto
    RtccWriteTimeDate(&timestamp,TRUE);
}

/**
 * Recupera la fecha/hora del sistema y la pone en el buffer USB para envío.
 *
 * @param usbInBuffer
 */
unsigned char USBRtccHandler_testRTCC(char* usbBuffer) {
    unsigned char i = 0;
    // Lee la fecha/hora
    rtccTimeDate timestamp;
    RtccReadTimeDate(&timestamp);
    // Pone ddmmyy en el buffer
    usbBuffer[i++] = timestamp.f.mday;
    usbBuffer[i++] = timestamp.f.mon;
    usbBuffer[i++] = timestamp.f.year;
    // Pone hhmmss
    usbBuffer[i++] = timestamp.f.hour;
    usbBuffer[i++] = timestamp.f.min;
    usbBuffer[i++] = timestamp.f.sec;
    // Devuelve el tamaño del contenido
    return i;
}
