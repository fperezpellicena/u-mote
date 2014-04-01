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

#ifndef fir_h
#define fir_h

#include "GenericTypeDefs.h"

/* The maximum length of the buffer size */
#define BHPF_SIZEMAX        36    
/* Number of FIR coeficients */
#define BHPF_FIRCOEFLEN     36

#define FIR_BUFFER_EMPTY    1
#define FIR_BUFFER_FULL     2

typedef struct {
    INT16 au16BufferIn[BHPF_SIZEMAX];
    INT16 au16Coefficients[BHPF_FIRCOEFLEN];
    INT16 au16BufferOut[BHPF_SIZEMAX];
    UINT8 u8WriteIndex;
    UINT8 u8ReadIndex;
    UINT8 u8BufferSize;
    UINT8 u8FullFlag;
    UINT8 u8WindowSize;
} FirBuffer;

void Fir_Create(FirBuffer* buffer, INT16* coefficients);

void Fir_Add(FirBuffer* buffer, UINT16 element);

UINT8 Fir_IsFull(FirBuffer* buffer);

UINT8 Fir_IsEmpty(FirBuffer* buffer);

INT16 Fir_Get(FirBuffer* buffer);

INT16 Fir_Calculate(FirBuffer* buffer);

void Fir_ReadIndexOffsetAdjust(FirBuffer* buffer);

void Fir_BufferExport(FirBuffer* buffer);

#endif  /* D_FakeFir_H */
