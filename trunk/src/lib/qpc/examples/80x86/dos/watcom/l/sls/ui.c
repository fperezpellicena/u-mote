/*****************************************************************************
* Product: User Interface with State-Local Storage Example
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
#include "num_ent.h"
#include "ui.h"
#include "video.h"

/* Local objects -----------------------------------------------------------*/
static UI_mem l_ui;            /* the instance of the UI "container" object */

/* Global objects ----------------------------------------------------------*/
QActive * const AO_UI = (QActive *)&l_ui;    /* opaque pointer to the UI AO */

/*..........................................................................*/
void UI_ctor(void) {                                  /* the global UI ctor */
    UI_top_ctor((UI_top *)&l_ui);
}
/*..........................................................................*/
void UI_top_ctor(UI_top *me) {
    QActive_ctor(&me->super, (QStateHandler)&UI_top_initial);
}

/* HSM definition ----------------------------------------------------------*/
QState UI_top_initial(UI_top *me, QEvent const *e) {
    (void)e;           /* avoid the compiler warning about unused parameter */

                                                  /* subscribe to events... */
    QActive_subscribe((QActive *)me, QUIT_SIG);

    me->history = (QStateHandler)&UI_num_sd_handler;  /* initialize history */

                                 /* send object dictionaries for UI objects */
    QS_OBJ_DICTIONARY(&l_ui);

                             /* send function dictionaries for UI states... */
    QS_FUN_DICTIONARY(&UI_top_handler);
    QS_FUN_DICTIONARY(&UI_top_final);
    QS_FUN_DICTIONARY(&UI_num_handler);
    QS_FUN_DICTIONARY(&UI_num_sd_handler);
    QS_FUN_DICTIONARY(&UI_num_lr_handler);
    QS_FUN_DICTIONARY(&UI_help_handler);

                  /* send signal dictionaries for signals specific to UI... */
    QS_SIG_DICTIONARY(C_SIG,         me);
    QS_SIG_DICTIONARY(CE_SIG,        me);
    QS_SIG_DICTIONARY(DIGIT_0_SIG,   me);
    QS_SIG_DICTIONARY(DIGIT_1_9_SIG, me);
    QS_SIG_DICTIONARY(POINT_SIG,     me);
    QS_SIG_DICTIONARY(NEG_SIG,       me);
    QS_SIG_DICTIONARY(ENTER_SIG,     me);
    QS_SIG_DICTIONARY(UP_SIG,        me);
    QS_SIG_DICTIONARY(DOWN_SIG,      me);
    QS_SIG_DICTIONARY(HELP_SIG,      me);


    return Q_TRAN(&UI_num_handler);
}
/*..........................................................................*/
QState UI_top_handler(UI_top *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            uint8_t c;
            Video_clearScreen(VIDEO_BGND_BLACK);
            Video_clearRect( 0,  0, 80,  7, VIDEO_BGND_LIGHT_GRAY);
            Video_clearRect( 0, 10, 80, 11, VIDEO_BGND_LIGHT_GRAY);
            Video_clearRect( 0, 23, 80, 24, VIDEO_BGND_LIGHT_GRAY);

            c = VIDEO_FGND_BLUE;
            Video_printStrAt(10, 0, c, "  __");
            Video_printStrAt(10, 1, c, " /  |      _   _ -|-     _ _");
            Video_printStrAt(10, 2, c, " \\__| | |  _\\ | \\ | | | | \\ \\");
            Video_printStrAt(10, 3, c, "    | \\_/ |_| | | | \\_| | | |");
            Video_printStrAt(10, 4, c, "    |");
            c = VIDEO_FGND_RED;
            Video_printStrAt(43, 0, c, "    _       __ ");
            Video_printStrAt(43, 1, c, "|  /_\\     |  \\  TM");
            Video_printStrAt(43, 2, c, "|  \\_   _  |__/ _");
            Video_printStrAt(43, 3, c, "|       _\\ |   |_");
            Video_printStrAt(43, 4, c, "|___   |_| |    _|");
            Video_printStrAt(10, 5, VIDEO_FGND_BLUE,
                "_____________________________________________________");
            Video_printStrAt(10, 6, VIDEO_FGND_RED,
                "i n n o v a t i n g   e m b e d d e d   s y s t e m s");
            Video_printStrAt(2,  8, VIDEO_FGND_WHITE,
                "State-Local Storage Example");
            Video_printStrAt(36,  8, VIDEO_FGND_WHITE, "QEP/C");
            Video_printStrAt(45,  8, VIDEO_FGND_YELLOW, QEP_getVersion());
            Video_printStrAt(55,  8, VIDEO_FGND_WHITE, "QF/C");
            Video_printStrAt(64,  8, VIDEO_FGND_YELLOW, QF_getVersion());

            Video_printStrAt(10, 23, VIDEO_FGND_BLUE,
              "* Copyright (c) Quantum Leaps, LLC * www.quantum-leaps.com *");
            Video_printStrAt(28, 24, VIDEO_FGND_LIGHT_RED,
                "<< Press Esc to quit >>");

            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            Video_clearScreen(VIDEO_BGND_BLACK);     /* clear the screen... */
            return Q_HANDLED();
        }
        case QUIT_SIG: {
            return Q_TRAN(&UI_top_final);
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState UI_top_final(UI_top *me, QEvent const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            QF_stop();                               /* stop QF and cleanup */
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}

