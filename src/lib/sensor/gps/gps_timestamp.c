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


#include "gps_timestamp.h"
#include <stdlib.h>

#define HOUR_INDEX          7
#define MINUTE_INDEX        9
#define SECOND_INDEX        11
#define DAY_INDEX           55
#define MONTH_INDEX         57
#define YEAR_INDEX          59

static UINT8 temporalToIntValue(NMEAOutputPacket* packet, UINT8 temporalIndex);

void Gps_readTimestamp(NMEAOutputPacket* packet, rtccTimeDate* timestamp) {
    timestamp->f.hour = temporalToIntValue(packet, HOUR_INDEX);
    timestamp->f.min = temporalToIntValue(packet, MINUTE_INDEX);
    timestamp->f.sec = temporalToIntValue(packet, SECOND_INDEX);
    timestamp->f.mday = temporalToIntValue(packet, DAY_INDEX);
    timestamp->f.mon = temporalToIntValue(packet, MONTH_INDEX);
    timestamp->f.year = temporalToIntValue(packet, YEAR_INDEX);
}

static UINT8 temporalToIntValue(NMEAOutputPacket* packet, UINT8 temporalIndex) {
    UINT8 temporal[3] = {
        packet->data[temporalIndex], packet->data[temporalIndex + 1], '\0'
    };
    return atoi((char*) temporal);
}