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

#ifndef fuzzy_lang_h 
#define fuzzy_lang_h

#include "fuzzy_mf.h"

#define MAX_PARTITIONS	7

/*..........................................................................*/
/* Linguistic variable class */
typedef struct LinguisticVariable LinguisticVariable;

struct LinguisticVariable {
    MembershipFunction functions[MAX_PARTITIONS];
    UINT8 size;
};

/*..........................................................................*/
/* Init partitions list */
void LinguisticVariable_init(LinguisticVariable* lv);

/*..........................................................................*/
/* Add membership partition */
void LinguisticVariable_addPartition(LinguisticVariable* lv, MembershipFunction* mf);

#endif /*fuzzy_lang_h*/