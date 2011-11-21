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

#ifndef hw_adc_h
#define hw_adc_h

#include "qep_port.h"


#ifndef AVERAGE_FACTOR
    #define AVERAGE_FACTOR  16
    #define DIV_AVERAGE     4
#endif

void Adc_init(uint8_t config, uint8_t config2, uint8_t channel);

void Adc_close();

void Adc_startConversion();

uint16_t Adc_readValue();

uint16_t Adc_getValue();

uint16_t Adc_getAveragedValue();

#endif  /* hw_adc_h*/
