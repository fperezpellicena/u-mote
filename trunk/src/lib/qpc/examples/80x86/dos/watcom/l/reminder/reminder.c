/*****************************************************************************
* Product: Reminder state pattern example
* Last Updated for Version: 4.1.01
* Date of the Last Update:  Nov 04, 2009
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
#include "bsp.h"

#include <stdio.h>

Q_DEFINE_THIS_FILE

/*..........................................................................*/
enum SensorSignals {
    TIMEOUT_SIG = Q_USER_SIG,                /* the periodic timeout signal */
    DATA_READY_SIG,                         /* the invented reminder signal */
    TERMINATE_SIG                              /* terminate the application */
};
/*..........................................................................*/
typedef struct SensorTag {                      /* the Sensor active object */
    QActive super;                                   /* derive from QActive */

    QTimeEvt timeEvt;                       /* private time event generator */
    uint16_t pollCtr;
    uint16_t procCtr;
} Sensor;


void Sensor_ctor(Sensor *me);
                                          /* hierarchical state machine ... */
static QState Sensor_initial   (Sensor *me, QEvent const *e);
static QState Sensor_polling   (Sensor *me, QEvent const *e);
static QState Sensor_processing(Sensor *me, QEvent const *e);
static QState Sensor_idle      (Sensor *me, QEvent const *e);
static QState Sensor_busy      (Sensor *me, QEvent const *e);
static QState Sensor_final     (Sensor *me, QEvent const *e);

/*..........................................................................*/
void Sensor_ctor(Sensor *me) {
    QActive_ctor(&me->super, (QStateHandler)&Sensor_initial);
    QTimeEvt_ctor(&me->timeEvt, TIMEOUT_SIG);
}

/* HSM definition ----------------------------------------------------------*/
QState Sensor_initial(Sensor *me, QEvent const *e) {
    (void)e;               /* avoid compiler warning about unused parameter */

    me->pollCtr = 0;
    me->procCtr = 0;

    return Q_TRAN(&Sensor_polling);
}
/*..........................................................................*/
QState Sensor_final(Sensor *me, QEvent const *e) {
    (void)me;              /* avoid compiler warning about unused parameter */
    (void)e;               /* avoid compiler warning about unused parameter */
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("final-ENTRY;\nBye!Bye!\n");
            QF_stop();                         /* terminate the application */
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState Sensor_polling(Sensor *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
                                       /* periodic timeout every 1/2 second */
            QTimeEvt_postEvery(&me->timeEvt, (QActive *)me,
                               BSP_TICKS_PER_SEC/2);
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            QTimeEvt_disarm(&me->timeEvt);
            return Q_HANDLED();
        }
        case Q_INIT_SIG: {
            return Q_TRAN(&Sensor_processing);
        }
        case TIMEOUT_SIG: {
            /* NOTE: this constant event is statically pre-allocated.
            * It can be posted/published as any other event.
            */
            static const QEvent reminderEvt = { DATA_READY_SIG, 0 };

            ++me->pollCtr;
            printf("polling %3d\n", me->pollCtr);
            if ((me->pollCtr & 0x3) == 0) {                     /* modulo 4 */
                QActive_postFIFO((QActive *)me, &reminderEvt);
            }
            return Q_HANDLED();
        }
        case TERMINATE_SIG: {
            return Q_TRAN(&Sensor_final);
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState Sensor_processing(Sensor *me, QEvent const *e) {
    switch (e->sig) {
        case Q_INIT_SIG: {
            return Q_TRAN(&Sensor_idle);
        }
    }
    return Q_SUPER(&Sensor_polling);
}
/*..........................................................................*/
QState Sensor_idle(Sensor *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("idle-ENTRY;\n");
            return Q_HANDLED();
        }
        case DATA_READY_SIG: {
            return Q_TRAN(&Sensor_busy);
        }
    }
    return Q_SUPER(&Sensor_processing);
}
/*..........................................................................*/
QState Sensor_busy(Sensor *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("busy-ENTRY;\n");
            return Q_HANDLED();
        }
        case TIMEOUT_SIG: {
            ++me->procCtr;
            printf("processing %3d\n", me->procCtr);
            if ((me->procCtr & 0x1) == 0) {                     /* modulo 2 */
                return Q_TRAN(&Sensor_idle);
            }
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&Sensor_processing);
}

/* test harness ============================================================*/

/* Local-scope objects -----------------------------------------------------*/
static Sensor l_sensor;                         /* the Sensor active object */
static QEvent const *l_sensorQSto[10];    /* Event queue storage for Sensor */

/*..........................................................................*/
int main(int argc, char *argv[]) {
    printf("Reminder state pattern\nQEP version: %s\nQF  version: %s\n"
           "Press ESC to quit...\n",
           QEP_getVersion(), QF_getVersion());

    BSP_init(argc, argv);                             /* initialize the BSP */

    QF_init();    /* initialize the framework and the underlying RT kernel */

    /* publish-subscribe not used, no call to QF_psInit() */
    /* dynamic event allocation not used, no call to QF_poolInit() */

                             /* instantiate and start the active objects... */
    Sensor_ctor(&l_sensor);
    QActive_start((QActive *)&l_sensor, 1, l_sensorQSto, Q_DIM(l_sensorQSto),
                  (void *)0, 0, (QEvent *)0);

    QF_run();                                     /* run the QF application */

    return 0;
}
/*..........................................................................*/
void BSP_onKeyboardInput(uint8_t key) {
    switch (key) {
        case 129: {                                         /* ESC pressed? */
            /* NOTE: this constant event is statically pre-allocated.
            * It can be posted/published as any other event.
            */
            static QEvent const terminateEvt = { TERMINATE_SIG, 0};
            QActive_postFIFO((QActive *)&l_sensor, &terminateEvt);
            break;
        }
    }
}
