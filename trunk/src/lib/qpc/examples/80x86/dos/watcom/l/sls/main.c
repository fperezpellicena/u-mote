/*****************************************************************************
* Product: State-Local Storage Example, 80x86, Vanilla
* Last Updated for Version: 4.0.01
* Date of the Last Update:  Sep 22, 2008
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
#include "qp_port.h"                        /* the port of the QP framework */
#include "bsp.h"                                   /* board support package */
#include "num_ent.h"
#include "ui.h"

/* Local-scope objects -----------------------------------------------------*/
static QEvent const * l_uiQueueSto[5];
static union {
    void  *vp;
    KeyboardEvt qe;
}                     l_smlPoolSto[5];                  /* small event pool */
static QSubscrList    l_subscrSto[MAX_PUB_SIG];

/*..........................................................................*/
void main(int argc, char *argv[]) {
    UI_ctor();                          /* instantiate the UI Active Object */

    BSP_init(argc, argv);           /* initialize the Board Support Package */

    QF_init();     /* initialize the framework and the underlying RT kernel */

                                           /* initialize the event pools... */
    QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));
//  QF_poolInit(l_medPoolSto, sizeof(l_medPoolSto), sizeof(l_medPoolSto[0]));

    QF_psInit(l_subscrSto, Q_DIM(l_subscrSto));   /* init publish-subscribe */

                                                 /* setup the QS filters... */
                            /* send object dictionaries for event queues... */
    QS_OBJ_DICTIONARY(l_uiQueueSto);

                             /* send object dictionaries for event pools... */
    QS_OBJ_DICTIONARY(l_smlPoolSto);
//    QS_OBJ_DICTIONARY(l_medPoolSto);

               /* send signal dictionaries for globally published events... */
    QS_SIG_DICTIONARY(QUIT_SIG, 0);

                                             /* start the active objects... */
    QActive_start(AO_UI,
                  1,                                            /* priority */
                  l_uiQueueSto, Q_DIM(l_uiQueueSto),           /* evt queue */
                  (void *)0, 0,                      /* no per-thread stack */
                  (QEvent *)0);                  /* no initialization event */

    QF_run();                                     /* run the QF application */
}
