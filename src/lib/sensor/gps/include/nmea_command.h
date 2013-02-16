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
 *
 * On power-up, GPS module sends out through UART:
 */

#ifndef nmea_command_h
#define nmea_command_h

#include "GenericTypeDefs.h"

/*...........................................................................*/
#define NMEA_MAX_COMMAND_LENGTH	    50		/* NMEA Max length frame */
#define NMEA_PREAMBLE		    '$'		/* NMEA command preamble */
#define NMEA_CHK_CHAR		    '*'	           /* NMEA chk delimiter */
#define NMEA_COMMA_CHAR		    ','		 /* NMEA comma delimiter */

/*
 * This will identify for the NMEA parser that it is command for MediaTek.
 * Four bytes character string.
 */
#define NMEA_ID                 "PMTK"                    /* NMEA command id */
#define NMEA_ID_CHK		0x02                  /* NMEA command id chk */
#define NMEA_ID_LENGTH		4                     /* NMEA id length */
#define NMEA_COMMAND_LENGTH     3                     /* NMEA command length */
#define NMEA_CHECKUM_LENGTH     2                     /* NMEA checkum lenght */
/* The two bytes are used to identify the end of a command. */
#define NMEA_CR                 13                        /* Carriage return */
#define NMEA_LF                 10                              /* Line feed */

/*...........................................................................*/
// NMEA baudrates
#define NMEA_BAUD_DEFAULT       "0"
#define NMEA_BAUD_4800          "4800"
#define NMEA_BAUD_9600          "9600"
#define NMEA_BAUD_14400         "14400"
#define NMEA_BAUD_19200         "19200"
#define NMEA_BAUD_38400         "38400"
#define NMEA_BAUD_57600         "57600"
#define NMEA_BAUD_115200        "115200"

/*...........................................................................*/
// NMEA Commands
#define NMEA_TEST               "000"
#define NMEA_ACK                "001"
#define NMEA_STARTUP            "010"
#define NMEA_HOT_START          "101"
#define NMEA_WARM_START         "102"
#define NMEA_COLD_START         "103"
#define NMEA_FULL_COLD_START    "104"
#define NMEA_CLEAR_EPO          "127"
#define NMEA_SET_BAUDRATE       "251"
#define NMEA_SET_FIX_CTL        "300"
#define NMEA_DGPS_MODE          "301"
#define NMEA_SBAS               "313"
#define NMEA_SET_OUTPUT         "314"
#define NMEA_SET_DATUM          "330"

/*...........................................................................*/
// NMEA DGPS modes
#define NMEA_NO_DGPS_SOURCE     "0"
#define NMEA_RTCM               "1"
#define NMEA_WAAS               "2"

/*...........................................................................*/
// NMEA fix interval msec.
#define NMEA_5_HZ               "200"
#define NMEA_10_HZ              "100"
#define NMEA_FIX_SIZE           3

/*...........................................................................*/
// NMEA common datums
#define NMEA_DATUM_WGS1984      "0"
#define NMEA_DATUM_TOKIO_M      "1"
#define NMEA_DATUM_TOKIO_A      "2"
// NMEA datum length
#define NMEA_DATUM_LENGTH       3

/*...........................................................................*/
// NMEA output config PMTK314
typedef struct NMEAOutputConfig NMEAOutputConfig;

struct NMEAOutputConfig {
    /* GPGLL interval - Geographic Position - Latitude longitude */
    UINT8 nmeaGLL;
    /* GPRMC interval - Recomended Minimum Specific GNSS Sentence */
    UINT8 nmeaRMC;
    /* GPVTG interval - Course Over Ground and Ground Speed */
    UINT8 nmeaVTG;
    /* GPGGA interval - GPS Fix Data */
    UINT8 nmeaGGA;
    /* GPGSA interval - GNSS DOPS and Active Satellites */
    UINT8 nmeaGSA;
    /* GPGSA interval - GNSS DOPS and Active Satellites */
    UINT8 nmeaGSV;
    /* GPGRS interval - GNSS Range Residuals */
    UINT8 nmeaGRS;
    /* GPGST interval - GNSS Pseudorange Erros Statistics */
    UINT8 nmeaGST;
    /* PMTKALM interval - GPS almanac information (not supported) */
    UINT8 nmeaMALM;
    /* PMTKEPH interval - GPS ephmeris information (not supported) */
    UINT8 nmeaMEPH;
    /* PMTKDGP interval - GPS differential correction information (not supported) */
    UINT8 nmeaMDGP;
    /* PMTKDBG interval ? MTK debug information (not supported) */
    UINT8 nmeaMDBG;
    /* GPZDA interval ? Time & Date */
    UINT8 nmeaZDA;
    /* PMTKCHN interval ? GPS channel status */
    UINT8 nmeaMCHN;
};

