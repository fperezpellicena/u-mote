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

#include "filter_median.h"
#include <string.h>

/** Init struct */
void Filter_initMedian(Filter* filter) {
    memset(filter->input, 0, SIGNAL_LENGTH);
    memset(filter->extended, 0, EXTENDED_SIGNAL_LENGTH);
    memset(filter->output, 0, SIGNAL_LENGTH);
    memset(filter->window, 0, WINDOW_LENGTH);
}

static void Filter_extendInputSymmetrically(Filter* filter);

static void Filter_extendInputSymmetrically(Filter* filter) {
    UINT8 i;
    // Create signal extension
    memcpy((void*)filter->extended + 2, (void*)filter->input, (size_t)SIGNAL_LENGTH * sizeof (UINT8));
    for (i = 0; i < EXTENDED_SIGNAL_DELTA; ++i) {
        filter->extended[i] = filter->input[1 - i];
        filter->extended[SIGNAL_LENGTH + 2 + i] = filter->input[SIGNAL_LENGTH - 1 - i];
    }
}


static UINT8 Filter_findMinimunIndex(UINT8 currentIndex, UINT8* window);

static UINT8 Filter_findMinimunIndex(UINT8 currentIndex, UINT8* window) {
    UINT8 min = currentIndex;
    UINT8 i;
    for (i = currentIndex + 1; i < WINDOW_LENGTH; ++i) {
        if (window[i] < window[min]) {
            min = i;
        }
    }
    return min;
}

static void Filter_putMinimunInProperPosition(UINT8* window, UINT8 windowIndex);

static void Filter_putMinimunInProperPosition(UINT8* window, UINT8 windowIndex) {
    UINT8 minIndex = Filter_findMinimunIndex(windowIndex, window);
    UINT8 temp = window[windowIndex];
    window[windowIndex] = window[minIndex];
    window[minIndex] = temp;
}

void Filter_orderWindow(UINT8* window);

void Filter_orderWindow(UINT8* window) {
    UINT8 windowIndex;
    // Order elements (only half of them)
    for (windowIndex = 0; windowIndex < WINDOW_MIDDLE + 1; ++windowIndex) {
        // Put found minimum element in its place
        Filter_putMinimunInProperPosition(window, windowIndex);
    }
}

static void Filter_fillWindow(UINT8* window, UINT8* extended, UINT16 startIndex);

static void Filter_fillWindow(UINT8* window, UINT8* extended, UINT16 startIndex) {
    UINT8 windowIndex;
    UINT16 windowOffset = startIndex - EXTENDED_SIGNAL_DELTA;
    for (windowIndex = 0; windowIndex < WINDOW_LENGTH; ++windowIndex) {
        window[windowIndex] = extended[windowOffset + windowIndex];
    }
}

static void Filter_putWindowMedianInOutput(UINT8* window, UINT8* output, UINT16 currentIndex);

static void Filter_putWindowMedianInOutput(UINT8* window, UINT8* output, UINT16 currentIndex) {
    UINT16 normalizedIndex = currentIndex - EXTENDED_SIGNAL_DELTA;
    output[normalizedIndex] = window[WINDOW_MIDDLE];
}

static void Filter_moveWindowAlongExtended(Filter* filter);

static void Filter_moveWindowAlongExtended(Filter* filter) {
    UINT16 extendedIndex;
    UINT16 extendedLength = EXTENDED_SIGNAL_LENGTH - EXTENDED_SIGNAL_DELTA;
    for (extendedIndex = EXTENDED_SIGNAL_DELTA; extendedIndex < extendedLength; ++extendedIndex) {
        Filter_fillWindow(filter->window, filter->extended, extendedIndex);
        Filter_orderWindow(filter->window);
        Filter_putWindowMedianInOutput(filter->window, filter->output, extendedIndex);
    }
}

/** Calculate median filter of the raw signal */
void Filter_calculateMedianSignal(Filter* filter) {
    // Extend edges
    Filter_extendInputSymmetrically(filter);
    // Move window through all elements of the signal
    Filter_moveWindowAlongExtended(filter);
}
