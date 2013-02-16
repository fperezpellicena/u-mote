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

#include "bsp.h"
#include "payload.h"

void Adc_init(void);

void Adc_close(void);

void Adc_startConversion(UINT8 channel);

UINT16 Adc_readValue(void);

UINT16 Adc_convert(UINT8 channel);

UINT16 Adc_convertAveragedValue(UINT8 channel);

UINT16 Adc_testChannelOne(void);

BOOL AdcInterrupt_check(void);

#endif  /* hw_adc_h*/