#define NMEA_OUTPUT_CFG_LENGTH         38                  /* Config length */
// Supported freqcuency setting
#define NMEA_OUTPUT_DISABLED            '0'
#define NMEA_OUTPUT_EVERY_ONE_POS_FIX   '1'
#define NMEA_OUTPUT_EVERY_TWO_POS_FIX   '2'
#define NMEA_OUTPUT_EVERY_THREE_POS_FIX '3'
#define NMEA_OUTPUT_EVERY_FOUR_POS_FIX  '4'
#define NMEA_OUTPUT_EVERY_FIVE_POS_FIX  '5'


void NMEAOutputConfig_create(NMEAOutputConfig* config, UINT8 nmeaGLL,
	UINT8 nmeaRMC, UINT8 nmeaVTG, UINT8 nmeaGGA,
	UINT8 nmeaGSA, UINT8 nmeaGSV, UINT8 nmeaGRS,
	UINT8 nmeaGST, UINT8 nmeaMALM, UINT8 nmeaMEPH,
	UINT8 nmeaMDGP, UINT8 nmeaMDBG, UINT8 nmeaZDA,
	UINT8 nmeaMCHN);

/*...........................................................................*/
typedef struct NMEACommandPacket NMEACommandPacket;

struct NMEACommandPacket {
    /*
     * Three bytes character string.From ?000? to ?999?.
     * An identifier used to tell the decoder how to decode the command
     */
    UINT8 commandNumber[NMEA_COMMAND_LENGTH]; /* Command number */
    /*
     * The DataField has variable length depending on the command type.
     * A comma symbol ?,? must be inserted ahead each data filed to help
     * the decoder process the DataField. *: 1 byte character.
     * The star symbol is used to mark the end of DataField.
     */
    UINT8 data[NMEA_MAX_COMMAND_LENGTH]; /* Command data */
    /*
     * Two bytes character string.
     * CHK1 and CHK2 are the checksum of the data between Preamble and ?*?.
     */
    UINT8 checksum; /* Checksum byte */
    UINT8 checksumString[NMEA_CHECKUM_LENGTH]; /* Checksum byte as string */
    UINT8 length; /* Frame length */
};

/*..........................................................................*/
void NMEACommand_create(NMEACommandPacket * packet,
	UINT8 rom* command, UINT8* data, UINT8 length);

void NMEACommand_createTest(NMEACommandPacket* packet);

void NMEACommand_createHotStartFrame(NMEACommandPacket* packet);

void NMEACommand_createWarmStartFrame(NMEACommandPacket* packet);

void NMEACommand_createColdStartFrame(NMEACommandPacket* packet);

void NMEACommand_createFullColdStartFrame(NMEACommandPacket* packet);

void NMEACommand_createClearEPOFrame(NMEACommandPacket* packet);

void NMEACommand_createSetBaudrateFrame(NMEACommandPacket* packet,
	UINT8* baudrate);

void NMEACommand_createSetFixCtlFrame(NMEACommandPacket* packet,
	UINT8* fixInterval);

void NMEACommand_createSetDgpsModeFrame(NMEACommandPacket* packet,
	UINT8* mode);

void NMEACommand_createSetSbasFrame(NMEACommandPacket* packet,
	BOOL enabled);

void NMEACommand_createSetOutput(NMEACommandPacket* packet,
	NMEAOutputConfig* config);

void NMEACommand_createSetDatum(NMEACommandPacket* packet,
	UINT8 rom* datum);


#endif /* nmea_command_h */
