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

#include "isr.h"
#include "qp_port.h"
#include "bsp.h"
#include "hw_profile.h"
#include "usb_config.h"
#include "usb_device.h"
#include "critical.h"

/*..........................................................................*/
/* Interrupt vectors */
InterruptHandlerVector interruptVectorHI;
InterruptHandlerVector interruptVectorLO;
/* Current active interrupt to dispath */
volatile InterruptHandler* activeInterrupt;


/*..........................................................................*/

/* HIGH LEVEL INTERRUPTION METHOD */
#pragma interrupt ISR_hi

void ISR_hi(void) {
    unsigned char index;
    for (index = 0; index < interruptVectorHI.size; index++) {
        // Check for active interrupt
        if (interruptVectorHI.handlers[index].isActive()) {
            // Clear flag
            interruptVectorHI.handlers[index].clearFlag();
            // Set active interrupt
            activeInterrupt = &interruptVectorHI.handlers[index];
            break;
        }
    }
    // Execute USB tasks only if usb is plugged
#if defined(USB_INTERRUPT)
    if (USB_PLUGGED) {
        USBDeviceTasks();
    }
#endif
}

/*..........................................................................*/
/* LOW LEVEL INTERRUPTION METHOD */
#pragma interruptlow ISR_lo

void ISR_lo(void) {
    unsigned char index;
    for (index = 0; index < interruptVectorLO.size; index++) {
        // Check for active interrupt
        if (interruptVectorLO.handlers[index].isActive()) {
            // Clear flag
            interruptVectorLO.handlers[index].clearFlag();
            // Set active interrupt
            activeInterrupt = &interruptVectorLO.handlers[index];
            break;
        }
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

/*..........................................................................*/
void InterruptHandler_create(InterruptHandler* handler,
        HandleInterrupt handleFunction, CheckInterrupt checkFunction,
        ClearInterruptFlag clearFunction) {
    handler->handle = handleFunction;
    handler->isActive = checkFunction;
    handler->clearFlag = clearFunction;
}

/*..........................................................................*/
/**
 * Init interrupt vectors size to zero.
 * Doing this, avoids error on first power up checking for active interrupts.
 */
void InterruptHandler_initVectors() {
    interruptVectorHI.size = 0;
    interruptVectorLO.size = 0;
}

/*..........................................................................*/
void InterruptHandler_addHI(HandleInterrupt handleFunction,
        CheckInterrupt checkFunction, ClearInterruptFlag clearFunction) {
    if (interruptVectorHI.size < MAX_INTERRUPT_HANDLERS) {
        interruptVectorHI.handlers[interruptVectorHI.size].handle = handleFunction;
        interruptVectorHI.handlers[interruptVectorHI.size].isActive = checkFunction;
        interruptVectorHI.handlers[interruptVectorHI.size].clearFlag = clearFunction;
        interruptVectorHI.size++;
    }
}

/*..........................................................................*/
void InterruptHandler_addLO(HandleInterrupt handleFunction,
        CheckInterrupt checkFunction, ClearInterruptFlag clearFunction) {
    if (interruptVectorLO.size < MAX_INTERRUPT_HANDLERS) {
        interruptVectorLO.handlers[interruptVectorLO.size].handle = handleFunction;
        interruptVectorLO.handlers[interruptVectorLO.size].isActive = checkFunction;
        interruptVectorLO.handlers[interruptVectorLO.size].clearFlag = clearFunction;
        interruptVectorLO.size++;
    }
}

/*..........................................................................*/

/* Handle active interrupt in critical section */
void InterruptHandler_handleActiveInterrupt() {
    enterCritical();
    activeInterrupt->handle();
    exitCritical();
}

