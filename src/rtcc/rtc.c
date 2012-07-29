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
