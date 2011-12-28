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
#include "GenericTypeDefs.h"
#include "gps.h"
#include "util.h"

/*..........................................................................*/
void NMEAOutputConfig_create(NMEAOutputConfig* config, unsigned char nmeaGLL,
        unsigned char nmeaRMC, unsigned char nmeaVTG, unsigned char nmeaGGA,
        unsigned char nmeaGSA, unsigned char nmeaGSV, unsigned char nmeaGRS,
        unsigned char nmeaGST, unsigned char nmeaMALM, unsigned char nmeaMEPH,
        unsigned char nmeaMDGP, unsigned char nmeaMDBG, unsigned char nmeaZDA,
        unsigned char nmeaMCHN) {
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
static void NMEACommand_create(NMEACommandPacket * nmeaCommandFrame,
        unsigned char rom* command, unsigned char* data, unsigned char length);

/*..........................................................................*/
static void NMEACommand_create(NMEACommandPacket* nmeaCommandFrame,
        unsigned char rom* command, unsigned char* data, unsigned char length) {
    // Set command number
    unsigned char* p = nmeaCommandFrame->commandNumber;
    unsigned char i = 0;
    //unsigned char* pCommand = command;
    while (i < NMEA_COMMAND_LENGTH) {
        *p++ = command[i];
        i++;
    }
    // Set data
    i = 0;
    p = nmeaCommandFrame->data;
    while (i < length) {
        *p++ = data[i];
        i++;
    }
    // Set length
    nmeaCommandFrame->length = length;
}

/*..........................................................................*/
void NMEACommand_createTest(NMEACommandPacket* nmeaCommandFrame) {
    NMEACommand_create(nmeaCommandFrame, (unsigned char rom*)NMEA_TEST, NULL, 0);
}

/*..........................................................................*/
void NMEACommand_createHotStartFrame(NMEACommandPacket* nmeaCommandFrame) {
    NMEACommand_create(nmeaCommandFrame, (unsigned char rom*)NMEA_HOT_START, NULL, 0);
}

/*..........................................................................*/
void NMEACommand_createWarmStartFrame(NMEACommandPacket* nmeaCommandFrame) {
    NMEACommand_create(nmeaCommandFrame, (unsigned char rom*)NMEA_WARM_START, NULL, 0);
}

/*..........................................................................*/
void NMEACommand_createColdStartFrame(NMEACommandPacket* nmeaCommandFrame) {
    NMEACommand_create(nmeaCommandFrame, (unsigned char rom*)NMEA_COLD_START, NULL, 0);
}

/*..........................................................................*/
void NMEACommand_createFullColdStartFrame(NMEACommandPacket* nmeaCommandFrame) {
    NMEACommand_create(nmeaCommandFrame, (unsigned char rom*)NMEA_FULL_COLD_START, NULL, 0);
}

/*..........................................................................*/
void NMEACommand_createClearEPOFrame(NMEACommandPacket* nmeaCommandFrame) {
    unsigned char* data = "0";
    NMEACommand_create(nmeaCommandFrame, (unsigned char rom*)NMEA_CLEAR_EPO, data, 1);
}

/*..........................................................................*/
void NMEACommand_createSetBaudrateFrame(NMEACommandPacket* nmeaCommandFrame,
        unsigned char* baudrate) {
    NMEACommand_create(nmeaCommandFrame, (unsigned char rom*)NMEA_SET_BAUDRATE, baudrate,
            strlen(baudrate));
}

/*..........................................................................*/
void NMEACommand_createSetFixCtlFrame(NMEACommandPacket* nmeaCommandFrame,
        unsigned char* fixInterval) {
    // Padding data
    unsigned char* padding = ",0,0,0,0";
    // Padding length
    unsigned char paddingLength = 9;
    // Prepare data
    unsigned char data[11];
    unsigned char i = 0;
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
    NMEACommand_create(nmeaCommandFrame, (unsigned char rom*)NMEA_SET_OUTPUT, data, NMEA_FIX_SIZE + paddingLength);
}

/*..........................................................................*/
void NMEACommand_createSetDgpsModeFrame(NMEACommandPacket* nmeaCommandFrame,
        unsigned char* mode) {
    // Create command
    NMEACommand_create(nmeaCommandFrame, (unsigned char rom*)NMEA_SET_FIX_CTL, mode, 1);
}

/*..........................................................................*/
void NMEACommand_createSetSbasFrame(NMEACommandPacket* nmeaCommandFrame,
        BOOL enabled) {
    // Prepare data
    unsigned char* data = enabled ? "1" : "0";
    // Create command
    NMEACommand_create(nmeaCommandFrame, (unsigned char rom*)NMEA_SBAS, data, 1);
}

/*..........................................................................*/
void NMEACommand_createSetOutput(NMEACommandPacket* nmeaCommandFrame,
        NMEAOutputConfig* config) {
    // Prepare data
    unsigned char data[] = {
        config->nmeaGLL, ',', config->nmeaRMC, ',', config->nmeaVTG, ',',
        config->nmeaGGA, ',', config->nmeaGSA, ',', config->nmeaGSV, ',',
        config->nmeaGRS, ',', config->nmeaGST, ',',
        // Padding data not specified
        '1', ',', '1', ',', '1', ',', '1', ',', '1', ',', '1', ',',
        config->nmeaMALM, ',', config->nmeaMEPH, ',', config->nmeaMDGP, ',',
        config->nmeaMDBG, ',', config->nmeaZDA, ',', config->nmeaMCHN
    };
    // Create command
    NMEACommand_create(nmeaCommandFrame, (unsigned char rom*)NMEA_SET_OUTPUT,
            data, NMEA_OUTPUT_CFG_LENGTH);
}

/*..........................................................................*/
void NMEACommand_createSetDatum(NMEACommandPacket* nmeaCommandFrame,
        unsigned char rom* datum) {
    // Prepare data
    unsigned char data[NMEA_DATUM_LENGTH];
    Util_str2ram(datum, data);
    // Create command
    NMEACommand_create(nmeaCommandFrame, (unsigned char rom*)NMEA_SET_OUTPUT,
            data, NMEA_OUTPUT_CFG_LENGTH);
}

