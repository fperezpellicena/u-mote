/*****************************************************************************
* Product: Calculator Example with inheritance of whole state model
* Last Updated for Version: 4.0.01
* Date of the Last Update:  Sep 23, 2008
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
#ifndef calc2_h
#define calc2_h

#include "calc1.h"

enum Calc2Signals {
    PERCENT_SIG = MAX_CALC1_SIG,
    MAX_CALC2_SIG
};

typedef struct Calc2Tag {                            /* Calc2 state machine */
    Calc1 super;                                       /* derive from Calc1 */
} Calc2;

void Calc2_ctor(Calc2 *me);                                         /* ctor */

QState Calc2_operand2(Calc2 *me, QEvent const *e);
extern QStateHandler Calc2_state_operand2;

#endif                                                           /* calc2_h */