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

#ifndef ee_h
#define ee_h

#include "GenericTypeDefs.h"

#define EE_BUFFER_SIZE		36
//#define EE_MAXTHRESHOLD         0x00268000
#define EE_MAXTHRESHOLD         0x01

#define EE_BUFFER_EMPTY		1
#define EE_BUFFER_FULL		2

#define EE_FALL                 0xFD
#define EE_NO_FALL              0xEF

/** Median ring buffer struct */
typedef struct {
	INT16 input[EE_BUFFER_SIZE];
	INT16 tmp[EE_BUFFER_SIZE];
	UINT8 readIndex;
	UINT8 writeIndex;
	UINT8 writeIndexHistory;
	UINT8 size;			// Buffer length, not the current size
	UINT8 status;
} EEBuffer;

void EE_Create(EEBuffer* eeBuffer);

void EE_Add(EEBuffer* eeBuffer, INT16 element);

INT16 EE_Get(EEBuffer* eeBuffer);

BOOL EE_IsFull(EEBuffer* eeBuffer);

BOOL EE_IsEmpty(EEBuffer* eeBuffer);

UINT32 EE_Calculate(EEBuffer* eeBuffer);

void EE_ReadIndexOffsetAdjust(EEBuffer* buffer);

void EE_BufferExport(EEBuffer *buffer);

#endif 
