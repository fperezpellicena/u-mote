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

#ifndef filter_median_h
#define filter_median_h

#include "GenericTypeDefs.h"

#define SIGNAL_LENGTH               450
#define EXTENDED_SIGNAL_SYM_DELTA   4
#define EXTENDED_SIGNAL_DELTA       EXTENDED_SIGNAL_SYM_DELTA / 2
#define EXTENDED_SIGNAL_LENGTH      SIGNAL_LENGTH + EXTENDED_SIGNAL_DELTA
#define WINDOW_LENGTH               13
#define WINDOW_MIDDLE               WINDOW_LENGTH / 2

typedef struct Filter Filter;

struct Filter {
    UINT8 input[SIGNAL_LENGTH];
    UINT8 extended[EXTENDED_SIGNAL_LENGTH];
    UINT8 output[SIGNAL_LENGTH];
    UINT8 window[WINDOW_LENGTH];
};

/** Init struct */
void Filter_initMedian(Filter* filter);

/** Calculate median filter of the raw signal */
void Filter_calculateMedianSignal(Filter* filter);


#endif