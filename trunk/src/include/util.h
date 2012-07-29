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

#ifndef util_h
#define util_h

#include "GenericTypeDefs.h"
#include <limits.h>

#define round(x) ((x) < CHAR_MIN - 0.5 || (x) > CHAR_MAX + 0.5 ?\
            (UINT8)x : ((x) >= 0 ? (UINT8)((x) + 0.5) : (UINT8)((x) - 0.5)))

void Util_str2ram(UINT8 rom* src, UINT8* dest);

void Util_strCopy(UINT8* src, UINT8* dest, UINT8 length);

#endif /* util_h*/
