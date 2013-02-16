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
#include "hw_serial.h"
#include "gps_interrupt.h"
#include "nmea_output.h"
#include "nmea_command.h"

/** Enable RMC output, all other disabled */
static NMEAOutputConfig defaultNMEAOutput
        = {'0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};

static void Gps_setDefaultMode(void);

/*...........................................................................*/
void Gps_init(void) {
    Serial_init(EUSART_9600); // RS-232
    Gps_setDefaultMode();
    GpsInterrupt_install(); // Interrupt pin and configuration
}

static void Gps_setDefaultMode(void) {
    NMEACommandPacket packet;
    NMEACommand_createSetOutput(&packet, &defaultNMEAOutput);
    Gps_sendPacket(&packet);
}