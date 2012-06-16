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
#include "bsp.h"
#include "usb_config.h"
#include "usb_device.h"

/*..........................................................................*/
/* Interrupt vectors */
InterruptHandlerVector interruptVectorHI;
InterruptHandlerVector interruptVectorLO;
/* Current active interrupt to dispatch */
InterruptHandler* activeInterrupt;


/*..........................................................................*/

/* HIGH LEVEL INTERRUPTION METHOD */
#pragma interrupt ISR_hi

/** Internally, clears GIE bit, so it is not reentrant */
void ISR_hi(void) {
    unsigned char index;
    // Execute USB tasks only if usb is plugged
#ifdef USB_INTERRUPT
    if (USB_PLUGGED) {
        USBDeviceTasks();
    } else {
        for (index = 0; index < interruptVectorHI.size; index++) {
            // Check for active interrupt
            if (interruptVectorHI.handlers[index].isActive()) {
                // Set active interrupt
                activeInterrupt = &interruptVectorHI.handlers[index];
                // Top halve function
                activeInterrupt->handleTopHalveInterrupt();
                break;
            }
        }
    }
#else
    for (index = 0; index < interruptVectorHI.size; index++) {
        // Check for active interrupt
        if (interruptVectorHI.handlers[index].isActive()) {
            // Set active interrupt
            activeInterrupt = &interruptVectorHI.handlers[index];
            break;
        }
    }
#endif
}

/*..........................................................................*/
/* LOW LEVEL INTERRUPTION METHOD */
#pragma interruptlow ISR_lo

/** Internally, clears GIE bit, so it is not reentrant */
void ISR_lo(void) {
    unsigned char index;
#ifdef USB_INTERRUPT
    if (!USB_PLUGGED) {
        for (index = 0; index < interruptVectorLO.size; index++) {
            // Check for active interrupt
            if (interruptVectorLO.handlers[index].isActive()) {
                // Set active interrupt
                activeInterrupt = &interruptVectorLO.handlers[index];
                // Top halve function
                activeInterrupt->handleTopHalveInterrupt();
                break;
            }
        }
    }
#else
    for (index = 0; index < interruptVectorLO.size; index++) {
        // Check for active interrupt
        if (interruptVectorLO.handlers[index].isActive()) {
            // Set active interrupt
            activeInterrupt = &interruptVectorLO.handlers[index];
            break;
        }
    }
#endif
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
        HandleInterrupt topHandleFunction, HandleInterrupt bottomHandleFunction,
        CheckInterrupt checkFunction) {
    handler->handleTopHalveInterrupt = topHandleFunction;
    handler->handleBottomHalveInterrupt = bottomHandleFunction;
    handler->isActive = checkFunction;
}

/*..........................................................................*/

/**
 * Init interrupt vectors size to zero.
 * Doing this, avoids error on first power up checking for active interrupts.
 */
void InterruptHandler_initVectors(void) {
    interruptVectorHI.size = 0;
    interruptVectorLO.size = 0;
}

/*..........................................................................*/
void InterruptHandler_addHI(HandleInterrupt topHandleFunction,
        HandleInterrupt bottomHandleFunction, CheckInterrupt checkFunction) {
    if (interruptVectorHI.size < MAX_INTERRUPT_HANDLERS) {
        interruptVectorHI.handlers[interruptVectorHI.size].handleTopHalveInterrupt = topHandleFunction;
        interruptVectorHI.handlers[interruptVectorHI.size].handleBottomHalveInterrupt = bottomHandleFunction;
        interruptVectorHI.handlers[interruptVectorHI.size].isActive = checkFunction;
        interruptVectorHI.size++;
    }
}

/*..........................................................................*/
void InterruptHandler_addLO(HandleInterrupt topHandleFunction,
        HandleInterrupt bottomHandleFunction, CheckInterrupt checkFunction) {
    if (interruptVectorLO.size < MAX_INTERRUPT_HANDLERS) {
        interruptVectorLO.handlers[interruptVectorLO.size].handleTopHalveInterrupt = topHandleFunction;
        interruptVectorLO.handlers[interruptVectorLO.size].handleBottomHalveInterrupt = bottomHandleFunction;
        interruptVectorLO.handlers[interruptVectorLO.size].isActive = checkFunction;
        interruptVectorLO.size++;
    }
}

/*..........................................................................*/

/**
 * Handle active interrupt in botton halve stategy.
 *
 * Interrupt Bottom halve, does not run in critical section, so it can be 
 * interrupted.
 * It also does not clear flag, because it must be done in interrupt context.
 */
void InterruptHandler_handleActiveInterrupt(void) {
    // Check for a valid active interrupt
    if(activeInterrupt) {
        activeInterrupt->handleBottomHalveInterrupt();
    }
}
