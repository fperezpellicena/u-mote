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

#include "list.h"

/*..........................................................................*/
/* Init list, set size equals zero */
void List_init(List* list) {
    list->size = 0;
    List_empty(list);
}

/*..........................................................................*/
/* Add one elemento to the list */
void List_add(List* list, UINT8 element) {
    if (list->size < MAX_LIST_SIZE - 1) {
        list->data[list->size++] = element;
    }
}

/*..........................................................................*/
/* Delete all elements of the list */
void List_empty(List* list) {
    UINT8 i = MAX_LIST_SIZE - 1;
    while (i--) {
        list->data[i] = NULL;
    }
}

/*..........................................................................*/
/* Append all elements to the list */
void List_append(List* list, List* elements) {
    UINT8 i = 0;
    while(i < elements->size) {
        List_add(list, elements->data[i++]);
    }
}
