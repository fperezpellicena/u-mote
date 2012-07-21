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
#include <math.h>

static float ircaPowCal[] = {-0.058, -0.199, -0.698};   // -b^(1/c)
static float ircaC[] = {0.520, 0.528, 0.698};           // c

/*...........................................................................*/

/* Init sensor struct and hw associated */
void IrcA1_init(void) {
    /* Set pin I/O */
    IRCA1_REF_DDR = 1;  /* Ref analog pin as input */
    IRCA1_ACT_DDR = 1;  /* Act analog pin as input */
    IRCA1_TMP_DDR = 1;  /* Tmp analog pin as input */
    /* Set A/D config register */
    IRCA1_REF = 0;      /* Ref pin configured as analog channel */
    IRCA1_ACT = 0;      /* Act pin configured as analog channel */
    IRCA1_TMP = 0;      /* Tmp pin configured as analog channel */
}

/*...........................................................................*/

/* IrcA1 calculate CO2 */
void IrcA1_calculate(IrcA1* ircA1) {
    float abs;
    // Operates with sensor data and calibration data
    abs = 1 - (ircA1->data.act / (ircA1->data.act * ircA1->cal.zero));
    if (abs > 0) {
        ircA1->data.x = (pow(log(1 - (abs / ircA1->cal.span)),(1 / ircaC[IRCA1_MODEL])))
                / ircaPowCal[IRCA1_MODEL];
    } else {
          ircA1->data.x = (pow(log(1 + (abs / ircA1->cal.span)), (1 / ircaC[IRCA1_MODEL])))
                / ircaPowCal[IRCA1_MODEL];
    }
}