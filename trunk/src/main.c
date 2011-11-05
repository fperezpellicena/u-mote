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

void main() {
    BSP_init();
    Timer0_init();
    Usart1_init();
    Interrupts_enable();

    XBee_create(&xbee);
    Serial_create(&serial, XBEE_SERIAL, XBEE_BAUD_RATE);
    Serial_init(&serial);
   

     XBee_CreateATCommandPacket(&packet, 0x01, "ND", "", 0x02);

    while (1);
}
