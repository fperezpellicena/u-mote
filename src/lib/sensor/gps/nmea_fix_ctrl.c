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
 
 /*..........................................................................*/
void NMEACommand_createSetFixCtlFrame(NMEACommandPacket* nmeaCommandFrame,
	UINT8* fixInterval) {
    // Padding data
    UINT8* padding = ",0,0,0,0";
    // Padding length
    UINT8 paddingLength = 9;
    // Prepare data
    UINT8 data[11];
    UINT8 i = 0;
    while (i < NMEA_FIX_SIZE) {
	data[i] = fixInterval[i];
	i++;
    }
    // Set padding data
    i = 0;
    while (i < paddingLength) {
	data[NMEA_FIX_SIZE + i] = padding[i];
	i++;
    }
    // Create command
    NMEACommand_create(nmeaCommandFrame, (UINT8 rom*)NMEA_SET_OUTPUT, data, NMEA_FIX_SIZE + paddingLength);
}