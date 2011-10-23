/*****************************************************************************
* Product: Console-based BSP, MinGW
* Last Updated for Version: 4.1.07
* Date of the Last Update:  Mar 03, 2011
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2011 Quantum Leaps, LLC. All rights reserved.
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

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

Q_DEFINE_THIS_FILE

/* local variables ---------------------------------------------------------*/
static uint8_t l_running;

/*..........................................................................*/
static DWORD WINAPI idleThread(LPVOID par) {/* signature for CreateThread() */
    (void)par;
    l_running = (uint8_t)1;
    while (l_running) {
        Sleep(10);                                      /* wait for a while */
        if (_kbhit()) {                                 /* any key pressed? */
            BSP_onKeyboardInput(_getch());
        }
    }
    return 0;                                             /* return success */
}
/*..........................................................................*/
void BSP_init(int argc, char *argv[]) {
    (void)argc;        /* avoid the compiler warning about unused parameter */
    (void)argv;        /* avoid the compiler warning about unused parameter */
}
/*..........................................................................*/
void QF_onStartup(void) {
    DWORD threadId;
    HANDLE hIdle;

    hIdle = CreateThread(NULL, 1024, &idleThread, (void *)0, 0, &threadId);
    Q_ASSERT(hIdle != (HANDLE)0);                 /* thread must be created */
    SetThreadPriority(hIdle, THREAD_PRIORITY_IDLE);
}
/*..........................................................................*/
void QF_onCleanup(void) {
    l_running = (uint8_t)0;
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    QF_stop();
}

