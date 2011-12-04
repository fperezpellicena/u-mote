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
#include "qp_port.h"                        /* the port of the QP framework */
#include "bsp.h"                                   /* board support package */
#include "calc2.h"

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>

// Local objects -----------------------------------------------------------*/
static Calc2 l_calc;                             /* instantiate Calculator2 */

//..........................................................................*/
void main() {

    Calc2_ctor(&l_calc);    /* explicitly instantiate the calculator object */

    printf("Calculator example, QEP version: %s\n"
           "Press '0' .. '9'     to enter a digit\n"
           "Press '.'            to enter the decimal point\n"
           "Press '+' or '#'     to add\n"
           "Press '-'            to subtract or negate a number\n"
           "Press '*'            to multiply\n"
           "Press '/'            to divide\n"
           "Press '=' or <Enter> to get the result\n"
           "Press '%'            to enter %\n"
           "Press 'c' or 'C'     to Cancel\n"
           "Press 'e' or 'E'     to Cancel Entry\n"
           "Press <Esc>          to quit.\n\n",
           QEP_getVersion());

    QHsm_init((QHsm *)&l_calc, (QEvent *)0);  /* trigger initial transition */

    for (;;) {                                                /* event loop */
        CalcEvt e;                                      /* Calculator event */

        BSP_display();                                  /* show the display */

        e.key_code = (uint8_t)getche();             /* get a char with echo */
        printf(": ");

        switch (e.key_code) {
            case 'c':                         /* intentionally fall through */
            case 'C': {
                ((QEvent *)&e)->sig = C_SIG;
                break;
            }
            case 'e':                         /* intentionally fall through */
            case 'E': {
                ((QEvent *)&e)->sig = CE_SIG;
                break;
            }
            case '0': {
                ((QEvent *)&e)->sig = DIGIT_0_SIG;
                break;
            }
            case '1':                         /* intentionally fall through */
            case '2':                         /* intentionally fall through */
            case '3':                         /* intentionally fall through */
            case '4':                         /* intentionally fall through */
            case '5':                         /* intentionally fall through */
            case '6':                         /* intentionally fall through */
            case '7':                         /* intentionally fall through */
            case '8':                         /* intentionally fall through */
            case '9': {
                ((QEvent *)&e)->sig = DIGIT_1_9_SIG;
                break;
            }
            case '.': {
                ((QEvent *)&e)->sig = POINT_SIG;
                break;
            }
            case '+':                         /* intentionally fall through */
            case '-':                         /* intentionally fall through */
            case '*':                         /* intentionally fall through */
            case '/': {
                ((QEvent *)&e)->sig = OPER_SIG;
                break;
            }
            case '#': {                                  /* alternative '+' */
                ((QEvent *)&e)->sig = OPER_SIG;
                e.key_code = '+';
                break;
            }
            case '%': {                              /* new event for Calc2 */
                ((QEvent *)&e)->sig = PERCENT_SIG;
                break;
            }
            case '=':                         /* intentionally fall through */
            case '\r': {                                       /* Enter key */
                ((QEvent *)&e)->sig = EQUALS_SIG;
                break;
            }
            case '\33': {                                        /* ESC key */
                ((QEvent *)&e)->sig = OFF_SIG;
                break;
            }
            default: {
                ((QEvent *)&e)->sig = 0;                   /* invalid event */
                break;
            }
        }

        if (((QEvent *)&e)->sig != 0) {           /* valid event generated? */
            QHsm_dispatch((QHsm *)&l_calc, (QEvent *)&e); /* dispatch event */
        }
    }
}