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

/*...........................................................................*/
/* Init ADC and I/O */
void IrcA1Proxy_init(void) {
    Adc_init();
    IrcA1_init();
}

/*...........................................................................*/
void IrcA1Proxy_measure(IrcA1* ircA1) {
    ircA1->data->ref = Adc_convert(IRCA1_REF);
    ircA1->data->act = Adc_convert(IRCA1_ACT);
    ircA1->data->tmp = Adc_convert(IRCA1_TMP);
}

/*...........................................................................*/
void IrcA1Proxy_addMeasuresToPayload(IrcA1* ircA1, Payload* payload) {
    // Raw values
    Payload_addByte(payload, ircA1->data->ref);
    Payload_addByte(payload, ircA1->data->act);
    Payload_addByte(payload, ircA1->data->tmp);
}

#endif
