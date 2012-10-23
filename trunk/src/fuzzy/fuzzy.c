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

#include "fuzzy.h"
#include <math.h>


/*..........................................................................*/

/* Run engine */
UINT8 RuleEngine_run(RuleEngine* engine) {
    UINT8 i;
    RuleTerm* ruleTerm;
    float_t num = (float_t) 0;
    float_t den = (float_t) 0;
    // Evaluate all rules
    for (i = 0; i < engine->size; i++) {
        // Evaluate i rule
        Rule_evaluate(engine->rules[i]);
        ruleTerm = engine->rules[i]->consecuent;
        // Apply COG
        den += ruleTerm->fuzzy;
        num += ruleTerm->fuzzy * (float_t)ruleTerm->membershipFunction->mid;
    }
    return (UINT8)(num / den);
}
