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
#define DEBUG   0
#define NORMAL  1
#define MODE    DEBUG

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

#define ON_SLEEP_INTERRUPT			2		// Xbee on_sleep interrupt 

// if xbee interrupt mcu via serial, uncomment this define
// #define XBEE_INTERRUPT			SERIAL_INTERRUPT

// else if xbee interrupt mcu via I/O pin, uncomment this define
#define XBEE_INTERRUPT				ON_SLEEP_INTERRUPT

#define SLEEP_STATUS_MESSAGES       0	// If xbee uart send sleep status messages on wake/sleep

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

#define XBEE_BAUDRATE   EUSART_9600     // Xbee serial baudrate

#define ON_MCLR         WDTCONbits.DS && DSWAKELbits.DSMCLR


/*...........................................................................*/
/* USB SECTION */

#define USB_ENABLED         1

/* USB attach detector */
#define USB_PLUG_PIN        PORTBbits.RB4

#if USB_ENABLED
#   ifdef USB_PLUG_PIN
#       define USB_PLUGGED          USB_PLUG_PIN == 1
#       define ENABLE_USB_ATTACH    TRISBbits.TRISB4 = 1
#   else
#       error "USB_PLUG_PIN not defined"
#   endif
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


/*...........................................................................*/
/* RTCC SECTION */

#define RTCC_ENABLED        1


/*...........................................................................*/
/* ADC SECTION */

#define ADC_INT_ENABLED     0

#define AVERAGE_FACTOR  16
#define DIV_AVERAGE     4


/*..........................................................................*/
/* SHT11 section */

#define SHT_ENABLED         1
#define SHT_ID              0x04

#define SHT_TMP_THR         20      // Umbral 80ºC

#define SHT_HUM_ENABLED     0

#if SHT_ENABLED
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

#define IRCA1_ENABLED   1
#define IRCA1_ID        0x08

#define IRC_A1_SAFE     0
#define IRC_A1_COMB     1
#define IRC_A1_INDU     2

#define IRCA1_MODEL     IRC_A1_COMB

#define IRCA1_CAL_TMP   295

#define IRCA1_X_THR     20      // Umbral 20%

#if IRCA1_ENABLED
#   define IRCA1_REF        ANCON1bits.PCFG10   /* AN10 */
#   define IRCA1_REF_DDR    TRISBbits.TRISB1    /* RB1 */
#   define IRCA1_ACT        ANCON0bits.PCFG4    /* AN4 */
#   define IRCA1_ACT_DDR    TRISAbits.TRISA5    /* RA5 */
#   define IRCA1_TMP        ANCON1bits.PCFG8    /* AN8 */
#   define IRCA1_TMP_DDR    TRISBbits.TRISB2    /* RB2 */
/* PWM OUTPUT CONFIGURATION FOR VPULSE */
#   define IRCA1_CLK_DDR    TRISCbits.TRISC2    /* Vpulse */
#   define IRCA1_RPIN       RPOR13              /* Remap-pin register */
#   define IRCA1_TIMER      14                  /* Remap-pin register */
#   define IRCA1_REG_PERIOD PR2                 /* Vpulse period */
#   define IRCA1_DUTY       CCPR1L              /* Vpulse period */
#   define IRCA1_TMR        T2CON               /* Timer counter */
#   define IRCA1_CCP        CCP1CON             /* Timer counter */
#   define IRCA1_PERIOD     255                 /* Period */
#   define IRCA1_PRESCALE   0b00000010          /* Prescaler */
#   define IRCA1_CCP_PWM    0b00001100          /* PWM mode */
/* ON/OFF PWM */
#   define IRCA1_PULSE_ON()    T2CONbits.TMR2ON = 1;
#   define IRCA1_PULSE_OFF()   T2CONbits.TMR2ON = 0;
#endif



/*..........................................................................*/

#define MONITORING                  0           /* Continuous sensing mode */
#define THRESHOLD_DRIVEN            1           /* Alert only sensing mode */
#define FUZZY_DRIVEN                2           /* Alert fuzzy based mode */
#define SENSING_MODE                FUZZY_DRIVEN

#define MAX_SENSORS                 5                       /* Max sensors */
#define SENSOR_BOARD_CTRL           LATBbits.LATB3      /* Power on/off pin*/
#define SENSOR_BOARD_CTRL_DDR       TRISBbits.TRISB3      /* Power on/off pin*/
#define SENSOR_BOARD_ON()           SENSOR_BOARD_CTRL = 1   /* Power on */
#define SENSOR_BOARD_OFF()          SENSOR_BOARD_CTRL = 0   /* Power off */
#define SENSOR_BOARD_CTRL_INIT() \
    SENSOR_BOARD_CTRL_DDR = OUTPUT_PIN; \
    SENSOR_BOARD_OFF()

#define MAX_INTERRUPT_HANDLERS      5            /* Max interrupt handlers */
#define MAX_PAYLOAD                 100
#define SENSORS                     1
#define NO_SENSORS                  0x00                  /* No sensors Id */
#define ON_DSWDT                  WDTCONbits.DS && DSWAKELbits.DSWDT

/*...........................................................................*/
/* Prototypes */
void BSP_init(void);
void BSP_enablePLL(void);
void BSP_defaultIO(void);

void BSP_deepSleep(void);
void BSP_sleep(void);

void BSP_onPowerUp(void);
void BSP_onMclr(void);
void BSP_onWakeUp(void);
void BSP_onDsWdtWakeUp(void);


#endif 	/* bsp_h */