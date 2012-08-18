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

#include "rtc.h"
#include "GenericTypeDefs.h"
#include "list.h"
#include <string.h>
#include <stdlib.h>

rom const CHAR8* SEPARATOR = "#";

/*..........................................................................*/

/* Init and enable RTC */
void Rtc_init() {
    rtccTimeDate timestamp;

    timestamp.f.hour = 0x01;
    timestamp.f.min = 0x00;
    timestamp.f.sec = 0x00;
    timestamp.f.year = 0x09;
    timestamp.f.mon = 0x01;
    timestamp.f.mday = 0x01;
    timestamp.f.wday = 0x12;

    // first initial power up of the device.
    // unlock the RTCC registers so that we can write to them
    EECON2 = 0x55;
    EECON2 = 0xAA;
    RTCCFGbits.RTCWREN = 1;

    // reset RTCC date/time (only on first power on)
    RTCCFGbits.RTCPTR1 = 1;
    RTCCFGbits.RTCPTR0 = 1;

    Rtc_write(&timestamp);

    Rtc_enable(); // enable RTCC module
    RTCCAL = 0;
}

/*..........................................................................*/

/* Enable RTC */
void Rtc_enable() {
    RTCCFGbits.RTCEN = 1;
}

/*..........................................................................*/

/* Read Rtc data into parameter */
void Rtc_read(rtccTimeDate* timestamp) {
    // if already in the middle of SYNC, wait for next period
    while (RTCCFGbits.RTCSYNC != 1);
    // wait while RTCC registers are safe to read
    while (RTCCFGbits.RTCSYNC == 1);
    RtccReadTimeDate(timestamp);
}

/*..........................................................................*/

/* Read Rtc data into a list */
void Rtc_readToList(List* list) {
    rtccTimeDate timestamp;
    Rtc_read(&timestamp);
    List_add(list, timestamp.f.mday);
    List_add(list, timestamp.f.mon);
    List_add(list, timestamp.f.year);
    List_add(list, timestamp.f.hour);
    List_add(list, timestamp.f.min);
    List_add(list, timestamp.f.sec);
}

/*..........................................................................*/

/* Set Rtc data from parameter */
void Rtc_write(rtccTimeDate* timestamp) {
    RTCVALL = timestamp->b[0];
    RTCVALH = timestamp->b[1];
    RTCVALL = timestamp->b[2];
    RTCVALH = timestamp->b[3];
    RTCVALL = timestamp->b[4];
    RTCVALH = timestamp->b[5];
    RTCVALL = timestamp->b[6];
    RTCVALH = timestamp->b[7];
}

/**
 * Recupera la fecha/hora del buffer USB y la establece en el sistema.
 *
 * Recibe una trama con el siguiente formato:
 *      rtccconfig#mday#12#wday#6#mon#2#year#12#hour#12#min#10#sec#00
 *      rtccconfig#12#06#02#12#12#10#00
 * equivalente a: 20/5/2011 12:10:00
 *
 * TODO Se podría implementar comprobación de los datos recibidos
 * PreCondition:    rtccTimeDate structure fields have to have proper values:
 *      sec:  BCD codification, 00-59
 *      min:  BCD codification, 00-59
 *      hour: BCD codification, 00-24
 *      wday: BCD codification, 00-06
 *      mday: BCD codification, 01-31
 *      mon: BCD codification, 01-12
 *      year: BCD codification, 00-99
 *
 * @param usbBuffer
 */
void Rtc_usbParse(char* usbBuffer) {
    rtccTimeDate timestamp;
    char* result = NULL;
    // Skip rtcc SOF(start of frame)
    result = strtokpgmram(usbBuffer, (rom const char far*)SEPARATOR);
    // Read mday
    result = strtokpgmram(NULL, (rom const char far*)SEPARATOR);
    timestamp.f.mday = atoi(result);
    // Read wday
    result = strtokpgmram(NULL, (rom const char far*)SEPARATOR);
    timestamp.f.wday = atoi(result);
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
    Rtc_write(&timestamp);
    RTCCFGbits.RTCEN = 1;
}

/**
 * Recupera la fecha/hora del sistema y la pone en el buffer USB para envío.
 *
 * @param usbInBuffer
 */
UINT8 Rtc_usbReadTest(char* usbBuffer) {
    UINT8 i = 0;
    // Lee la fecha/hora
    rtccTimeDate timestamp;
    Rtc_read(&timestamp);
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
