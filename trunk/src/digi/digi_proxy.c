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

#include "bsp.h"
#include "digi_serial.h"
#include "digi_proxy.h"
#include "digi_interrupt.h"
#include "sensor_proxy.h"
#include "util.h"
#include "rtc.h"


#pragma udata xbee_data
static XBeePacket xbeeProxyPacket;
#pragma udata


static void XBeeProxy_commissioning(UINT8 presses);
static void XBeeProxy_setSleepMode(UINT8 mode);

/*..........................................................................*/

/* XBee proxy constructor */
void XBeeProxy_init(void) {
    XBeeSerial_init(EUSART_9600);
    XBeeInterrupt_install();
}

/*..........................................................................*/
/* Join mote */
BOOL XBeeProxy_join(void) {
    // Simulate one press commissioning button
    XBeeProxy_commissioning(1);
    // Set sleep mode 8
    XBeeProxy_setSleepMode(8);
    return TRUE;
}

/*..........................................................................*/
void XBeeProxy_usbJoin(char* usbBuffer) {
    if (XBeeProxy_join())
        Util_str2ram((UINT8 rom*)USB_JOINED_MSG, (UINT8*) usbBuffer);
    Util_str2ram((UINT8 rom*)USB_JOINED_MSG, (UINT8*) usbBuffer);
}

static void XBeeProxy_commissioning(UINT8 presses) {
    static UINT8 command[3] = {'C', 'B', '\0'};
    UINT8 params[2] = {presses, '\0'};
    XBee_resetPacket(&xbeeProxyPacket);
    XBee_createCompleteATCommandPacket(&xbeeProxyPacket, 0x01, command, params, 0x01);
    XBeeProxy_sendPacket(&xbeeProxyPacket);
}

static void XBeeProxy_setSleepMode(UINT8 mode) {
    static UINT8 command[3] = {'S', 'M', '\0'};
    UINT8 params[2] = {mode, '\0'};
    XBee_resetPacket(&xbeeProxyPacket);
    XBee_createCompleteATCommandPacket(&xbeeProxyPacket, 0x01, command, params, 0x01);
    XBeeProxy_sendPacket(&xbeeProxyPacket);
}



