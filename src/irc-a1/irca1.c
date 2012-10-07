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

#include "irca1.h"
#include "register.h"

#if IRCA1_ENABLED
#   include <math.h>

/* Calculation factors previously done */
static float ircaPowCal[] = {-0.058, -0.199, -0.698};   // -b^(1/c)
static float ircaC[] = {0.520, 0.528, 0.698};           // c

/*...........................................................................*/

static void IrcA1_initIO(void);
static void IrcA1_initIO(void) {
    IRCA1_REF = 0;      /* AN0 analog */
    IRCA1_REF_DDR = 1;  /* AN0 as input */
    IRCA1_ACT = 0;      /* AN1 analog */
    IRCA1_ACT_DDR = 1;  /* AN1 as input */
    IRCA1_TMP = 0;      /* AN1 analog */
    IRCA1_TMP_DDR = 1;  /* AN2 as input */
    IRCA1_CLK_DDR = 0;  /* PWM output */
}

static void IrcA1_initPWM(void);
static void IrcA1_initPWM(void) {
    IRCA1_REG_PERIOD = IRCA1_PERIOD; /* PWM period */
    IRCA1_DUTY = IRCA1_PERIOD / 2;   /* PWM duty */
    IRCA1_TMR |= IRCA1_PRESCALE;    /* Prescale 16 */
    IRCA1_CCP |= IRCA1_CCP_PWM;  /* PWM mode*/
    IRCA1_PULSE_ON();
}

/* Init sensor struct and hw associated */
void IrcA1_init(void) {
    IrcA1_initIO();
    Register_remap(&IRCA1_RPIN, IRCA1_TIMER);
    IrcA1_initPWM();
}

/*...........................................................................*/

/* IrcA1 calculate CO2 */
void IrcA1_calculate(IrcA1* ircA1) {
    float abs;
    // Operates with sensor data and calibration data
    abs = 1 - (ircA1->data->act / (ircA1->data->act * ircA1->cal->zero));
    if (abs > 0) {
        ircA1->data->x = (pow(log(1 - (abs / ircA1->cal->span)),(1 / ircaC[IRCA1_MODEL])))
                / ircaPowCal[IRCA1_MODEL];
    } else {
          ircA1->data->x = (pow(log(1 + (abs / ircA1->cal->span)), (1 / ircaC[IRCA1_MODEL])))
                / ircaPowCal[IRCA1_MODEL];
    }
}
#endif
