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

#ifndef payload_h
#define payload_h

#include "bsp.h"

/* Static List struct */
typedef struct Payload Payload;

struct Payload {
    UINT8 data[MAX_PAYLOAD];
    UINT8 size;
};

/* Init list */
void Payload_init(Payload* list);

/* Add one element to the list */
void Payload_add(Payload* list, UINT8 element);

/* Delete all elements of the list */
void Payload_empty(Payload* list);

/* Append */
void Payload_append(Payload* to, Payload* from);

#endif /* list_H*/
