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

#include "nmea_command.h"
#include <string.h>
#include "bsp.h"

/*..........................................................................*/
void NMEACommand_createSetDatum(NMEACommandPacket* nmeaCommandFrame,
	UINT8 rom* datum) {
    // Prepare data
    UINT8 data[NMEA_DATUM_LENGTH];
    strcpypgm2ram((char*) datum, (MOTE_MEM_MODEL rom char*) data);
    // Create command
    NMEACommand_create(nmeaCommandFrame, (UINT8 rom*)NMEA_SET_OUTPUT,
	    data, NMEA_COMMAND_CFG_LENGTH);
}