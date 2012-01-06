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
    // Service_sendXbeePacket(&packet);
    LATDbits.LATD5 = !LATDbits.LATD5;
}

/**
 * Check XBee interrupt flag.
 * Reads INTCON register to determine if xbee has interrupted execution.
 *
 * @return true when INTCON flag is True
 */
BOOL XBee_checkInterrupt(void) {
    return INTCON3bits.INT1IF;
}

/**
 * Clear XBee interrupt flag.
 */
void XBee_clearInterruptFlag(void) {
    INTCON3bits.INT1IF = 0;
}

// TODO Revisar INT0 interrupt

void XBee_installInterrupt(void) {
    TRISDbits.TRISD5 = 0;
    RPINR1 = 21;
    /* Input switch */
    TRISDbits.TRISD4 = 1;
    /* Falling edge */
    INTCON2bits.INTEDG1 = 0;
    /* Input interrupt enable */
    INTCON3bits.INT1IE = 1;
    /* Portb high interrupt priority */
    INTCON3bits.INT1IP = 1;
    /* enable all high priority interrupts */
    RCONbits.IPEN = 1;
    INTCONbits.GIEH = 1;
    /* Clear flag */
    INTCON3bits.INT1IF = 0;
}
