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
#include "bsp.h"
#include "hw_adc.h"
#include "hw_serial.h"
#include "gps_proxy.h"

/*..........................................................................*/
/* All this variables have External linkage and can be used in any file */
#pragma udata mote_data
Mote mote;
Serial serial;
XBee xbee;
XBeePacket xbeePacket;
XBeeProxy xbeeProxy;
GpsProxy gpsProxy;
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
void Service_initMote() {
    rtccTimeDate d;
    /* BSP init */
    BSP_init();

    /* Create XBee */
    XBee_create(&xbee);

    /* Create Serial */
    Serial_create(&serial, XBEE_SERIAL, DEFAULT_UART_CONFIG, 25);

    /* Init serial */
    Serial_init(&serial);

    /* Create Proxy */
    XBeeProxy_create(&xbeeProxy, &serial, &xbee);

    /* Init ADC */
    Adc_init(DEFAULT_ADC_CONFIG, DEFAULT_ADC_CONFIG2, 15);

    /* Create Mote */
    Mote_create(&mote, &xbeeProxy);

    /* Enable RTCC */
    RTCCFGbits.RTCEN = 1; // enable RTCC module
}

/**
 * Send XBee packet using XBeeProxy internal instance
 *
 * @param packet packet to send
 */
void Service_sendXbeePacket(XBeePacket* packet) {
    XBeeProxy_sendPacket(&xbeeProxy, packet);
}

boolean Service_readXbeePacket(XBeePacket* packet) {
    return XBeeProxy_readPacket(&xbeeProxy, packet);
}

void Service_readGpsSignal() {

    //    NMEAOutput nmeaOutput;
    //    XBeePacket packet;
    //    // Lee la señal GPS
    //    GpsProxy_readOutput(&gpsProxy, &nmeaOutput);
    //    // Crea la trama XBee
    //    XBee_createTransmitRequestPacket(&packet, ..., nmeaOutput.data, nmeaOutput.length, ...);
    //    // Envía la trama
    //    XBeeProxy_sendPacket(&xbeeProxy, &packet);
}