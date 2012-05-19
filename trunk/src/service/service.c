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

#include <rtcc.h>

#include "service.h"
#include "hw_adc.h"
#include "digi_api.h"
#include "gps_proxy.h"

// TODO A eliminar

/*..........................................................................*/
/* All this variables have External linkage and can be used in any file */
#pragma udata serial1_data
Serial serial1;
#pragma

#pragma udata serial2_data
Serial serial2;
#pragma
/*..........................................................................*/

/**
 * Init mote resources(XBee, Serial, XBeeProxy).
 * Init BSP package.
 *
 * @param mote		pointer to mote struct
 * @param serial	pointer to serial struct
 * @param xbee		ponter to xbee struct
 * @param proxy		ponter to proxy struct
 */
void Service_initMote(void) {

    /* Create Proxy */
    XBeeProxy_create();

    /* Init ADC */
    //Adc_init(DEFAULT_ADC_CONFIG, DEFAULT_ADC_CONFIG2, 15);

    /* Create Mote */
   // Mote_create(&mote, &xbeeProxy);
    
    /* Enable RTCC */
    RTCCFGbits.RTCEN = 1; // enable RTCC module
}

void Service_parseXBeePacket(XBeePacket* xbeePacket) {
    // Check frameId
    if(xbeePacket->apiId == MODEM_STATUS) {
        UINT8 status;
        XBee_readModemStatusPacket(xbeePacket, &status);
        // Si se ha despertado la red, envía una trama
        if(status == MODEM_STATUS_NETWORK_WOKE_UP) {
             LATDbits.LATD5 = !LATDbits.LATD5; // Test
        }
    }
    // At the end, reset XBeePacket
    XBee_resetPacket(xbeePacket);
}



void Service_readGpsSignal(void) {

    //    NMEAOutput nmeaOutput;
    //    XBeePacket packet;
    //    // Lee la señal GPS
    //    GpsProxy_readOutput(&gpsProxy, &nmeaOutput);
    //    // Crea la trama XBee
    //    XBee_createTransmitRequestPacket(&packet, ..., nmeaOutput.data, nmeaOutput.length, ...);
    //    // Envía la trama
    //    XBeeProxy_sendPacket(&xbeeProxy, &packet);
}