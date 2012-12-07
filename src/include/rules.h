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

#ifndef rules_h
#define rules_h

#include "fuzzy.h"
#include "bsp.h"

void Fuzzy_initRules(void);

#ifdef SHT_ENABLED
#include "sht.h"
void Fuzzy_initSht(Sht* sht);
#endif

#ifdef IRCA1_ENABLED
#include "irca1.h"
void Fuzzy_initIrca(IrcA1* irca);
#endif

#endif /* rules_h */

