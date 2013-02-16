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

#ifndef bsp_ambiental_h
#define bsp_ambiental_h

#include "bsp.h"

#define SENSING_MODE                FUZZY_DRIVEN

/*..........................................................................*/
/* SHT11 section */

#define SHT_ID              0x04

#define SHT_TMP_THR         20      // Umbral 80ºC

#define SHT_HUM_ENABLED     0

#define SHT_DATA_PIN     PORTAbits.RA0
#define SHT_DATA         TRISAbits.TRISA0    /* Data tx/rx pin */
#define SHT_DATA_DDR     LATAbits.LATA0	/* Data direction register */
#define SHT_SCK          LATAbits.LATA1	/* Clk pin */
#define SHT_SCK_DDR      TRISAbits.TRISA1    /* Clk direction register */
#define SHT_DATA_CNF     ANCON0bits.PCFG0
#define SHT_SCK_CNF      ANCON0bits.PCFG1

/*...........................................................................*/
/* IRC-A1 section */

#define IRCA1_ID        0x08

#define IRC_A1_SAFE     0
#define IRC_A1_COMB     1
#define IRC_A1_INDU     2

#define IRCA1_MODEL     IRC_A1_COMB

#define IRCA1_CAL_TMP   295

#define IRCA1_X_THR     20      // Umbral 20%

#define IRCA1_REF        ANCON1bits.PCFG10   /* AN10 */
#define IRCA1_REF_DDR    TRISBbits.TRISB1    /* RB1 */
#define IRCA1_ACT        ANCON0bits.PCFG4    /* AN4 */
#define IRCA1_ACT_DDR    TRISAbits.TRISA5    /* RA5 */
#define IRCA1_TMP        ANCON1bits.PCFG8    /* AN8 */
#define IRCA1_TMP_DDR    TRISBbits.TRISB2    /* RB2 */
/* PWM OUTPUT CONFIGURATION FOR VPULSE */
#define IRCA1_CLK_DDR    TRISCbits.TRISC2    /* Vpulse */
#define IRCA1_RPIN       RPOR13              /* Remap-pin register */
#define IRCA1_TIMER      14                  /* Remap-pin register */
#define IRCA1_REG_PERIOD PR2                 /* Vpulse period */
#define IRCA1_DUTY       CCPR1L              /* Vpulse period */
#define IRCA1_TMR        T2CON               /* Timer counter */
#define IRCA1_CCP        CCP1CON             /* Timer counter */
#define IRCA1_PERIOD     255                 /* Period */
#define IRCA1_PRESCALE   0b00000010          /* Prescaler */
#define IRCA1_CCP_PWM    0b00001100          /* PWM mode */
/* ON/OFF PWM */
#define IRCA1_PULSE_ON()    T2CONbits.TMR2ON = 1;
#define IRCA1_PULSE_OFF()   T2CONbits.TMR2ON = 0;

/*..........................................................................*/
/* SENSOR BOARD CONTROL PINS */
#define SENSOR_BOARD_CTRL           LATBbits.LATB3      /* Power on/off pin*/
#define SENSOR_BOARD_CTRL_DDR       TRISBbits.TRISB3      /* Power on/off pin*/
#define SENSOR_BOARD_ON()           SENSOR_BOARD_CTRL = 1   /* Power on */
#define SENSOR_BOARD_OFF()          SENSOR_BOARD_CTRL = 0   /* Power off */

void BSP_ambientalInit(void);

#endif /* umote_ambiental_bsp_h */