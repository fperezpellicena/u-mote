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

static UINT8 num;
static UINT8 den;

static void RuleEngine_add(Rule* fromRule, Rule* toRule);

static void RuleEngine_add(Rule* fromRule, Rule* toRule) {
    UINT8 i;
    // Add antecedents
    for (i = 0; i < fromRule->antecedentsSize; i++) {
	Rule_addAntedecent(toRule, fromRule->antecedents[i]);
    }
    // Add consecuent
    Rule_setConsecuent(toRule, fromRule->consecuent);
}

void RuleEngine_addRule(RuleEngine* engine, Rule* rule) {
    if (engine->size < MAX_RULES) {
	RuleEngine_add(rule, &engine->rules[engine->size]);
	++engine->size;
    }
}

static void RuleEngine_defuzzyfyRule(Rule* rule);

static void RuleEngine_defuzzyfyRule(Rule* rule) {
    den += rule->consecuent->fuzzy;
    num += rule->consecuent->fuzzy * rule->consecuent->membershipFunction.mid;
}

/*..........................................................................*/

/* Run engine */
UINT8 RuleEngine_run(RuleEngine* engine) {
    UINT8 i;
    num = 0;
    den = 1;
    for (i = 0; i < engine->size; i++) {
	// Evaluate i rule
	Rule_evaluate(&engine->rules[i]);
	// Apply COG
	RuleEngine_defuzzyfyRule(&engine->rules[i]);
    }
    return num / den;
}
