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

#ifndef isr_h
#define isr_h

#include "GenericTypeDefs.h"
#include "bsp.h"

/*..........................................................................*/
/* Handler function prototype */
typedef void rom (*HandleInterrupt)(void);

/* Check interrupt function prototype */
typedef BOOL rom (*CheckInterrupt)(void);

/*..........................................................................*/
/* Interrupt handler class */
typedef struct InterruptHandler InterruptHandler;

struct InterruptHandler {
    /* Top halve Function handler */
    HandleInterrupt handleTopHalveInterrupt;
    /* Bottom halve Function handler */
    HandleInterrupt handleBottomHalveInterrupt;
    /* Indicates if interrupt is enabled */
    BOOL enabled;
    /* Check if interrupt has happened */
    CheckInterrupt isActive;
};

/*..........................................................................*/
/* Interrupt handle vector */
typedef struct InterruptHandlerVector InterruptHandlerVector;

struct InterruptHandlerVector {
    InterruptHandler handlers[MAX_INTERRUPT_HANDLERS];
    UINT8 size;
};

/*..........................................................................*/
void InterruptHandler_create(InterruptHandler* handler,
        HandleInterrupt topHandleFunction, HandleInterrupt bottomHandleFunction,
        CheckInterrupt checkFunction);

/*..........................................................................*/
void InterruptHandler_initVectors(void);

/*..........................................................................*/
void InterruptHandler_addHI(HandleInterrupt topHandleFunction,
        HandleInterrupt bottomHandleFunction,CheckInterrupt checkFunction);

/*..........................................................................*/
void InterruptHandler_addLO(HandleInterrupt topHandleFunction,
        HandleInterrupt bottomHandleFunction,CheckInterrupt checkFunction);

/*..........................................................................*/
void InterruptHandler_handleActiveInterrupt(void);


#endif  /* isr_h*/
