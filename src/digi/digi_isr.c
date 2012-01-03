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

#include <p18f46j50.h>
#include "digi_isr.h"
#include "qf_port.h"
#include "digi_proxy.h"
#include "service.h"

#pragma udata digi_isr_data
static XBeePacket packet;
#pragma udata

/**
 * Interrupt function executed on XBee wake up to send mote status
 * to the network.
 * Init packet depending on mote configuration(sensors installed).
 */
void XBee_handleInterrupt(void) {
    // Init XBeePacket
    // ..........
    // ..........
    // Send packet
    Service_sendXbeePacket(&packet);
}

/**
 * Check XBee interrupt flag.
 * Reads INTCON register to determine if xbee has interrupted execution.
 *
 * @return true when INTCON flag is True
 */
BOOL XBee_checkInterrupt(void) {
    return INTCONbits.INT0F;
}

/**
 * Clear XBee interrupt flag.
 */
void XBee_clearInterruptFlag(void) {
    INTCONbits.INT0F = 0;
}
