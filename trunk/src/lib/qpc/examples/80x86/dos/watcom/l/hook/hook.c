/*****************************************************************************
* Product: Ultimate Hook state pattern example
* Last Updated for Version: 4.0.00
* Date of the Last Update:  Apr 07, 2008
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

typedef struct UltimateHookTag {
    QHsm super;                                         /* derive from QHsm */
} UltimateHook;

void UltimateHook_ctor(UltimateHook *me);

static QState UltimateHook_initial (UltimateHook *me, QEvent const *e);
static QState UltimateHook_generic (UltimateHook *me, QEvent const *e);
static QState UltimateHook_specific(UltimateHook *me, QEvent const *e);
static QState UltimateHook_final   (UltimateHook *me, QEvent const *e);

enum UltimateHookSignals {
    A_SIG = Q_USER_SIG,
    B_SIG,
    C_SIG,
    D_SIG
};

/*..........................................................................*/
void UltimateHook_ctor(UltimateHook *me) {
    QHsm_ctor(&me->super, (QStateHandler)&UltimateHook_initial);
}
/*..........................................................................*/
QState UltimateHook_initial(UltimateHook *me, QEvent const *e) {
    (void)e;                  /* avoid the "unreferenced parameter" warning */
    printf("top-INIT;");
    return Q_TRAN(&UltimateHook_generic);
}
/*..........................................................................*/
QState UltimateHook_final(UltimateHook *me, QEvent const *e) {
    (void)me;                 /* avoid the "unreferenced parameter" warning */
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("final-ENTRY;");
            printf("\nBye!Bye!\n");
            exit(0);
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState UltimateHook_generic(UltimateHook *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("generic-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            printf("generic-EXIT;");
            return Q_HANDLED();
        }
        case Q_INIT_SIG: {
            printf("generic-INIT;");
            return Q_TRAN(&UltimateHook_specific);
        }
        case A_SIG: {
            printf("generic-A;");
            return Q_HANDLED();
        }
        case B_SIG: {
            printf("generic-B;");
            return Q_HANDLED();
        }
        case C_SIG: {
            printf("generic:C(reset);");
            return Q_TRAN(&UltimateHook_generic);
        }
        case D_SIG: {
            return Q_TRAN(&UltimateHook_final);
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState UltimateHook_specific(UltimateHook *me, QEvent const *e) {
    (void)me;                 /* avoid the "unreferenced parameter" warning */
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("specific-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            printf("specific-EXIT;");
            return Q_HANDLED();
        }
        case A_SIG: {
            printf("specific-A;");
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&UltimateHook_generic);
}
/* test harness ============================================================*/

#include <conio.h>

static UltimateHook l_test;

void main() {
    printf("Ultimate Hook pattern\nQEP version: %s\n"
           "Press 'a'..'c' to inject signals A..C\n"
           "Press 'd' or ESC to inject signal D and quit\n",
           QEP_getVersion());

    UltimateHook_ctor(&l_test);      /* instantiate the UltimateHook object */

                /* trigger the initial transition before dispatching events */
    QHsm_init((QHsm *)&l_test, (QEvent *)0);

    for (;;) {
        QEvent e;
        uint8_t c;

        printf("\n");
        c = (uint8_t)getch();        /* read one character from the console */
        printf("%c: ", c);

        switch (c) {
            case 'a':  e.sig = A_SIG;  break;
            case 'b':  e.sig = B_SIG;  break;
            case 'c':  e.sig = C_SIG;  break;
            case 'd':
            case 0x1B: e.sig = D_SIG;  break;                  /* terminate */
        }
                               /* dispatch the event into the state machine */
        QHsm_dispatch((QHsm *)&l_test, &e);
    }
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    _exit(-1);
}
