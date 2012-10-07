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

#include "ircA1_proxy.h"

#if IRCA1_ENABLED
#   include "hw_adc.h"

static float_t ircaExpCal[] = {-0.058, -0.199, -0.698};  // e^(-b*x^c)

/*...........................................................................*/
/* Init ADC and I/O */
void IrcA1Proxy_init(void) {
    Adc_init();
    IrcA1_init();
}

/*...........................................................................*/
void IrcA1Proxy_sense(Payload* measures) {
    IrcA1 ircA1;
    ircA1.data->ref = Adc_convert(IRCA1_REF);
    ircA1.data->act = Adc_convert(IRCA1_ACT);
    ircA1.data->tmp = Adc_convert(IRCA1_TMP);
    Payload_add(measures, ircA1.data->ref);
    Payload_add(measures, ircA1.data->act);
    Payload_add(measures, ircA1.data->tmp);
}

void IrcA1Proxy_pulseOn(void) {
    IRCA1_PULSE_ON();
}

void IrcA1Proxy_pulseOff(void) {
    IRCA1_PULSE_OFF();
}
#endif
