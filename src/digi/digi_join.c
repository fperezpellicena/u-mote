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

#include "digi_api.h"
#include "util.h"

#define USB_JOINED_MSG              "Mote joined\0"
#define USB_JOINED_ERROR_MSG        "Mote not joined\0"

static XBeePacket packet;

static void XBee_commissioning(UINT8 presses);

static void XBee_commissioning(UINT8 presses) {
    static UINT8 command[3] = {'C', 'B', '\0'};
    UINT8 params[2] = {presses, '\0'};
    XBee_resetPacket(&packet);
    XBee_createCompleteATCommandPacket(&packet, 0x01, command, params, 0x01);
    XBee_sendPacket(&packet);
}

static void XBee_setSleepMode(UINT8 mode);

static void XBee_setSleepMode(UINT8 mode) {
    static UINT8 command[3] = {'S', 'M', '\0'};
    UINT8 params[2] = {mode, '\0'};
    XBee_resetPacket(&packet);
    XBee_createCompleteATCommandPacket(&packet, 0x01, command, params, 0x01);
    XBee_sendPacket(&packet);
}

/*..........................................................................*/
/* Join mote */
BOOL XBee_join(void) {
    // Simulate one press commissioning button
    XBee_commissioning(COMMISSIONING_ONE_PRESS);
    // Set sleep mode 8
    XBee_setSleepMode(DEEP_SLEEP_MODE);
    return TRUE;
}

/*..........................................................................*/
void XBee_usbJoin(char* usbBuffer) {
    if (XBee_join())
        Util_str2ram((UINT8 rom*)USB_JOINED_MSG, (UINT8*) usbBuffer);
    Util_str2ram((UINT8 rom*)USB_JOINED_MSG, (UINT8*) usbBuffer);
}