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
 #include "nmea_output.h"
#include <string.h>

void NMEAOutput_resetPacket(NMEAOutputPacket * const packet) {
    packet->chkCalculated = 0;
    packet->chkRead = 0;
    packet->rxState = NMEA_PREAMBLE;
    packet->length = 0;
    memset(packet->data, 0, NMEA_OUTPUT_MAX_LENGTH);
}

/*..........................................................................*/
void NMEACommand_createSetOutput(NMEACommandPacket* packet,
	NMEAOutputConfig* config) {
    // Prepare data
    UINT8 data[] = {NMEA_COMMA_CHAR,
	config->nmeaGLL, NMEA_COMMA_CHAR, config->nmeaRMC, NMEA_COMMA_CHAR, config->nmeaVTG, NMEA_COMMA_CHAR,
	config->nmeaGGA, NMEA_COMMA_CHAR, config->nmeaGSA, NMEA_COMMA_CHAR, config->nmeaGSV, NMEA_COMMA_CHAR,
	config->nmeaGRS, NMEA_COMMA_CHAR, config->nmeaGST, NMEA_COMMA_CHAR,
	// Padding data not specified
	'0', NMEA_COMMA_CHAR, '0', NMEA_COMMA_CHAR, '0', NMEA_COMMA_CHAR, '0', NMEA_COMMA_CHAR, '0', NMEA_COMMA_CHAR,
	config->nmeaMALM, NMEA_COMMA_CHAR, config->nmeaMEPH, NMEA_COMMA_CHAR, config->nmeaMDGP, NMEA_COMMA_CHAR,
	config->nmeaMDBG, NMEA_COMMA_CHAR, config->nmeaZDA, NMEA_COMMA_CHAR, config->nmeaMCHN};
    // Create command
    NMEACommand_create(packet, (UINT8 rom*)NMEA_SET_OUTPUT,
	    data, NMEA_OUTPUT_CFG_LENGTH);
}

void NMEAOutput_readRMC(NMEAOutputPacket* output, NMEAOutputRMC* rmc) {}