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

#ifndef nmea_output_h
#define nmea_output_h

#include "GenericTypeDefs.h"

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
 *  $GPRMC,114353.000,A,6016.3245,$GPGGA,114353.000,6016.3245,N,02458.3270,E,1,
 *          10,0.81,35.2,M,19.5,M,,*50N,02458.3270,E,0.01,0.00,121009,,,A*69
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

/*...........................................................................*/
typedef struct NMEAOutputPacket NMEAOutputPacket;

struct NMEAOutputPacket {
    UINT8 data[NMEA_OUTPUT_MAX_LENGTH]; /* Content */
    UINT8 lastByte; /* Last received byte index */
    UINT8 length; /* Content length */
    UINT8 rxState; /* Read status */
    UINT8 chkRead[3]; /* Checksum */
    UINT8 chkCalculated; /* Checksum */
};

/*...........................................................................*/
enum NMEAReadStatus {
    NMEA_PACKET_PREAMBLE = 0, NMEA_PACKET_DATA, NMEA_PACKET_CRC_1,
    NMEA_PACKET_CRC_2, NMEA_PACKET_CR, NMEA_PACKET_LF, NMEA_PACKET_OK,
    NMEA_PACKET_ERROR
};

void NMEAOutput_resetPacket(NMEAOutputPacket * packet);

#endif	/* gps_output_api_h */
