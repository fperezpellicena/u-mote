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

#ifndef rtc_h
#define rtc_h

#include <rtcc.h>
#include "payload.h"

/*..........................................................................*/
void Rtc_init(void);

/*..........................................................................*/
void Rtc_enable(void);

/*..........................................................................*/
void Rtc_readTimestamp(void);

/*..........................................................................*/
void Rtc_readCurrentTimestamp(rtccTimeDate* current);

/*..........................................................................*/
void Rtc_writeTimestamp(rtccTimeDate* timestamp);

/*..........................................................................*/
void Rtc_writeFormattedTimestamp(Payload* output);

/*..........................................................................*/
void Rtc_readInputStream(Payload* input);

/*..........................................................................*/
void Rtc_addCurrentTimeToPayload(Payload* payload);

/*..........................................................................*/
void Rtc_addTimeToPayload(Payload* payload, rtccTimeDate* timestamp);

#endif