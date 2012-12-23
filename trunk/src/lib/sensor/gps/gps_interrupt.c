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

#include "gps_interrupt.h"
#include "isr.h"
#include "payload.h"
#include "hw_serial.h"
#include "nmea_output.h"
#include "gps.h"

static NMEAOutputPacket packet;
static NMEAOutputRMC rmc;

void GpsInterrupt_install(void) {
    NMEAOutput_resetPacket(&packet);
    InterruptHandler_addHI((HandleInterrupt) & GpsInterrupt_handleTopHalve,
	    (HandleInterrupt) & GpsInterrupt_handleBottomHalve,
	    (CheckInterrupt) & Serial_checkInterrupt);
}

/** Read byte from uart, process packet and clear interrupt flag */
void GpsInterrupt_handleTopHalve(void) {
    Gps_readByte(&packet);
}

/** Check for valid nmea frame and save last valid location */
void GpsInterrupt_handleBottomHalve(void) {
    if(packet.rxState == NMEA_PACKET_OK) {
	NMEAOutput_readRMC(&packet, &rmc);
    }
}
