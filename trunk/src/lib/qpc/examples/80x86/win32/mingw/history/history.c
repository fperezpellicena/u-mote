/*****************************************************************************
* Product: History state pattern example
* Last Updated for Version: 4.0.00
* Date of the Last Update:  Apr 06, 2008
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
#include "qep_port.h"
#include "qassert.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Q_DEFINE_THIS_FILE

/*..........................................................................*/
enum ToasterOvenSignals {
    OPEN_SIG = Q_USER_SIG,
    CLOSE_SIG,
    TOAST_SIG,
    BAKE_SIG,
    OFF_SIG,
    TERMINATE_SIG                              /* terminate the application */
};
/*..........................................................................*/
typedef struct ToasterOvenTag ToasterOven;
struct ToasterOvenTag {
    QHsm super;                                         /* derive from QHsm */
    QStateHandler doorClosed_history;    /* history of the doorClosed state */
};

void ToasterOven_ctor(ToasterOven *me);                     /* default ctor */

QState ToasterOven_initial   (ToasterOven *me, QEvent const *e);
QState ToasterOven_doorOpen  (ToasterOven *me, QEvent const *e);
QState ToasterOven_off       (ToasterOven *me, QEvent const *e);
QState ToasterOven_heating   (ToasterOven *me, QEvent const *e);
QState ToasterOven_toasting  (ToasterOven *me, QEvent const *e);
QState ToasterOven_baking    (ToasterOven *me, QEvent const *e);
QState ToasterOven_doorClosed(ToasterOven *me, QEvent const *e);
QState ToasterOven_final     (ToasterOven *me, QEvent const *e);

/*..........................................................................*/
void ToasterOven_ctor(ToasterOven *me) {                    /* default ctor */
    QHsm_ctor(&me->super, (QStateHandler)&ToasterOven_initial);
}

/* HSM definitio -----------------------------------------------------------*/
QState ToasterOven_initial(ToasterOven *me, QEvent const *e) {
    (void)e;               /* avoid compiler warning about unused parameter */
    me->doorClosed_history = (QStateHandler)&ToasterOven_off;
    return Q_TRAN(&ToasterOven_doorClosed);
}
/*..........................................................................*/
QState ToasterOven_final(ToasterOven *me, QEvent const *e) {
    (void)me;              /* avoid compiler warning about unused parameter */
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("-> final\nBye!Bye!\n");
            _exit(0);
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState ToasterOven_doorClosed(ToasterOven *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("door-Closed;");
            return Q_HANDLED();
        }
        case Q_INIT_SIG: {
            return Q_TRAN(&ToasterOven_off);
        }
        case OPEN_SIG: {
            return Q_TRAN(&ToasterOven_doorOpen);
        }
        case TOAST_SIG: {
            return Q_TRAN(&ToasterOven_toasting);
        }
        case BAKE_SIG: {
            return Q_TRAN(&ToasterOven_baking);
        }
        case OFF_SIG: {
            return Q_TRAN(&ToasterOven_off);
        }
        case TERMINATE_SIG: {
            return Q_TRAN(&ToasterOven_final);
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState ToasterOven_off(ToasterOven *me, QEvent const *e) {
    (void)me;              /* avoid compiler warning about unused parameter */
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("toaster-Off;");
            me->doorClosed_history = (QStateHandler)&ToasterOven_off;
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&ToasterOven_doorClosed);
}
/*..........................................................................*/
QState ToasterOven_heating(ToasterOven *me, QEvent const *e) {
    (void)me;              /* avoid compiler warning about unused parameter */
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("heater-On;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            printf("heater-Off;");
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&ToasterOven_doorClosed);
}
/*..........................................................................*/
QState ToasterOven_toasting(ToasterOven *me, QEvent const *e) {
    (void)me;              /* avoid compiler warning about unused parameter */
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("toasting;");
            me->doorClosed_history = (QStateHandler)&ToasterOven_toasting;
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&ToasterOven_heating);
}
/*..........................................................................*/
QState ToasterOven_baking(ToasterOven *me, QEvent const *e) {
    (void)me;              /* avoid compiler warning about unused parameter */
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("baking;");
            me->doorClosed_history = (QStateHandler)&ToasterOven_baking;
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&ToasterOven_heating);
}
/*..........................................................................*/
QState ToasterOven_doorOpen(ToasterOven *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("door-Open,lamp-On;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            printf("lamp-Off;");
            return Q_HANDLED();
        }
        case CLOSE_SIG: {
            return Q_TRAN(me->doorClosed_history);/*<-tranisition to HISTORY*/
        }
    }
    return Q_SUPER(&QHsm_top);
}
/* test harness ============================================================*/

#include <conio.h>

static ToasterOven l_test;

int main() {
    printf("History state pattern\nQEP version: %s\n"
           "Press 'o' to OPEN  the door\n"
           "Press 'c' to CLOSE the door\n"
           "Press 't' to start TOASTING\n"
           "Press 'b' to start BAKING\n"
           "Press 'f' to turn the oven OFF\n"
           "Press ESC to quit...\n",
           QEP_getVersion());

      /* instantiate the ToasterOven HSM and trigger the initial transition */
    ToasterOven_ctor(&l_test);
    QHsm_init((QHsm *)&l_test, (QEvent *)0);

    for (;;) {
        QEvent e;
        uint8_t c;

        printf("\n");
        c = (uint8_t)_getch();       /* read one character from the console */
        printf("%c: ", c);

        switch (c) {
            case 'o':  e.sig = OPEN_SIG;        break;
            case 'c':  e.sig = CLOSE_SIG;       break;
            case 't':  e.sig = TOAST_SIG;       break;
            case 'b':  e.sig = BAKE_SIG;        break;
            case 'f':  e.sig = OFF_SIG;         break;
            case 0x1B: e.sig = TERMINATE_SIG;   break;
        }
                               /* dispatch the event into the state machine */
        QHsm_dispatch((QHsm *)&l_test,  &e);
    }
    return 0;
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    _exit(-1);
}
