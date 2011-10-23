/*****************************************************************************
* Product: QHsmTst Example
* Last Updated for Version: 4.0.00
* Date of the Last Update:  Apr 05, 2008
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
#include "qhsmtst.h"

/* local-scope objects .....................................................*/
typedef struct QHsmTstTag {
    QHsm super;                                        /* extend class QHsm */
    uint8_t foo;                                 /* extended state variable */
    /* . . . */
} QHsmTst;

/* HSM state handler functions: */
static QState QHsmTst_initial(QHsmTst *me, QEvent const *e); /* pseudostate */
static QState QHsmTst_s    (QHsmTst *me, QEvent const *e); /* state-handler */
static QState QHsmTst_s1   (QHsmTst *me, QEvent const *e); /* state-handler */
static QState QHsmTst_s11  (QHsmTst *me, QEvent const *e); /* state-handler */
static QState QHsmTst_s2   (QHsmTst *me, QEvent const *e); /* state-handler */
static QState QHsmTst_s21  (QHsmTst *me, QEvent const *e); /* state-handler */
static QState QHsmTst_s211 (QHsmTst *me, QEvent const *e); /* state-handler */

static QHsmTst l_hsmtst;   /* the sole instance of the state machine object */

/* global-scope definitions ------------------------------------------------*/
QHsm * const the_hsm = (QHsm *)&l_hsmtst;             /* the opaque pointer */

/*..........................................................................*/
void QHsmTst_ctor(void) {
    QHsmTst *me = &l_hsmtst;
    QHsm_ctor(&me->super, (QStateHandler)&QHsmTst_initial);
}

/* HSM definition ----------------------------------------------------------*/
QState QHsmTst_initial(QHsmTst *me, QEvent const *e) {
    (void)e;                      /* suppress "e not used" compiler warning */
    BSP_display("top-INIT;");
    me->foo = 0;                      /* initialize extended state variable */
    return Q_TRAN(&QHsmTst_s2);
}
/*..........................................................................*/
QState QHsmTst_s(QHsmTst *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_display("s-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_display("s-EXIT;");
            return Q_HANDLED();
        }
        case Q_INIT_SIG: {
            BSP_display("s-INIT;");
            return Q_TRAN(&QHsmTst_s11);
        }
        case E_SIG: {
            BSP_display("s-E;");
            return Q_TRAN(&QHsmTst_s11);
        }
        case I_SIG: {                   /* internal transition with a guard */
            if (me->foo) {
                BSP_display("s-I;");
                me->foo = 0;
                return Q_HANDLED();
            }
            break;
        }
        case TERMINATE_SIG: {
            BSP_exit();
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState QHsmTst_s1(QHsmTst *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_display("s1-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_display("s1-EXIT;");
            return Q_HANDLED();
        }
        case Q_INIT_SIG: {
            BSP_display("s1-INIT;");
            return Q_TRAN(&QHsmTst_s11);
        }
        case A_SIG: {
            BSP_display("s1-A;");
            return Q_TRAN(&QHsmTst_s1);
        }
        case B_SIG: {
            BSP_display("s1-B;");
            return Q_TRAN(&QHsmTst_s11);
        }
        case C_SIG: {
            BSP_display("s1-C;");
            return Q_TRAN(&QHsmTst_s2);
        }
        case D_SIG: {                            /* transition with a gurad */
            if (!me->foo) {
                BSP_display("s1-D;");
                me->foo = 1;
                return Q_TRAN(&QHsmTst_s);
            }
            break;
        }
        case F_SIG: {
            BSP_display("s1-F;");
            return Q_TRAN(&QHsmTst_s211);
        }
        case I_SIG: {                                /* internal transition */
            BSP_display("s1-I;");
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsmTst_s);
}
/*..........................................................................*/
QState QHsmTst_s11(QHsmTst *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_display("s11-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_display("s11-EXIT;");
            return Q_HANDLED();
        }
        case D_SIG: {                            /* transition with a gurad */
            if (me->foo) {
                BSP_display("s11-D;");
                me->foo = 0;
                return Q_TRAN(&QHsmTst_s1);
            }
            break;
        }
        case G_SIG: {
            BSP_display("s11-G;");
            return Q_TRAN(&QHsmTst_s211);
        }
        case H_SIG: {
            BSP_display("s11-H;");
            return Q_TRAN(&QHsmTst_s);
        }
    }
    return Q_SUPER(&QHsmTst_s1);
}
/*..........................................................................*/
QState QHsmTst_s2(QHsmTst *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_display("s2-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_display("s2-EXIT;");
            return Q_HANDLED();
        }
        case Q_INIT_SIG: {
            BSP_display("s2-INIT;");
            return Q_TRAN(&QHsmTst_s211);
        }
        case C_SIG: {
            BSP_display("s2-C;");
            return Q_TRAN(&QHsmTst_s1);
        }
        case F_SIG: {
            BSP_display("s2-F;");
            return Q_TRAN(&QHsmTst_s11);
        }
        case I_SIG: {                   /* internal transition with a guard */
            if (!me->foo) {
                BSP_display("s2-I;");
                me->foo = (uint8_t)1;
                return Q_HANDLED();
            }
            break;
        }
    }
    return Q_SUPER(&QHsmTst_s);
}
/*..........................................................................*/
QState QHsmTst_s21(QHsmTst *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_display("s21-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_display("s21-EXIT;");
            return Q_HANDLED();
        }
        case Q_INIT_SIG: {
            BSP_display("s21-INIT;");
            return Q_TRAN(&QHsmTst_s211);
        }
        case A_SIG: {
            BSP_display("s21-A;");
            return Q_TRAN(&QHsmTst_s21);
        }
        case B_SIG: {
            BSP_display("s21-B;");
            return Q_TRAN(&QHsmTst_s211);
        }
        case G_SIG: {
            BSP_display("s21-G;");
            return Q_TRAN(&QHsmTst_s1);
        }
    }
    return Q_SUPER(&QHsmTst_s2);
}
/*..........................................................................*/
QState QHsmTst_s211(QHsmTst *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_display("s211-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_display("s211-EXIT;");
            return Q_HANDLED();
        }
        case D_SIG: {
            BSP_display("s211-D;");
            return Q_TRAN(&QHsmTst_s21);
        }
        case H_SIG: {
            BSP_display("s211-H;");
            return Q_TRAN(&QHsmTst_s);
        }
    }
    return Q_SUPER(&QHsmTst_s21);
}

