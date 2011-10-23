/*****************************************************************************
* Interrupts for PIC18
* Last Updated for Version: 4.0.01
* Date of the Last Update:  Aug 13, 2008
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2008 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qp_port.h"
#include "dpp.h"
#include "bsp.h"

#define BSP_TMR1_HZ          32768UL
#define SYSTEM_TICK_TOUT     (0x10000 - BSP_TMR1_HZ/BSP_TICKS_PER_SEC)

/*..........................................................................*/
#pragma code
#pragma interrupt ISR_hi
void ISR_hi(void) {

    if (PIR1bits.TMR1IF) {                               /* TMR1 interrupt? */
        PIR1bits.TMR1IF = 0;                   /* clear TMR1 interrupt flag */

                                          /* program TMR1 for the next tick */
        TMR1H = (uint8_t)(SYSTEM_TICK_TOUT >> 8);
        TMR1L = (uint8_t)(SYSTEM_TICK_TOUT);

        QF_tick();                    /* handle all armed time events in QF */
    }
#ifdef Q_SPY
    else if (INTCONbits.TMR0IF) {                       /* Timer0 overflow? */

        INTCONbits.TMR0IF = 0;             /* clear the TMR0 interrupt flag */
        BSP_tickTime += (QSTimeCtr)0x10000;    /* account for TMR0 overflow */

    }
#endif
    /*
    else if (...) {                          // handle other interrupt sources
        . . .
    }
    */
}
/*..........................................................................*/
#pragma code intVector_hi = 0x08
void intVector_hi(void) {
    _asm
        goto ISR_hi                                       /* jump to ISR_hi */
    _endasm
}

