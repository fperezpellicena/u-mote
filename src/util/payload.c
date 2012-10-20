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

#include "payload.h"

/*..........................................................................*/

/* Init list, set size equals zero */
void Payload_init(Payload* payload) {
    payload->size = 0;
}

/*..........................................................................*/

/* Add one byte to the list */
void Payload_addByte(Payload* payload, UINT8 element) {
    if (payload->size < MAX_PAYLOAD - 1) {
        payload->data[payload->size++] = element;
    }
}

/*..........................................................................*/

/* Add one word to the list(big endian) */
void Payload_addWord(Payload* payload, UINT16 element) {
    UINT8 lsb = (UINT8)element;
    UINT8 msb = (UINT8)element >> 8;
    if (payload->size < MAX_PAYLOAD - 2) {
        payload->data[payload->size++] = msb;
        payload->data[payload->size++] = lsb;
    }
}

/*..........................................................................*/

/* Delete all elements of the list */
void Payload_empty(Payload* payload) {
    UINT8 i = MAX_PAYLOAD - 1;
    while (i--) {
        payload->data[i] = NULL;
    }
}

/*..........................................................................*/

/* Append all elements to the list */
void Payload_append(Payload* to, Payload* from) {
    UINT8 i = 0;
    while (i < from->size) {
        Payload_addByte(to, from->data[i++]);
    }
}
