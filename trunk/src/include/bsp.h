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

#ifndef bsp_h
#define bsp_h

#include "GenericTypeDefs.h"


/*..........................................................................*/
/* UART SECTION */

#define EUSART_9600			25	// EUSART 9600 baudrate

/* EUSART1 */
#define EUSART1    			0	// EUSART 1

/* EUSART2 */
#define EUSART2    			1	// EUSART 2


/*..........................................................................*/
/* XBEE SECTION */

#define XBEE_SERIAL     			EUSART1	// Xbee serial port

#define ON_SLEEP_INTERRUPT			2		// Xbee on_sleep interrupt 

// if xbee interrupt mcu via serial, uncomment this define
// #define XBEE_INTERRUPT			SERIAL_INTERRUPT

// else if xbee interrupt mcu via I/O pin, uncomment this define
#define XBEE_INTERRUPT				ON_SLEEP_INTERRUPT

#define SLEEP_STATUS_MESSAGES FALSE	// If xbee uart send sleep status messages on wake/sleep

// Defines based on previous selection
#if XBEE_INTERRUPT == SERIAL_INTERRUPT
#	if XBEE_SERIAL == EUSART1 
#		define EUSART1_INTERRUPT
#	else
#		define EUSART2_INTERRUPT
#	endif
#else
#	define XBEE_ON_SLEEP_PIN()	(TRISBbits.TRISB0 = 1)
#	define XBEE_ON_SLEEP_INT()	(INTCONbits.INT0IE = 1)
#	define XBEE_ON_SLEEP_EDGE()	(INTCON2bits.INTEDG0 = 0)
#	define XBEE_ON_SLEEP_FLAG	INTCONbits.INT0IF
#	define XBEE_ON_SLEEP_CLEAR_FLAG()	(INTCONbits.INT0IF = 0)
#	if XBEE_SERIAL == EUSART1 
#		define EUSART1_POLL
#	else
#		define EUSART2_POLL
#	endif
#endif

#define XBEE_BAUDRATE   EUSART_9600		// Xbee serial baudrate


/*...........................................................................*/
/* Prototypes */
void BSP_init(void);
void BSP_prepareSleep(void);
void BSP_installInterrupts(void);
void BSP_enablePLL(void);


#endif 	/* bsp_h */