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

#include "qp_port.h"
#include "bsp.h"
#include "digi.h"
#include "hw_serial.h"
#include "digi_proxy.h"
#include "digi_api.h"


XBee xbee;
Serial serial;
#pragma udata packet
XBeePacket packet;
#pragma udata
XBeeProxy proxy;
uint8_t atCommand[3] = "ND";

void main() {
    BSP_init();
   // Timer0_init();
   
    //Interrupts_enable();

    XBee_create(&xbee);
    Serial_create(&serial, XBEE_SERIAL, DEFAULT_UART_CONFIG, 25);
    Serial_init(&serial);

	Serial_send(&serial, 'V');
	Serial_send(&serial, 'P');
	Serial_send(&serial, 'N');

	XBee_createATCommandPacket(&packet, 'B', atCommand);
	XBeeProxy_create(&proxy, &serial, &xbee);
	//XBeeProxy_sendAtCommand(&proxy, &packet, 2);

    while (1);
}
