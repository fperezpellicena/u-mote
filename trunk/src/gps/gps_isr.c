/**
 * This file is part of uMote.
 *
 *  uMote is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *  uMote is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with uMote.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <p18cxxx.h>
#include "gps_isr.h"
#include "isr.h"

/* Método privado para configurar la interrupción */
static void Gps_configureInterrupt(void);
void Gps_enable(void);
void Gps_disable(void);

/* Capture Gps message via UART */
void Gps_handleInterrupt(void) {
    Gps_disable();
 //   LATDbits.LATD6 = !LATDbits.LATD6; // Test
}

/* Capture Gps message via UART */
void Gps_handleTrackingInterrupt(void) {
   // LATDbits.LATD6 = !LATDbits.LATD6; // Test
}

/* Check for valid gps signal */
BOOL Gps_checkInterrupt(void) {
    return INTCON3bits.INT2IF;
}

/* Clear GPs interrupt flag */
void Gps_clearInterruptFlag(void) {
    INTCON3bits.INT2IF = 0;
}

static void Gps_configureInterrupt(void) {
    /* Output led */
   // TRISDbits.TRISD6 = 0;
     /* Output led */
   // TRISDbits.TRISD7 = 0;
    /* Falling edge */
    INTCON2bits.INTEDG2 = 0;
    /* Input interrupt enable */
    INTCON3bits.INT2IE = 1;
    /* Portb low interrupt priority */
    INTCON3bits.INT2IP = 0;
    /* Clear flag */
    INTCON3bits.INT2IF = 0;
}

/* Enable VDD Gps module*/
void Gps_enable(void) {
  //  LATDbits.LATD7 = 1;
}

/* Disable VDD Gps module */
void Gps_disable(void) {
  //  LATDbits.LATD7 = 0;
}

/** Install interrupt */
void Gps_installInterrupt(void) {
    // Configure interrupt
    Gps_configureInterrupt();
    // Install interrupt handler
//    InterruptHandler_addLO(&Gps_handleInterrupt, &Gps_checkInterrupt,
//            &Gps_clearInterruptFlag);
    // Enable GPS
    Gps_enable();
}
