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

#ifndef digi_proxy_h
#define digi_proxy_h

#include "digi.h"
#include "digi_api.h"
#include "hw_serial.h"

typedef struct XBeeProxy XBeeProxy;

struct XBeeProxy {
    Serial* serial; /* Hardware physical interface */
    XBee* xbee; /* Xbee firmware configuration */
};

void XBeeProxy_create(XBeeProxy* const proxy, Serial* const serial, XBee* const xbee);

boolean XBeeProxy_readPacket(XBeeProxy* const proxy, XBeePacket* const packet);

boolean XBeeProxy_sendAtCommand(XBeeProxy* const proxy, XBeePacket* const packet,
        uint8_t length);

boolean XBeeProxy_sendTransmitRequest(XBeeProxy* const proxy, XBeePacket* const packet,
        uint8_t length);

boolean XBeeProxy_sendExplicitAddressing(XBeeProxy* const proxy, XBeePacket* const packet,
        uint8_t length);

boolean XBeeProxy_sendRemoteAtCommand(XBeeProxy* const proxy, XBeePacket* const packet,
        uint8_t length);


#endif     /* digi_proxy_h*/
