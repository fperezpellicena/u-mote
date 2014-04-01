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

#include "bsp_inertial.h"
#include "payload.h"

#include "gps_isr.h"
#include "fall_interrupt.h"
#include "timer.h"

#pragma interrupt ISR_hi

void ISR_hi(void) {
#if USB_ENABLED
    if (USB_PLUGGED) {
        USBDeviceTasks();
    } else {
#endif
        if (XBEE_ON_SLEEP_FLAG) {
            // Do network processing without interrupts
            BSP_sendPacket(); 
            XBEE_ON_SLEEP_CLEAR_FLAG();
        }
#if USB_ENABLED
    }
#endif
}

#pragma interruptlow ISR_lo

void ISR_lo(void) {
    if (SERIAL_INT) {
        SERIAL_OVF_CHECK();
        GpsInterrupt_readByte();
    }
    if (TIMER0_INT) {
        FallInterrupt_readAccelerations();
        TIMER0_CLEAR_INT();
    }
}

/*..........................................................................*/
#pragma code intVector_hi = 0x08

void intVector_hi(void) {
    _asm goto ISR_hi _endasm
}

/*..........................................................................*/
#pragma code intVector_lo = 0x18

void intVector_lo(void) {
    _asm goto ISR_lo _endasm
}
