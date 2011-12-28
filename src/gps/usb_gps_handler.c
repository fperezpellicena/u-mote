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
#include "usb_gps_handler.h"
#include "gps.h"
#include "gps_proxy.h"

NMEACommandPacket command;
NMEAOutputConfig config;
NMEAOutput nmeaOutput;

/**
 * Test de creación de tramas
 * 
 * @param usbBuffer
 */
void USBGpsHandler_parseGPSData(char* usbBuffer) {
    // Create test
    NMEACommand_createTest(&command);
    // Create hot start
    NMEACommand_createHotStartFrame(&command);
    // Create warm start
    NMEACommand_createWarmStartFrame(&command);
    // Create cold start
    NMEACommand_createColdStartFrame(&command);
    // Create full cold start
    NMEACommand_createFullColdStartFrame(&command);
    // Create clear EPO
    NMEACommand_createClearEPOFrame(&command);
    // Create set baudrate
    NMEACommand_createSetBaudrateFrame(&command, NMEA_BAUD_9600);
    // Create fix interval
    NMEACommand_createSetFixCtlFrame(&command, NMEA_5_HZ);
    // Create DGPS mode
    NMEACommand_createSetDgpsModeFrame(&command, NMEA_WAAS);
    // Create set SBAS
    NMEACommand_createSetSbasFrame(&command, FALSE);
    // Create set output
    NMEAOutputConfig_create(&config, NMEA_OUTPUT_EVERY_ONE_POS_FIX,
            NMEA_OUTPUT_DISABLED, NMEA_OUTPUT_EVERY_ONE_POS_FIX,
            NMEA_OUTPUT_DISABLED, NMEA_OUTPUT_EVERY_ONE_POS_FIX,
            NMEA_OUTPUT_EVERY_ONE_POS_FIX, NMEA_OUTPUT_DISABLED,
            NMEA_OUTPUT_DISABLED, NMEA_OUTPUT_EVERY_ONE_POS_FIX,
            NMEA_OUTPUT_EVERY_ONE_POS_FIX, NMEA_OUTPUT_EVERY_ONE_POS_FIX,
            NMEA_OUTPUT_DISABLED, NMEA_OUTPUT_DISABLED, NMEA_OUTPUT_DISABLED);
    NMEACommand_createSetOutput(&command, &config);
    // Create set datum
    NMEACommand_createSetDatum(&command, NMEA_DATUM_WGS1984);
}

/**
 *
 * @param usbBuffer
 * @return
 */
unsigned char USBGpsHandler_testGPS(char* usbBuffer) {
//    // Lee el la trama GPS
//    GpsProxy_readOutput(&nmeaOutput);
//    // Copia la trama recibida al buffer USB
//    Util_strCopy(nmeaOutput.data, nmeaOutput.length);
    return nmeaOutput.length;
}