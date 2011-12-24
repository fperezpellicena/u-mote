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

#include "GenericTypeDefs.h"
#include "gps.h"

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
static void NMEACommand_create(NMEACommandFrame * nmeaCommandFrame,
        enum NMEACommand command, unsigned char* data, unsigned char length);

/*..........................................................................*/
static void NMEACommand_create(NMEACommandFrame* nmeaCommandFrame,
        enum NMEACommand command, unsigned char* data, unsigned char length) {
    // Set command number
    unsigned char* p = nmeaCommandFrame->commandNumber;
    unsigned char i = NMEA_COMMAND_LENGTH;
    unsigned char* pCommand = NMEACommandValues[command];
    while (i--) {
        *p++ = *pCommand++;
    }
    // Set data
    p = nmeaCommandFrame->data;
    i = length;
    while (i--) {
        *p++ = *data++;
    }
    // Set data length
    nmeaCommandFrame->length = length;
}

/*..........................................................................*/
void NMEACommand_createTest(NMEACommandFrame* nmeaCommandFrame) {
    NMEACommand_create(nmeaCommandFrame, NMEA_TEST, NULL, 0);
}

/*..........................................................................*/
void NMEACommand_createHotStartFrame(NMEACommandFrame* nmeaCommandFrame) {
    NMEACommand_create(nmeaCommandFrame, NMEA_HOT_START, NULL, 0);
}

/*..........................................................................*/
void NMEACommand_createWarmStartFrame(NMEACommandFrame* nmeaCommandFrame) {
    NMEACommand_create(nmeaCommandFrame, NMEA_WARM_START, NULL, 0);
}

/*..........................................................................*/
void NMEACommand_createColdStartFrame(NMEACommandFrame* nmeaCommandFrame) {
    NMEACommand_create(nmeaCommandFrame, NMEA_COLD_START, NULL, 0);
}

/*..........................................................................*/
void NMEACommand_createFullColdStartFrame(NMEACommandFrame* nmeaCommandFrame) {
    NMEACommand_create(nmeaCommandFrame, NMEA_FULL_COLD_START, NULL, 0);
}

/*..........................................................................*/
void NMEACommand_createClearEPOFrame(NMEACommandFrame* nmeaCommandFrame) {
    unsigned char* data = "0";
    NMEACommand_create(nmeaCommandFrame, NMEA_CLEAR_EPO, data, 1);
}

/*..........................................................................*/
void NMEACommand_createSetBaudrateFrame(NMEACommandFrame* nmeaCommandFrame,
        enum NMEABaudrate baudrate) {
    NMEACommand_create(nmeaCommandFrame, NMEA_SET_BAUDRATE,
            NMEABaudrateValues[baudrate],
            strlen(NMEABaudrateValues[baudrate]));
}

/*..........................................................................*/
void NMEACommand_createSetFixCtlFrame(NMEACommandFrame* nmeaCommandFrame,
        unsigned char* fixInterval, unsigned char length) {
    unsigned char* padding = ",0,0,0,0"; // Padding data
    unsigned char paddingLength = 9; // Padding length
    // Set command number
    unsigned char* p = nmeaCommandFrame->commandNumber;
    unsigned char i = NMEA_COMMAND_LENGTH;
    unsigned char* pCommand = NMEACommandValues[NMEA_SET_FIX_CTL];
    while (i--) {
        *p++ = *pCommand++;
    }
    // Set fix interval
    i = length;
    p = nmeaCommandFrame->data;
    while (i--) {
        *p++ = *fixInterval++;
    }
    // Set padding data
    i = paddingLength;
    while (i--) {
        *p++ = *padding++;
    }
    nmeaCommandFrame->length = length + paddingLength;
}

/*..........................................................................*/
void NMEACommand_createSetDgpsModeFrame(NMEACommandFrame* nmeaCommandFrame,
        enum NMEADgpsMode mode) {
    // Set command number
    unsigned char* p = nmeaCommandFrame->commandNumber;
    unsigned char i = NMEA_COMMAND_LENGTH;
    unsigned char* pCommand = NMEACommandValues[NMEA_SET_FIX_CTL];
    while (i--) {
        *p++ = *pCommand++;
    }
    // Set data
    p = nmeaCommandFrame->data;
    *p++ = NMEADgpsModeValues[mode];
    nmeaCommandFrame->length = 1;
}

/*..........................................................................*/
void NMEACommand_createSetSbasFrame(NMEACommandFrame* nmeaCommandFrame,
        BOOL enabled) {
    unsigned char* data = enabled ? "1" : "0";
    NMEACommand_create(nmeaCommandFrame, NMEA_SBAS, data, 1);
}

/*..........................................................................*/
void NMEACommand_createSetOutput(NMEACommandFrame* nmeaCommandFrame,
        NMEAOutputConfig* config) {
    unsigned char data[] = {
        config->nmeaGLL, ',',
        config->nmeaRMC, ',',
        config->nmeaVTG, ',',
        config->nmeaGGA, ',',
        config->nmeaGSA, ',',
        config->nmeaGSV, ',',
        config->nmeaGRS, ',',
        config->nmeaGST, ',',
        // Padding data not specified
        '1', ',', '1', ',', '1', ',', '1', ',', '1', ',', '1', ',',
        config->nmeaMALM, ',',
        config->nmeaMEPH, ',',
        config->nmeaMDGP, ',',
        config->nmeaMDBG, ',',
        config->nmeaZDA, ',',
        config->nmeaMCHN
    };
    // Set command number
    unsigned char* p = nmeaCommandFrame->commandNumber;
    unsigned char i = NMEA_COMMAND_LENGTH;
    unsigned char* pCommand = NMEACommandValues[NMEA_OUTPUT];
    while (i--) {
        *p++ = *pCommand++;
    }
    // Set data
    i = 0;
    while (i < NMEA_OUTPUT_CFG_LENGTH) {
        *p++ = data[i];
        i++;
    }
    nmeaCommandFrame->length = NMEA_OUTPUT_CFG_LENGTH;
}
