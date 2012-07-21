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

#ifndef list_h
#define list_h

#include "bsp.h"

/* Static List struct */
typedef struct List List;

struct List {
    UINT8 data[MAX_LIST_SIZE];
    UINT8 size;
};

/* Init list */
void List_init(List* list);

/* Add one element to the list */
void List_add(List* list, UINT8 element);

/* Delete all elements of the list */
void List_empty(List* list);

/* Append */
void List_append(List* list, List* elements);

#endif /* list_H*/
