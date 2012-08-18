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
#include "GenericTypeDefs.h"

/*..........................................................................*/
/* Operating mode */
#define TEST    0
#define NORMAL  1
#define MODE    TEST

/*..........................................................................*/
/* Sleep mode */
#define SLEEP           0
#define DEEP_SLEEP      1
#define SLEEP_MODE      DEEP_SLEEP

/*..........................................................................*/
/* IO PIN STATUS */

#define INPUT_PIN 1
#define OUTPUT_PIN 0


/*..........................................................................*/
/* UART SECTION */

#define EUSART_9600			12	// EUSART 9600 baudrate

/* EUSART1 */
#define EUSART1    			0	// EUSART 1

/* EUSART2 */
#define EUSART2    			1	// EUSART 2


/*..........................................................................*/
/* XBEE SECTION */

#define XBEE_RADIOUS                            0x00
#define XBEE_OPTIONS                            0x00

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
#	define XBEE_ON_SLEEP_PIN	TRISBbits.TRISB0 = 1
#	define XBEE_ON_SLEEP_INT	INTCONbits.INT0IE = 1
#	define XBEE_ON_SLEEP_EDGE	INTCON2bits.INTEDG0 = 1
#	define XBEE_ON_SLEEP_FLAG	INTCONbits.INT0IF
#	define XBEE_ON_SLEEP_CLEAR_FLAG	INTCONbits.INT0IF = 0
#       define XBEE_ON_SLEEP_AWAKE      WDTCONbits.DS && DSWAKEHbits.DSINT0
#	if XBEE_SERIAL == EUSART1 
#		define EUSART1_POLL
#	else
#		define EUSART2_POLL
#	endif
#endif

#define XBEE_BAUDRATE   EUSART_9600		// Xbee serial baudrate

#define AUTO_JOIN_ON_POWER_UP   TRUE           // Join on power
#define ON_MCLR                 DSWAKELbits.DSMCLR == 1


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


/*...........................................................................*/
/* ADC SECTION */

#define ADC_INT_ENABLED     0

#define AVERAGE_FACTOR  16
#define DIV_AVERAGE     4


/*..........................................................................*/
/* SHT11 section */

#define SHT_ENABLED
#define SHT_ID              0xAA

#define SHT_TMP_THR         20      // Umbral 80ºC

#define SHT_HUM_ENABLED     0

#ifdef SHT_ENABLED
#   define SHT_DATA_PIN     PORTAbits.RA0
#   define SHT_DATA         TRISAbits.TRISA0    /* Data tx/rx pin */
#   define SHT_DATA_DDR     LATAbits.LATA0	/* Data direction register */
#   define SHT_SCK          LATAbits.LATA1	/* Clk pin */
#   define SHT_SCK_DDR      TRISAbits.TRISA1    /* Clk direction register */
#   define SHT_DATA_CNF     ANCON0bits.PCFG0
#   define SHT_SCK_CNF      ANCON0bits.PCFG1
#endif

/*...........................................................................*/
/* IRC-A1 section */

#define IRCA1_ENABLED
#define IRCA1_ID        2

#define IRC_A1_SAFE     0
#define IRC_A1_COMB     1
#define IRC_A1_INDU     2

#define IRCA1_MODEL     IRC_A1_COMB

#define IRCA1_CAL_TMP   295

#define IRCA1_X_THR     20      // Umbral 20%

#ifdef IRCA1_ENABLED
#   define IRCA1_REF        ANCON0bits.PCFG0  /* AN0 */
#   define IRCA1_REF_DDR    TRISAbits.TRISA0
#   define IRCA1_ACT        ANCON0bits.PCFG1  /* AN1 */
#   define IRCA1_ACT_DDR    TRISAbits.TRISA1
#   define IRCA1_TMP        ANCON0bits.PCFG2  /* AN2 */
#   define IRCA1_TMP_DDR    TRISAbits.TRISA2
#   define IRCA1_CLK        
#   define IRCA1_CLK_DDR
#endif


/*..........................................................................*/

#define MONITORING                  0           /* Continuous sensing mode */
#define THRESHOLD_DRIVEN            1           /* Alert only sensing mode */
#define FUZZY_DRIVEN                2           /* Alert fuzzy based mode */
#define SENSING_MODE                FUZZY_DRIVEN

#define MAX_SENSORS                 5                       /* Max sensors */
#define SENSORS_PWR                 LATBbits.LATB3      /* Power on/off pin*/
#define SENSORS_PWR_DDR             TRISBbits.TRISB3

#define MAX_INTERRUPT_HANDLERS      5            /* Max interrupt handlers */
#define MAX_LIST_SIZE               100
#define SENSORS                     1

/*...........................................................................*/
/* Prototypes */
void BSP_init(void);
void BSP_prepareSleep(void);
void BSP_enablePLL(void);


#endif 	/* bsp_h */