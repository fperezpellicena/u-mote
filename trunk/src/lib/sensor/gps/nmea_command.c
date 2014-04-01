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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "nmea_command.h"
#include "bsp.h"

/*..........................................................................*/
static void hexToString(UINT8* str, UINT8 hex);

/*..........................................................................*/
static void hexToString(UINT8* str, UINT8 hex) {
    UINT8 dec = hex >> 4;
    UINT8 uni = hex & 0x0F;
    itoa(dec, (char*)&str[0]);
    itoa(uni, (char*)&str[1]);
}

/*..........................................................................*/
void NMEACommand_create(NMEACommandPacket* packet,
	UINT8 rom* command, UINT8* data, UINT8 length) {
    // Set command number
    UINT8* p = packet->commandNumber;
    UINT8 i = 0;
    // Set id calculated chk
    packet->checksum = NMEA_ID_CHK;
    while (i < NMEA_COMMAND_LENGTH) {
	*p++ = command[i];
	packet->checksum ^= command[i];
	i++;
    }
    // Set data
    i = 0;
    p = packet->data;
    while (i < length) {
	*p++ = data[i];
	packet->checksum ^= data[i];
	i++;
    }
    // Set checksum string
    hexToString(packet->checksumString, packet->checksum);
    // Set length
    packet->length = length;
}

/*..........................................................................*/
void NMEACommandConfig_create(NMEACommandConfig* config, UINT8 nmeaGLL,
	UINT8 nmeaRMC, UINT8 nmeaVTG, UINT8 nmeaGGA,UINT8 nmeaGSA, UINT8 nmeaGSV,
	UINT8 nmeaGRS, UINT8 nmeaGST, UINT8 nmeaMALM, UINT8 nmeaMEPH,
	UINT8 nmeaMDGP, UINT8 nmeaMDBG, UINT8 nmeaZDA, UINT8 nmeaMCHN) {
    config->nmeaGLL = nmeaGLL;
    config->nmeaRMC = nmeaRMC;
    config->nmeaVTG = nmeaVTG;
    config->nmeaGGA = nmeaGGA;
    config->nmeaGSA = nmeaGSA;
    config->nmeaGSV = nmeaGSV;
    config->nmeaGRS = nmeaGRS;
    config->nmeaGST = nmeaGST;
    config->nmeaMALM = nmeaMALM;
    config->nmeaMEPH = nmeaMEPH;
    config->nmeaMDGP = nmeaMDGP;
    config->nmeaMDBG = nmeaMDBG;
    config->nmeaZDA = nmeaZDA;
    config->nmeaMCHN = nmeaMCHN;
}
