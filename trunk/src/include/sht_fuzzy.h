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

#include "bsp.h"
#ifdef SHT_ENABLED
#ifndef sht_fuzzy_h
#define sht_fuzzy_h

#include "list.h"
#include "fuzzy.h"
#include "sht.h"

/* Declare fuzzy sht sensor */
#define DECLARE_FUZZY_SHT(id, name, senseFn, termsSize, ...) \
    DECLARE_FUZZY_SENSOR(id, name##id, senseFn, termsSize, __VA_ARGS__);\
    ShtData name##data;\
    Sht name = {&name##id, &name##data}


void ShtFuzzy_init(void);

#endif /*sht_fuzzy_h*/
#endif
