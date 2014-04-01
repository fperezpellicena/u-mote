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

#include "ee.h"
#include "bsp.h"
#include "math.h"
#include <string.h>

void EE_Create(EEBuffer* eeBuffer) {
    eeBuffer->size = EE_BUFFER_SIZE;
    eeBuffer->readIndex = 0;
    eeBuffer->writeIndex = 0;
    eeBuffer->writeIndexHistory = 0;
    eeBuffer->status = EE_BUFFER_EMPTY;
    memset(eeBuffer->input, 0, EE_BUFFER_SIZE);
    memset(eeBuffer->tmp, 0, EE_BUFFER_SIZE);
#ifdef _DEBUG_
    printf("****** EE_Create ******\n");
    printf("eeBuffer->readIndex = %i \n", eeBuffer->readIndex);
    printf("eeBuffer->writeIndex = %i \n", eeBuffer->writeIndex);
    printf("eeBuffer->size = %i \n", eeBuffer->size);
    printf("eeBuffer->status = %i \n", eeBuffer->status);
#endif
}

void EE_Add(EEBuffer* eeBuffer, INT16 element) {
    eeBuffer->status &= (UINT8) ~EE_BUFFER_EMPTY;
#ifdef _DEBUG_
   printf("eeBuffer->status = %i \n", eeBuffer->status);
#endif
    if (EE_IsFull(eeBuffer)) {
#ifdef _DEBUG_
    printf("EE buffer is full! Returning... \n");
#endif
        return;
    }
    eeBuffer->input[eeBuffer->writeIndex] = element;
#ifdef _DEBUG_
    printf("eeBuffer->contents[%i] = %i \n",
            eeBuffer->writeIndex, eeBuffer->input[eeBuffer->writeIndex]);
#endif
    eeBuffer->writeIndex = (UINT8) ((eeBuffer->writeIndex + 1) % eeBuffer->size);
    if (eeBuffer->writeIndex == eeBuffer->readIndex) {
        eeBuffer->status |= EE_BUFFER_FULL;
    }
}

INT16 EE_Get(EEBuffer* eeBuffer) {
    INT16 tmp;
    eeBuffer->status &= (UINT8) ~EE_BUFFER_FULL;
    if (eeBuffer->status & EE_BUFFER_EMPTY) {
        return 0;
    }

    tmp = eeBuffer->input[eeBuffer->readIndex];
    eeBuffer->readIndex = (UINT8) ((eeBuffer->readIndex + 1) % eeBuffer->size);

    if (eeBuffer->writeIndex == eeBuffer->readIndex) {
        eeBuffer->status |= EE_BUFFER_EMPTY;
    }
    return tmp;
}

BOOL EE_IsFull(EEBuffer* eeBuffer) {
    return eeBuffer->status & EE_BUFFER_FULL;
}

BOOL EE_IsEmpty(EEBuffer* eeBuffer) {
    return eeBuffer->status & EE_BUFFER_EMPTY;
}

UINT32 EE_Calculate(EEBuffer* eeBuffer) {
    UINT32 ee = 0;
    UINT8 index;
    for (index = 0; index < eeBuffer->size; index++) {
        ee +=  (UINT32) (eeBuffer->tmp[index] * eeBuffer->tmp[index]) >> 8;
    }
#ifdef _DEBUG_
    printf("EE = %i \n", ee);
#endif
    return ee;
}

void EE_ReadIndexOffsetAdjust(EEBuffer* buffer) {
    buffer->readIndex = (UINT8) ((buffer->readIndex + 1) % buffer->size);
}

void EE_BufferExport(EEBuffer *buffer) {
    UINT8 counter = 0;
    while (EE_IsEmpty(buffer) != EE_BUFFER_EMPTY) {
        buffer->tmp[counter] = EE_Get(buffer);
        counter++;
    }
}