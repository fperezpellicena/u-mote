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

#include "usb_digi_handler.h"
#include "digi_api.h"
#include "service.h"
#include "power.h"

/* External variable from service.c */
extern XBeePacket xbeePacket;

void USBXBeeHandler_join(char usbBuffer[]) {
    unsigned int status;
    // Send ATCB1
    XBee_createATCommandPacket(&xbeePacket, 0x01, "ATCB1");
    Service_sendXbeePacket(&xbeePacket);
    // Wait for response
    while(!Service_readXbeePacket(&xbeePacket)){
        idleRcMode();
    }
    // Parse packet
    XBee_readModemStatusPacket(&xbeePacket,&status);
    if(xbeePacket.frame.modemStatus.status) {
        // Send ATSM8
        XBee_createATCommandPacket(&xbeePacket, 0x02, "ATSM8");
        Service_sendXbeePacket(&xbeePacket);
    }
}