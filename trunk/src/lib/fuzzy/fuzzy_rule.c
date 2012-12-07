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

#include "fuzzy_rule.h"
#include "fuzzy_mf.h"
#include <stdlib.h>

/*..........................................................................*/
void Rule_addAntedecents(Rule* rule, RuleTerm* antecedents[]) {
    UINT8 i;
    for(i = 0; i < MAX_ANTECEDENTS; i++) {
       Rule_addAntedecent(rule, antecedents[i]);
    }
}

/*..........................................................................*/
void Rule_addAntedecent(Rule* rule, RuleTerm* antecedent) {
    if(rule->antecedentsSize < MAX_ANTECEDENTS) {
	rule->antecedents[rule->antecedentsSize++] = antecedent;
    }
}

/*..........................................................................*/
void Rule_setConsecuent(Rule* rule, RuleTerm* consecuent) {
    rule->consecuent = consecuent;
}

/*..........................................................................*/

/* MIN rule implication */
UINT8 RuleImplication_min(UINT8 fuzzyInputA, UINT8 fuzzyInputB) {
    return fuzzyInputA < fuzzyInputB ? fuzzyInputA : fuzzyInputB;
}

/*..........................................................................*/

/* Evaluate term on input value */
UINT8 RuleTerm_evaluate(UINT8 input, MembershipFunction* mf) {
    return triangularFuzzify(input, mf);
}
