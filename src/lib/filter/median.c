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

#include "median.h"
#include <string.h>
#include <stdio.h>

/** Using register storage class to remain unchanged array */
#define SWAP(a,b) { register UINT16 t=(a);(a)=(b);(b)=t; }

/** Do not refactor this method (efficiency) */
static UINT16 kth_smallest(UINT16* a, UINT8 n, UINT8 k) {
    register UINT8 i, j, l, m;
    register UINT16 x;

    l = 0;
    m = (UINT8) (n - 1);
    while (l < m) {
        x = a[k];
        i = l;
        j = m;
        do {
            while (a[i] < x) {
                i++;
            }
            while (x < a[j]) {
                j--;
            }
            if (i <= j) {
                SWAP(a[i], a[j]);
                i++;
                j--;
            }
        } while (i <= j);
        if (j < k) {
            l = i;
        }
        if (k < i) {
            m = j;
        }
    }
    return a[k];
}

void Median_Create(MedianBuffer* medianBuffer) {
    // Init default values
    medianBuffer->status = MEDIAN_BUFFER_EMPTY;
    medianBuffer->readIndex = 0;
    medianBuffer->writeIndex = 0;
    medianBuffer->size = MEDIAN_BUFFER_SIZE;
    memset(medianBuffer->contents, 0, MEDIAN_BUFFER_SIZE);
    memset(medianBuffer->exportContents, 0, MEDIAN_BUFFER_SIZE);
}

void Median_Add(MedianBuffer* medianBuffer, UINT16 element) {
    medianBuffer->status &= (UINT8) ~MEDIAN_BUFFER_EMPTY;
    if (Median_IsFull(medianBuffer)) {
        Median_OnBufferFull(medianBuffer);
        return;
    }
    medianBuffer->contents[medianBuffer->writeIndex] = element;
    medianBuffer->writeIndex = (UINT8) ((medianBuffer->writeIndex + 1) % medianBuffer->size);
    if (medianBuffer->writeIndex == medianBuffer->readIndex) {
        medianBuffer->status |= MEDIAN_BUFFER_FULL;

        Median_OnBufferFull(medianBuffer);
    }
}

UINT16 Median_Get(MedianBuffer* medianBuffer) {
    UINT16 tmp;
    medianBuffer->status &= (UINT8) ~MEDIAN_BUFFER_FULL;
    if (medianBuffer->status & MEDIAN_BUFFER_EMPTY) {
        return 0;
    }

    tmp = medianBuffer->contents[medianBuffer->readIndex];
    medianBuffer->readIndex = (UINT8) ((medianBuffer->readIndex + 1) % medianBuffer->size);

    if (medianBuffer->writeIndex == medianBuffer->readIndex) {
        medianBuffer->status |= MEDIAN_BUFFER_EMPTY;
    }
    return tmp;
}

BOOL Median_IsFull(MedianBuffer* medianBuffer) {
    return medianBuffer->status & MEDIAN_BUFFER_FULL;
}

BOOL Median_IsEmpty(MedianBuffer* medianBuffer) {
    return medianBuffer->status & MEDIAN_BUFFER_EMPTY;
}

void Median_OnBufferFull(MedianBuffer* medianBuffer) {
}

UINT16 Median_Calculate(MedianBuffer* medianBuffer) {
    // Find the median of the buffer elements, the k-th smallest element problem
    return kth_smallest(medianBuffer->contents, MEDIAN_BUFFER_SIZE, MEDIAN_INDEX);
}

void Median_ReadIndexOffsetAdjust(MedianBuffer* buffer) {
    buffer->readIndex = (UINT8) ((buffer->readIndex + 1) % buffer->size);
}

void Median_BufferExport(MedianBuffer *buffer) {
    UINT8 counter = 0;
    while (Median_IsEmpty(buffer) != MEDIAN_BUFFER_EMPTY) {
        buffer->exportContents[counter] = Median_Get(buffer);
#ifdef _DEBUG_
        printf("%u \n", buffer->exportContents[counter]);
#endif
        counter++;
    }
#ifdef _DEBUG_
    printf("----");
#endif
}
