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

#include <p18cxxx.h>

/*..........................................................................*/
/* IO PIN STATUS */

#define INPUT_PIN 1
#define OUTPUT_PIN 0


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
#       define XBEE_ON_SLEEP_WAKE()     (WDTCONbits.DS && DSWAKEHbits.DSINT0)
#	if XBEE_SERIAL == EUSART1 
#		define EUSART1_POLL
#	else
#		define EUSART2_POLL
#	endif
#endif

#define XBEE_BAUDRATE   EUSART_9600		// Xbee serial baudrate


/*...........................................................................*/
/* USB SECTION */

#define USB_ENABLED

/* USB attach detector */
#define USB_PLUG_PIN      PORTBbits.RB4

#ifdef USB_ENABLED
#   ifdef USB_PLUG_PIN
#       define USB_PLUGGED    USB_PLUG_PIN == 1
#       define ENABLE_USB_ATTACH     TRISBbits.TRISB4 = 1
#   else
#       error "USB_PLUG_PIN not defined"
#   endif
#endif

/*...........................................................................*/
/* RTCC SECTION */

#define RTCC_ENABLED

#ifdef RTCC_ENABLED
#   define ENABLE_RTCC    RTCCFGbits.RTCEN = 1
#endif


#define mInitAllLEDs()      LATE &= 0xFC; TRISE &= 0xFC;

#define mLED_1              LATEbits.LATE0
#define mLED_2              LATEbits.LATE1
#define mLED_3
#define mLED_4

#define mGetLED_1()         mLED_1
#define mGetLED_2()         mLED_2
#define mGetLED_3()         1
#define mGetLED_4()         1

#define mLED_1_On()         mLED_1 = 1;
#define mLED_2_On()         mLED_2 = 1;
#define mLED_3_On()
#define mLED_4_On()

#define mLED_1_Off()        mLED_1 = 0;
#define mLED_2_Off()        mLED_2 = 0;
#define mLED_3_Off()
#define mLED_4_Off()

#define mLED_1_Toggle()     mLED_1 = !mLED_1;
#define mLED_2_Toggle()     mLED_2 = !mLED_2;
#define mLED_3_Toggle()
#define mLED_4_Toggle()


/*...........................................................................*/
/* Prototypes */
void BSP_init(void);
void BSP_prepareSleep(void);
void BSP_enablePLL(void);


#endif 	/* bsp_h */