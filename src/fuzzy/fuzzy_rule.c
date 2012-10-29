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

/* MIN rule implication */
float_t RuleImplication_min(float_t fuzzyInputA, float_t fuzzyInputB) {
    return fuzzyInputA < fuzzyInputB ? fuzzyInputA : fuzzyInputB;
}


void RuleImplication(RuleTerm* antecedent, RuleTerm* consecuent) {
    consecuent->fuzzy = RuleImplication_min(consecuent->fuzzy, antecedent->fuzzy);
}

/*..........................................................................*/

/* Evaluate term on input value */
void RuleTerm_evaluate(RuleTerm* antecedent) {
    antecedent->fuzzy = triangularFuzzify(
            antecedent->input,
            antecedent->membershipFunction);
}


/*..........................................................................*/

/* Evaluates rule */
void Rule_evaluate(Rule* rule) {
    UINT8 i;
    // Fuzzy crisp inputs
    for (i = 0; i < MAX_ANTECEDENTS; i++) {
        if (rule->antecedents[i] != NULL) {
            RuleTerm_evaluate(rule->antecedents[i]);
            // Apply min{u1, u2} implication rule
            RuleImplication(rule->antecedents[i], rule->consecuent);
        }
    }
}
