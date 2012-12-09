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

#ifndef gps_api_h
#define gps_api_h

#include "GenericTypeDefs.h"

/*...........................................................................*/
#define NMEA_MAX_COMMAND_LENGTH 50                  /* NMEA Max length frame */
#define NMEA_PREAMBLE           '$'                 /* NMEA command preamble */
#define NMEA_CHK_CHAR           '*'                    /* NMEA chk delimiter */
#define NMEA_COMMA_CHAR         ','                  /* NMEA comma delimiter */

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

void NMEACommand_createTest(NMEACommandPacket* nmeaCommandFrame);

void NMEACommand_createHotStartFrame(NMEACommandPacket* nmeaCommandFrame);

void NMEACommand_createWarmStartFrame(NMEACommandPacket* nmeaCommandFrame);

void NMEACommand_createColdStartFrame(NMEACommandPacket* nmeaCommandFrame);

void NMEACommand_createFullColdStartFrame(NMEACommandPacket* nmeaCommandFrame);

void NMEACommand_createClearEPOFrame(NMEACommandPacket* nmeaCommandFrame);

void NMEACommand_createSetBaudrateFrame(NMEACommandPacket* nmeaCommandFrame,
        UINT8* baudrate);

void NMEACommand_createSetFixCtlFrame(NMEACommandPacket* nmeaCommandFrame,
        UINT8* fixInterval);

void NMEACommand_createSetDgpsModeFrame(NMEACommandPacket* nmeaCommandFrame,
        UINT8* mode);

void NMEACommand_createSetSbasFrame(NMEACommandPacket* nmeaCommandFrame,
        BOOL enabled);

void NMEACommand_createSetOutput(NMEACommandPacket* nmeaCommandFrame,
        NMEAOutputConfig* config);

void NMEACommand_createSetDatum(NMEACommandPacket* nmeaCommandFrame,
        UINT8 rom* datum);


/*...........................................................................*/
/**
 * Standard NMEA0183
 * In default configuration outputted messages are GGA, RMC, GSV and GSA messages.
 * Receiver can be configured to have user defined set of output messages
 * by command PMTK314
 * ............................................................................
 * GGA - Global Positioning System Fix Data
 * Time, position and fix related data for a GPS receiver.
 * Example:
 *  $GPGGA,114353.000,6016.3245,N,02458.3270,E,1,10,0.81,35.2,M,19.5,M,,*50
 * Format:
 *  $GPGGA,hhmmss.dd,xxmm.dddd,<N|S>,yyymm.dddd,<E|W>,v,ss,d.d,h.h,M,g.g,M,a.a,
 *  xxxx*hh<CR><LF>
 * ............................................................................
 * RMC - Recommended Minimum Specific GNSS Data.
 * Time, date, position, course and speed data.
 * Example:
 *  $GPRMC,114353.000,A,6016.3245,$GPGGA,114353.000,6016.3245,N,02458.3270,E,1,10,0.81,35.2,M,19.5,M,,*50N,02458.3270,E,0.01,0.00,121009,,,A*69
 * Format:
 *  $GPRMC,hhmmss.dd,S,xxmm.dddd,<N|S>,yyymm.dddd,<E|W>,s.s,h.h,ddmmyy,d.d,
 *  <E|W>,M*hh<CR><LF>
 * ............................................................................
 * GSV - Satellites in view
 * Number of satellites in view, satellite ID (PRN) numbers, elevation, azimuth,
 * and SNR value. The information for four satellites is a maximum per one message,
 * additional messages up to maximum of eight are sent if needed.
 * The satellites are in PRN number order.
 * Example:
 *  $GPGSV,3,1,11,29,68,228,47,30,59,151,47,31,44,284,45,02,38,062,44*7C
 *  $GPGSV,3,2,11,12,28,130,41,10,14,102,35,05,12,110,35,04,11,040,34*70
 *  $GPGSV,3,3,11,21,05,196,29,16,05,297,28,13,02,021,30*4E
 * Format:
 *  $GPGSV,n,m,ss,xx,ee,aaa,cn,????. ,xx,ee,aaa,cn*hh<CR><LF>
 * ............................................................................
 * GSA - DOP and Active Satellites
 * GPS receiver operating mode, satellites used in the navigation solution
 * reported by the GGA sentence, and DOP values.
 * Example:
 *  $GPGSA,A,3,02,21,30,04,16,05,10,12,31,29,,,1.33,0.81,1.06*02
 * Format:
 *  $GPGSA,a,b,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,p.p,h.h,v.v*hh<CR><LF>
 * ............................................................................
 * Objective:
 *  Bridging GPS messages to XBee network without processing.
 */

// FIXME
//  Each sentence begins with a '$' and ends with a carriage return/line feed
//  sequence and can be no longer than 80 characters of visible text
//  (plus the line terminators).
#define NMEA_OUTPUT_MAX_LENGTH      100           /* Max NMEA message length */

typedef struct NMEAOutput NMEAOutput;

struct NMEAOutput {
    UINT8 data[NMEA_OUTPUT_MAX_LENGTH]; /* Content */
    UINT8 length; /* Content length */
    UINT8 rxState; /* Read status */
    unsigned int chkRead; /* Checksum */
    unsigned int chkCalculated; /* Checksum */
};

enum NMEAReadStatus {
    NMEA_PACKET_PREAMBLE, NMEA_PACKET_DATA, NMEA_PACKET_CRC_1,
    NMEA_PACKET_CRC_2
};

#endif /* gps_api_h */