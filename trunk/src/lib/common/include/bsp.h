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
#include <stdio.h>
#include "GenericTypeDefs.h"

#include "bsp_config.h"

#ifdef MEM_MODEL
#   define MOTE_MEM_MODEL far
#else
#   define MOTE_MEM_MODEL
#endif

/*..........................................................................*/
/* Operating mode */
#define DEBUG		1
#define NORMAL		2
#define MODE		DEBUG

/*..........................................................................*/
/* Sleep mode */
#define SLEEP           1
#define DEEP_SLEEP      2

/*..........................................................................*/
/* IO PIN STATUS */

#define INPUT_PIN	1
#define OUTPUT_PIN	0

#define ON_MCLR		WDTCONbits.DS && DSWAKELbits.DSMCLR
#define ON_DSWDT	WDTCONbits.DS && DSWAKELbits.DSWDT


#define EUSART_SERIAL_INTERRUPT	    1
/*..........................................................................*/
/* UART SECTION */

#define EUSART_9600		    12	// TODO Depends on Fosc

/*..........................................................................*/
/* XBEE SECTION */

#define XBEE_RADIOUS                0x00
#define XBEE_OPTIONS                0x00

#define SLEEP_STATUS_MESSAGES       0	// If xbee uart send sleep status messages on wake/sleep

// Defines based on previous selection
#define XBEE_ON_SLEEP_PIN	    TRISBbits.TRISB0 = 1
#define XBEE_ON_SLEEP_INT	    INTCONbits.INT0IE = 1
#define XBEE_ON_SLEEP_EDGE	    INTCON2bits.INTEDG0 = 1
#define XBEE_ON_SLEEP_FLAG	    INTCONbits.INT0IF
#define XBEE_ON_SLEEP_CLEAR_FLAG    INTCONbits.INT0IF = 0
#define XBEE_ON_SLEEP_AWAKE	    WDTCONbits.DS && DSWAKEHbits.DSINT0

/*...........................................................................*/
/* USB SECTION */

#if USB_ENABLED
#ifdef USB_PLUG_PIN
#define USB_PLUGGED          USB_PLUG_POR == 1
#define ENABLE_USB_ATTACH    USB_PLUG_TRI = 1
#else
#error "USB_PLUG_PIN not defined"
#endif
#endif

#ifdef __18F46J50_H
#define BSP_initLeds()      TRISE &= 0xFC; TRISD &= 0xFC; LATE &= 0x00; LATD &= 0x00;

#define mLED_1              LATEbits.LATE0
#define mLED_2              LATEbits.LATE1
#define mLED_3              LATDbits.LATD0
#define mLED_4              LATDbits.LATD1
#else
#define BSP_initLeds()      LATC &= 0xFC; TRISC &= 0xFC;

#define mLED_1              LATCbits.LATC0
#define mLED_2              LATCbits.LATC1
#define mLED_3
#define mLED_4
#endif

#define mGetLED_1()         mLED_1
#define mGetLED_2()         mLED_2
#define mGetLED_3()         mLED_3;
#define mGetLED_4()         mLED_4;

#define mLED_1_On()         mLED_1 = 1;
#define mLED_2_On()         mLED_2 = 1;
#define mLED_3_On()         mLED_3 = 1;
#define mLED_4_On()         mLED_4 = 1;

#define mLED_1_Off()        mLED_1 = 0;
#define mLED_2_Off()        mLED_2 = 0;
#define mLED_3_Off()        mLED_3 = 0;
#define mLED_4_Off()        mLED_4 = 0;

#define mLED_1_Toggle()     mLED_1 = !mLED_1;
#define mLED_2_Toggle()     mLED_2 = !mLED_2;
#define mLED_3_Toggle()     mLED_3 = !mLED_3;
#define mLED_4_Toggle()     mLED_4 = !mLED_4;

#define mLED_Both_Off()         {mLED_1_Off();mLED_2_Off();}
#define mLED_Both_On()          {mLED_1_On();mLED_2_On();}
#define mLED_Only_1_On()        {mLED_1_On();mLED_2_Off();}
#define mLED_Only_2_On()        {mLED_1_Off();mLED_2_On();}


/*..........................................................................*/
/* SENSOR BOARD CONTROL */
#define SENSOR_BOARD_CTRL_INIT() \
    SENSOR_BOARD_CTRL_DDR = OUTPUT_PIN; \
    SENSOR_BOARD_OFF()


/*..........................................................................*/
/* MODES */
#define MONITORING          1           /* Continuous sensing mode */
#define FUZZY_DRIVEN        2           /* Alert fuzzy based mode */

/*...........................................................................*/
/* GENERAL */
#define MAX_SENSORS                 5                       /* Max sensors */
#define MAX_INTERRUPT_HANDLERS      5            /* Max interrupt handlers */
#define MAX_PAYLOAD                 100

/*...........................................................................*/
/* Prototypes */
void BSP_init(void);
void BSP_enablePLL(void);
void BSP_defaultIO(void);
/* Sleep modes */
void BSP_deepSleep(void);
void BSP_sleep(void);
/* Awake flag control */
void BSP_clearMclrFlags(void);
void BSP_clearWakeUpFlags(void);    // TODO Refactor function name
/* Awake functions implemented by custom BSP */
void BSP_onPowerUp(void);
void BSP_onMclr(void);
void BSP_onWakeUp(void);
void BSP_onDsWdtWakeUp(void);


#endif 	/* bsp_h */