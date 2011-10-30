/*****************************************************************************
* BSP for PICDEM 2 PLUS with PIC18F452
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
#ifndef bsp_h
#define bsp_h

#include <p18f46j50.h>           /* SFR declarations for PIC18F46J50 device */
#include <usart.h>                                        	/* UART library */
#include <timers.h>										/* Timers library */

                                                /* The Oscillator frequency */
#define BSP_FOSC_HZ          12000000UL

/*--------------------------------------------------------------------------*/
                                               /* the system tick rate [Hz] */
#define BSP_TICKS_PER_SEC    20UL

void BSP_init(void);

#ifdef Q_SPY
    extern QSTimeCtr BSP_tickTime;
#endif

void Interrupts_enable(void);

void Timer0_init(void);

void Usart1_init(void);

void Usart1_write(uint8_t byte);


#endif                                                             /* bsp_h */

