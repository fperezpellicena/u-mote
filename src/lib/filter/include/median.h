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

#ifndef median_h
#define median_h

#include "GenericTypeDefs.h"

#define MEDIAN_BUFFER_SIZE		13	// Window size
#define MEDIAN_INDEX			6

#define MEDIAN_BUFFER_EMPTY		1
#define MEDIAN_BUFFER_FULL		2

/** Median ring buffer struct */
typedef struct {
	UINT16 contents[MEDIAN_BUFFER_SIZE];
	UINT16 exportContents[MEDIAN_BUFFER_SIZE];
	UINT8 readIndex;
	UINT8 writeIndex;
	UINT8 size;			// Buffer length, not the current size
	UINT8 status;
} MedianBuffer;

void Median_Create(MedianBuffer* medianBuffer);

void Median_Add(MedianBuffer* medianBuffer, UINT16 element);

UINT16 Median_Get(MedianBuffer* medianBuffer);

BOOL Median_IsFull(MedianBuffer* medianBuffer);

BOOL Median_IsEmpty(MedianBuffer* medianBuffer);

void Median_OnBufferFull(MedianBuffer* medianBuffer);

UINT16 Median_Calculate(MedianBuffer* medianBuffer);

void Median_ReadIndexOffsetAdjust(MedianBuffer* buffer);

void Median_BufferExport(MedianBuffer *buffer);

#endif  /* D_FakeMedian_H */
