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
#include "register.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

rom const CHAR8* SEPARATOR = "#";
static rtccTimeDate timestamp;


static void Rtc_initDefaultTimeDate(void);

static void Rtc_initDefaultTimeDate(void) {
    timestamp.f.hour = 0x01;
    timestamp.f.min = 0x00;
    timestamp.f.sec = 0x00;
    timestamp.f.year = 0x09;
    timestamp.f.mon = 0x01;
    timestamp.f.mday = 0x01;
    timestamp.f.wday = 0x12;
}

static void Rtc_resetTimeDatePtr(void);

static void Rtc_resetTimeDatePtr(void) {
    // reset RTCC date/time (only on first power on)
    RTCCFGbits.RTCPTR1 = 1;
    RTCCFGbits.RTCPTR0 = 1;
}

static void Rtc_writeDefatultTimeDate(void);

static void Rtc_writeDefatultTimeDate(void) {
    Register_unLockRTC();
    Rtc_resetTimeDatePtr();
    Rtc_writeTimestamp(&timestamp);
}

/*..........................................................................*/

/* Init and enable RTC */
void Rtc_init(void) {
    Rtc_initDefaultTimeDate();
    Rtc_writeDefatultTimeDate();
    Rtc_enable();
}

/*..........................................................................*/

/* Enable RTC */
void Rtc_enable(void) {
    RTCCFGbits.RTCEN = 1;
    RTCCAL = 0;
}

/*..........................................................................*/

/* Read Rtc data into parameter */
void Rtc_readTimestamp(void) {
    // if already in the middle of SYNC, wait for next period
    while (RTCCFGbits.RTCSYNC != 1);
    // wait while RTCC registers are safe to read
    while (RTCCFGbits.RTCSYNC == 1);
    RtccReadTimeDate(&timestamp);
}

/* Read RTC datetime and return current */
void Rtc_readCurrentTimestamp(rtccTimeDate* current) {
    Rtc_readTimestamp();
    current->f.hour = timestamp.f.hour;
    current->f.min = timestamp.f.min;
    current->f.sec = timestamp.f.sec;
    current->f.year = timestamp.f.year;
    current->f.mon = timestamp.f.mon;
    current->f.mday = timestamp.f.mday;
    current->f.wday = timestamp.f.wday;
}

/*..........................................................................*/

/* Set Rtc data from parameter */
void Rtc_writeTimestamp(rtccTimeDate* timestamp) {
    RTCVALL = timestamp->b[0];
    RTCVALH = timestamp->b[1];
    RTCVALL = timestamp->b[2];
    RTCVALH = timestamp->b[3];
    RTCVALL = timestamp->b[4];
    RTCVALH = timestamp->b[5];
    RTCVALL = timestamp->b[6];
    RTCVALH = timestamp->b[7];
}

/*..........................................................................*/
// FIXME Eliminate payload dependency

/* Read Rtc data into buffer */
void Rtc_writeFormattedTimestamp(Payload* output) {
    char buffer[50];
    memset(buffer, NULL, 50);
    sprintf((char*)buffer, (MOTE_MEM_MODEL rom char*)"Fecha/Hora: %2u:%2u:%2u %2u/%2u/%4u \n\r",
            timestamp.f.hour, timestamp.f.min, timestamp.f.sec,
            timestamp.f.mday, timestamp.f.mon, timestamp.f.year);
    Payload_putString(output, (UINT8*)buffer);
}

/*..........................................................................*/

/* Read Rtc data into a list */
void Rtc_addCurrentTimeToPayload(Payload* payload) {
    Rtc_readTimestamp();
    Rtc_addTimeToPayload(payload, &timestamp);
}

void Rtc_addTimeToPayload(Payload* payload, rtccTimeDate* timestamp) {
    Payload_putByte(payload, timestamp->f.sec);
    Payload_putByte(payload, timestamp->f.min);
    Payload_putByte(payload, timestamp->f.hour);
    Payload_putByte(payload, timestamp->f.mday);
    Payload_putByte(payload, timestamp->f.mon);
    Payload_putByte(payload, timestamp->f.year);
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
 */
void Rtc_readInputStream(Payload* input) {
    char* result = NULL;
    // Skip rtcc SOF(start of frame)
    result = strtokpgmram((char*)input->data, (rom const char far*)SEPARATOR);
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

    // unlock the RTCC registers so that we can write to them
    Register_unLockRTC();
    Rtc_resetTimeDatePtr();
    Rtc_writeTimestamp(&timestamp);
    Rtc_enable(); // enable RTCC module
}
