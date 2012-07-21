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
    timestamp.f.hour = 0x0A;
    timestamp.f.min = 9;
    timestamp.f.sec = 0x1E;
    timestamp.f.year = 12;
    timestamp.f.mon = 2;
    timestamp.f.mday = 18;
    timestamp.f.wday = 6;
    Rtc_write(&timestamp);
    Rtc_enable();
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
    // wait while RTCC registers are unsafe to read
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
    // Write rtcc
    EECON2 = 0x55;
    EECON2 = 0xAA;
    RTCCFGbits.RTCWREN = 1;
    RtccWriteTimeDate(timestamp, TRUE);
}
