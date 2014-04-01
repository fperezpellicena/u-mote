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

#include "fir.h"
#include "bsp.h"
#include "math.h"
#include <string.h>
#ifdef _DEBUG_
    #include <stdio.h>
#endif

void Fir_Create(FirBuffer* buffer, INT16* coefficients) {
    UINT8 i;
    buffer->u8BufferSize = BHPF_SIZEMAX;
    buffer->u8ReadIndex = 0;
    buffer->u8WriteIndex = 0;
    buffer->u8FullFlag = FIR_BUFFER_EMPTY;
    memset(buffer->au16BufferIn, 0, BHPF_SIZEMAX);
    memset(buffer->au16BufferOut, 0, BHPF_SIZEMAX);
    memset(buffer->au16Coefficients, 0, BHPF_FIRCOEFLEN);
#ifdef _DEBUG_
    printf("****** Fir_Create ******\n");
    printf("buffer->u8BufferSize = %i \n", buffer->u8BufferSize);
    printf("buffer->u8ReadIndex = %i \n", buffer->u8ReadIndex);
    printf("buffer->u8WriteIndex = %i \n", buffer->u8WriteIndex);
    printf("buffer->u8FullFlag = %i \n", buffer->u8FullFlag);
#endif
    /** Copy coefficients to RAM in reverse order */
    for (i = 0; i < BHPF_FIRCOEFLEN; i++) {
        buffer->au16Coefficients[BHPF_FIRCOEFLEN - 1 - i] = coefficients[i];
#ifdef _DEBUG_
        printf("Set coefficient %u in position %u \n",
                buffer->au16Coefficients[BHPF_FIRCOEFLEN - 1 - i],
                (BHPF_FIRCOEFLEN - 1 - i));
#endif
    }
#ifdef _DEBUG_
    printf("***********************\n");
#endif
}

void Fir_Add(FirBuffer* buffer, UINT16 element){
#ifdef _DEBUG_
    printf("******** Fir_Add ********\n");
    printf("Element = %i \n", element);
    printf("buffer->u8FullFlag = %i \n", buffer->u8FullFlag);
#endif
   buffer->u8FullFlag &= (UINT8) ~FIR_BUFFER_EMPTY;
#ifdef _DEBUG_
   printf("buffer->u8FullFlag = %i \n", buffer->u8FullFlag);
#endif
    if (Fir_IsFull(buffer)) {
#ifdef _DEBUG_
    printf("Fir buffer is full! Returning... \n");
#endif
        return;
    }
    buffer->au16BufferIn[buffer->u8WriteIndex] = element;
#ifdef _DEBUG_
    printf("buffer->au16BufferIn[%i] = %i \n",
            buffer->u8WriteIndex, buffer->au16BufferIn[buffer->u8WriteIndex]);
#endif
    buffer->u8WriteIndex = (UINT8) ((buffer->u8WriteIndex + 1) % buffer->u8BufferSize);
    if (buffer->u8WriteIndex == buffer->u8ReadIndex) {
        buffer->u8FullFlag |= FIR_BUFFER_FULL;
#ifdef _DEBUG_
        printf("Fir buffer has been filled right now! Setting flag \n");
        printf("buffer->u8FullFlag = %i \n", buffer->u8FullFlag);
#endif
    }
}

INT16 Fir_Get(FirBuffer* buffer) {
    INT16 tmp;
    buffer->u8FullFlag &= (UINT8) ~FIR_BUFFER_FULL;
    if (buffer->u8FullFlag & FIR_BUFFER_EMPTY) {
        return 0;
    }

    tmp = buffer->au16BufferIn[buffer->u8ReadIndex];
    buffer->u8ReadIndex = (UINT8) ((buffer->u8ReadIndex + 1) % buffer->u8BufferSize);

    if (buffer->u8WriteIndex == buffer->u8ReadIndex) {
        buffer->u8FullFlag |= FIR_BUFFER_EMPTY;
    }
    return tmp;
}

UINT8 Fir_IsFull(FirBuffer *buffer) {
#ifdef _DEBUG_
    printf("Fir is full? %i \n", buffer->u8FullFlag & FIR_BUFFER_FULL);
#endif
    return buffer->u8FullFlag & FIR_BUFFER_FULL;
}

UINT8 Fir_IsEmpty(FirBuffer *buffer) {
#ifdef _DEBUG_
    printf("Fir is empty? %i \n", buffer->u8FullFlag & FIR_BUFFER_EMPTY);
#endif
    return buffer->u8FullFlag & FIR_BUFFER_EMPTY;
}

void Fir_ReadIndexOffsetAdjust(FirBuffer *buffer) {
    buffer->u8ReadIndex = (UINT8) ((buffer->u8ReadIndex + 1) % buffer->u8BufferSize);
}

void Fir_BufferExport(FirBuffer *buffer) {
    UINT8 counter = 0;
#ifdef _DEBUG_
    printf("**** Fir_BufferExport ****\n");
#endif
    while (Fir_IsEmpty(buffer) != FIR_BUFFER_EMPTY) {
        buffer->au16BufferOut[counter] = Fir_Get(buffer);
#ifdef _DEBUG_
        printf("buffer->au16BufferOut[%i] = %u \n", counter, buffer->au16BufferOut[counter]);
#endif
        counter++;
    }
#ifdef _DEBUG_
    printf("***********************\n");
#endif
}

INT16 Fir_Calculate(FirBuffer* buffer) {
    INT32 mac = 0;
    INT16 macRounded = 0;
    INT16* pSignal = buffer->au16BufferOut;
    INT16* pCoeffs = buffer->au16Coefficients;
    UINT8 i;
#ifdef _DEBUG_
    printf("**** Fir_Calculate ****\n");
#endif
    for (i = 0; i < BHPF_FIRCOEFLEN; i++) {
#ifdef _DEBUG_
        printf("Signal[%u] = %u , Coeff[%u] = %i \n", i, *pSignal, i,  *pCoeffs);
#endif
        mac += (INT32)(*pSignal++) * (INT32)(*pCoeffs++);
    }
    macRounded = (INT16) (mac >> 15);
#ifdef _DEBUG_
    printf("Fir result = %i\n", macRounded);
    printf("***********************\n");
#endif
    return macRounded;
}