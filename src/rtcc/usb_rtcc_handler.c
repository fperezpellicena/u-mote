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
 * Recibe una trama con el siguiente formato:
 *      rtccconfig#mday#20#mon#5#year#11#hour#12#min#10#sec#00
 * equivalente a: 20/5/2011 12:10:00
 *
 * TODO Se podría implementar comprobación de los datos recibidos
 *
 * @param usbBuffer
 */
void USBRtccHandler_parseRTCCData(char* usbBuffer) {
    rtccTimeDate timestamp;
    char* result = NULL;
    // Skip rtcc SOF(start of frame)
    result = strtokpgmram(usbBuffer, (rom const char far*)SEPARATOR);
    // Read mday
    result = strtokpgmram(NULL, (rom const char far*)SEPARATOR);
    timestamp.f.mday = atoi(result);
    // Read mon
    result = strtokpgmram(NULL, (rom const char far*)SEPARATOR);
    timestamp.f.mon = atoi(result);
    // Read year
    result = strtokpgmram(NULL, (rom const char far*)SEPARATOR);
    timestamp.f.year = atoi(result);
    // Read hour
    result = strtokpgmram(NULL, (rom const char far*)SEPARATOR);
    timestamp.f.hour = atoi(result);
    // Read min
    result = strtokpgmram(NULL, (rom const char far*)SEPARATOR);
    timestamp.f.min = atoi(result);
    // Read sec
    result = strtokpgmram(NULL, (rom const char far*)SEPARATOR);
    timestamp.f.sec = atoi(result);
    // Write rtcc
    RtccWriteTimeDate(&timestamp, TRUE);
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
    // Pone dd/mm/yy en el buffer
    usbBuffer[i++] = timestamp.f.mday;
    usbBuffer[i++] = timestamp.f.mon;
    usbBuffer[i++] = timestamp.f.year;
    // Pone hh:mm:ss
    usbBuffer[i++] = timestamp.f.hour;
    usbBuffer[i++] = timestamp.f.min;
    usbBuffer[i++] = timestamp.f.sec;
    // Devuelve el tamaño del contenido
    return i;
}
