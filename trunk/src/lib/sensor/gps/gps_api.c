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
#include "gps_api.h"
#include "bsp.h"

/*..........................................................................*/
void NMEAOutputConfig_create(NMEAOutputConfig* config, UINT8 nmeaGLL,
	UINT8 nmeaRMC, UINT8 nmeaVTG, UINT8 nmeaGGA,
	UINT8 nmeaGSA, UINT8 nmeaGSV, UINT8 nmeaGRS,
	UINT8 nmeaGST, UINT8 nmeaMALM, UINT8 nmeaMEPH,
	UINT8 nmeaMDGP, UINT8 nmeaMDBG, UINT8 nmeaZDA,
	UINT8 nmeaMCHN) {
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
static void NMEACommand_create(NMEACommandPacket * nmeaCommandFrame,
	UINT8 rom* command, UINT8* data, UINT8 length);

/*..........................................................................*/
static void NMEACommand_create(NMEACommandPacket* nmeaCommandFrame,
	UINT8 rom* command, UINT8* data, UINT8 length) {
    // Set command number
    UINT8* p = nmeaCommandFrame->commandNumber;
    UINT8 i = 0;
    // Set id calculated chk
    nmeaCommandFrame->checksum = NMEA_ID_CHK;
    while (i < NMEA_COMMAND_LENGTH) {
	*p++ = command[i];
	nmeaCommandFrame->checksum ^= command[i];
	i++;
    }
    // Set data
    i = 0;
    p = nmeaCommandFrame->data;
    while (i < length) {
	*p++ = data[i];
	nmeaCommandFrame->checksum ^= data[i];
	i++;
    }
    // Set checksum string
    hexToString(nmeaCommandFrame->checksumString, nmeaCommandFrame->checksum);
    // Set length
    nmeaCommandFrame->length = length;
}

/*..........................................................................*/
void NMEACommand_createTest(NMEACommandPacket* nmeaCommandFrame) {
    NMEACommand_create(nmeaCommandFrame, (UINT8 rom*)NMEA_TEST, NULL, 0);
}

/*..........................................................................*/
void NMEACommand_createHotStartFrame(NMEACommandPacket* nmeaCommandFrame) {
    NMEACommand_create(nmeaCommandFrame, (UINT8 rom*)NMEA_HOT_START, NULL, 0);
}

/*..........................................................................*/
void NMEACommand_createWarmStartFrame(NMEACommandPacket* nmeaCommandFrame) {
    NMEACommand_create(nmeaCommandFrame, (UINT8 rom*)NMEA_WARM_START, NULL, 0);
}

/*..........................................................................*/
void NMEACommand_createColdStartFrame(NMEACommandPacket* nmeaCommandFrame) {
    NMEACommand_create(nmeaCommandFrame, (UINT8 rom*)NMEA_COLD_START, NULL, 0);
}

/*..........................................................................*/
void NMEACommand_createFullColdStartFrame(NMEACommandPacket* nmeaCommandFrame) {
    NMEACommand_create(nmeaCommandFrame, (UINT8 rom*)NMEA_FULL_COLD_START, NULL, 0);
}

/*..........................................................................*/
void NMEACommand_createClearEPOFrame(NMEACommandPacket* nmeaCommandFrame) {
    UINT8* data = "0";
    NMEACommand_create(nmeaCommandFrame, (UINT8 rom*)NMEA_CLEAR_EPO, data, 1);
}

/*..........................................................................*/
void NMEACommand_createSetBaudrateFrame(NMEACommandPacket* nmeaCommandFrame,
	UINT8* baudrate) {
    NMEACommand_create(nmeaCommandFrame, (UINT8 rom*)NMEA_SET_BAUDRATE, baudrate,
	    (UINT8)strlen(baudrate));
}

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

/*..........................................................................*/
void NMEACommand_createSetDgpsModeFrame(NMEACommandPacket* nmeaCommandFrame,
	UINT8* mode) {
    // Create command
    NMEACommand_create(nmeaCommandFrame, (UINT8 rom*)NMEA_SET_FIX_CTL, mode, 1);
}

/*..........................................................................*/
void NMEACommand_createSetSbasFrame(NMEACommandPacket* nmeaCommandFrame,
	BOOL enabled) {
    // Prepare data
    UINT8* data = enabled ? "1" : "0";
    // Create command
    NMEACommand_create(nmeaCommandFrame, (UINT8 rom*)NMEA_SBAS, data, 1);
}

/*..........................................................................*/
void NMEACommand_createSetOutput(NMEACommandPacket* nmeaCommandFrame,
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
    NMEACommand_create(nmeaCommandFrame, (UINT8 rom*)NMEA_SET_OUTPUT,
	    data, NMEA_OUTPUT_CFG_LENGTH);
}

/*..........................................................................*/
void NMEACommand_createSetDatum(NMEACommandPacket* nmeaCommandFrame,
	UINT8 rom* datum) {
    // Prepare data
    UINT8 data[NMEA_DATUM_LENGTH];
    strcpypgm2ram((char*)datum, (MOTE_MEM_MODEL rom char*)data);
    // Create command
    NMEACommand_create(nmeaCommandFrame, (UINT8 rom*)NMEA_SET_OUTPUT,
	    data, NMEA_OUTPUT_CFG_LENGTH);
}