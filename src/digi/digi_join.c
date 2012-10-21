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

static XBeePacket packet;

static void XBee_commissioning(UINT8 timesButtonPress);

static void XBee_commissioning(UINT8 timesButtonPress) {
    static UINT8 command[3] = {'C', 'B', '\0'};
    UINT8 params[2] = {timesButtonPress, '\0'};
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

/* Join xbee */
void XBee_join(void) {
    // Simulate one press commissioning button
    XBee_commissioning(COMMISSIONING_ONE_PRESS);
    // Set sleep mode 8
    XBee_setSleepMode(DEEP_SLEEP_MODE);
}

/*..........................................................................*/

/* Reset xbee */
void XBee_reset(void) {
    static UINT8 command[3] = {'F', 'R', '\0'};
    XBee_resetPacket(&packet);
    XBee_createATCommandPacket(&packet, 0x01, command);
    XBee_sendPacket(&packet);
}
