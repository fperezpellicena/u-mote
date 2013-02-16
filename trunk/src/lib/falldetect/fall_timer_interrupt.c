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

#include "fall_interrupt.h"

#include "timer.h"
#include "adxl.h"

#include "isr.h"
#include "bsp_inertial.h"

static Acceleration acceleration;

/** Install interrupt */
void FallTimerInterrupt_install(void) {
    InterruptHandler_addHI((HandleInterrupt) & FallTimerInterrupt_handleTopHalve,
            (HandleInterrupt) & FallTimerInterrupt_handleBottomHalve,
            (CheckInterrupt) & FallTimerInterrupt_check);
}

/** Top halve interrupt handler */
void FallTimerInterrupt_handleTopHalve(void) {
    return;
}

/** Bottom halve interrupt handler */
void FallTimerInterrupt_handleBottomHalve(void) {
    // Get samples
    acceleration.x = Adxl_convertAccelerationX();
    acceleration.y = Adxl_convertAccelerationY();
    // Add to median filter
    //Filter_add(acceleration);
    // Check if median filter is full to process
//    if(MedianFilter_isFull()) {
//        // Disable timer
//        Timer0_stop();
//        MedianFilter_process();
//    }
//    // Check if fir filter is full to process
//    if(FirFilter_isFull()) {
//        FirFilter_process();
//    }
    // Enable timer
    Timer0_start();
}

/** Clear interrupt flag */
BOOL FallTimerInterrupt_check(void) {
    return Timer0_checkInterrupt();
}