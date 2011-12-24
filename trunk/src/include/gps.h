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
#ifndef gps_h
#define gps_h

#include "GenericTypeDefs.h"


/*...........................................................................*/
#define NMEA_MAX_COMMAND_LENGTH     243             /* NMEA Max length frame */
#define NMEA_PREAMBLE   "$"                         /* NMEA command preamble */
#define NMEA_CHK_CHAR   "*"                            /* NMEA chk delimiter */

/*
 * This will identify for the NMEA parser that it is command for MediaTek.
 * Four bytes character string.
 */
#define NMEA_ID                 "PMTK"                    /* NMEA command id */
#define NMEA_COMMAND_LENGTH     3                     /* NMEA command length */
#define NMEA_CHECKUM_LENGTH     2                     /* NMEA checkum lenght */
/* The two bytes are used to identify the end of a command. */
#define NMEA_CR         13                                /* Carriage return */
#define NMEA_LF         10                                      /* Line feed */

/*...........................................................................*/
// NMEA baudrates
enum NMEABaudrate {
    NMEA_BAUD_DEFAULT = 0, NMEA_BAUD_4800, NMEA_BAUD_9600, NMEA_BAUD_14400,
    NMEA_BAUD_19200, NMEA_BAUD_38400, NMEA_BAUD_57600, NMEA_BAUD_115200
};

unsigned char* NMEABaudrateValues[] = {
    "0", "4800", "9600", "14400", "19200", "38400", "57600", "115200"
};

/*...........................................................................*/
enum NMEACommand {
    NMEA_TEST = 0, NMEA_ACK, NMEA_STARTUP, NMEA_HOT_START, NMEA_WARM_START,
    NMEA_COLD_START, NMEA_FULL_COLD_START, NMEA_CLEAR_EPO, NMEA_SET_BAUDRATE,
    NMEA_SET_FIX_CTL,NMEA_DGPS_MODE, NMEA_SBAS, NMEA_OUTPUT
};

unsigned char* NMEACommandValues[] = {
    "000", "001", "010", "101", "102",
    "103", "104", "127", "251",
    "300", "301", "313", "314"
};

// NMEA DGPS modes
enum NMEADgpsMode {
    NO_DGPS_SOURCE = 0, RTCM, WAAS
};

unsigned char NMEADgpsModeValues[] = {
    '0', '1', '2'
};

// NMEA output config PMTK314
typedef struct NMEAOutputConfig NMEAOutputConfig;

struct NMEAOutputConfig {
    /* GPGLL interval - Geographic Position - Latitude longitude */
    unsigned char nmeaGLL;
    /* GPRMC interval - Recomended Minimum Specific GNSS Sentence */
    unsigned char nmeaRMC;
    /* GPVTG interval - Course Over Ground and Ground Speed */
    unsigned char nmeaVTG;
    /* GPGGA interval - GPS Fix Data */
    unsigned char nmeaGGA;
    /* GPGSA interval - GNSS DOPS and Active Satellites */
    unsigned char nmeaGSA;
    /* GPGSA interval - GNSS DOPS and Active Satellites */
    unsigned char nmeaGSV;
    /* GPGRS interval - GNSS Range Residuals */
    unsigned char nmeaGRS;
    /* GPGST interval - GNSS Pseudorange Erros Statistics */
    unsigned char nmeaGST;
    /* PMTKALM interval - GPS almanac information (not supported) */
    unsigned char nmeaMALM;
    /* PMTKEPH interval - GPS ephmeris information (not supported) */
    unsigned char nmeaMEPH;
    /* PMTKDGP interval - GPS differential correction information (not supported) */
    unsigned char nmeaMDGP;
    /* PMTKDBG interval ? MTK debug information (not supported) */
    unsigned char nmeaMDBG;
    /* GPZDA interval ? Time & Date */
    unsigned char nmeaZDA;
     /* PMTKCHN interval ? GPS channel status */
    unsigned char nmeaMCHN;
};

#define NMEA_OUTPUT_CFG_LENGTH         39                  /* Config length */

enum NMEAOutputType {
    DISABLED = 0, EVERY_ONE_POS_FIX, EVERY_TWO_POS_FIX, EVERY_THREE_POS_FIX,
    EVERY_FOUR_POS_FIX, EVERY_FIVE_POS_FIX
};

void NMEAOutputConfig_create(NMEAOutputConfig* config, unsigned char nmeaGLL,
        unsigned char nmeaRMC, unsigned char nmeaVTG, unsigned char nmeaGGA,
        unsigned char nmeaGSA, unsigned char nmeaGSV, unsigned char nmeaGRS,
        unsigned char nmeaGST, unsigned char nmeaMALM, unsigned char nmeaMEPH,
        unsigned char nmeaMDGP, unsigned char nmeaMDBG, unsigned char nmeaZDA,
        unsigned char nmeaMCHN);

/*...........................................................................*/
typedef struct NMEACommandFrame NMEACommandFrame;

struct NMEACommandFrame {
    /*
     * Three bytes character string.From ?000? to ?999?.
     * An identifier used to tell the decoder how to decode the command
     */
    unsigned char commandNumber[NMEA_COMMAND_LENGTH]; /* Command number */
    /*
     * The DataField has variable length depending on the command type.
     * A comma symbol ?,? must be inserted ahead each data filed to help
     * the decoder process the DataField. *: 1 byte character.
     * The star symbol is used to mark the end of DataField.
     */
    unsigned char data[NMEA_MAX_COMMAND_LENGTH]; /* Command data */
    /*
     * Two bytes character string.
     * CHK1 and CHK2 are the checksum of the data between Preamble and ?*?.
     */
    unsigned char checksum[NMEA_CHECKUM_LENGTH]; /* Checksum */
    unsigned char length; /* Frame length */
};

void NMEACommand_createTest(NMEACommandFrame* nmeaCommandFrame);

void NMEACommand_createHotStartFrame(NMEACommandFrame* nmeaCommandFrame);

void NMEACommand_createWarmStartFrame(NMEACommandFrame* nmeaCommandFrame);

void NMEACommand_createColdStartFrame(NMEACommandFrame* nmeaCommandFrame);

void NMEACommand_createFullColdStartFrame(NMEACommandFrame* nmeaCommandFrame);

void NMEACommand_createClearEPOFrame(NMEACommandFrame* nmeaCommandFrame);

void NMEACommand_createSetBaudrateFrame(NMEACommandFrame* nmeaCommandFrame,
        enum NMEABaudrate baudrate);

void NMEACommand_createSetFixCtlFrame(NMEACommandFrame* nmeaCommandFrame,
        unsigned char* fixInterval, unsigned char length);

void NMEACommand_createSetDgpsModeFrame(NMEACommandFrame* nmeaCommandFrame,
        enum NMEADgpsMode mode) ;

void NMEACommand_createSetSbasFrame(NMEACommandFrame* nmeaCommandFrame,
        BOOL enabled) ;

void NMEACommand_createSetOutput(NMEACommandFrame* nmeaCommandFrame,
        NMEAOutputConfig* config) ;

#endif /* gps_h */
