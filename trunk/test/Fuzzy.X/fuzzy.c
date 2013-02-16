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
#include "fuzzy_rule.h"
#include <math.h>

DECLARE_ENGINE(engine);

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

void RuleEngine_addRule(Rule* rule) {
    if (engine.size < MAX_RULES) {
	RuleEngine_add(rule, &engine.rules[engine.size]);
	++engine.size;
    }
}

void RuleEngine_defuzzyfyRule(Rule* rule);

void RuleEngine_defuzzyfyRule(Rule* rule) {
    den += rule->consecuent->fuzzy;
    num += rule->consecuent->fuzzy * rule->consecuent->function.mid;
}

/*..........................................................................*/

/* Run engine */
UINT8 RuleEngine_run(void) {
    UINT8 i;
    UINT8 j;
    UINT8 antecedentsSize;
    UINT8 input;
    MembershipFunction* mf;
    UINT8 fuzzyInput;
    UINT8 fuzzyOutput;
    float_t fout;
    UINT8 iout;
    num = 0;
    den = 1;
    for (i = 0; i < engine.size; i++) {
	antecedentsSize = engine.rules[i].antecedentsSize;
	fuzzyOutput = UCHAR_MAX;
	// Evaluate i rule
	for (j = 0; j < antecedentsSize; j++) {
	    input = engine.rules[i].antecedents[j]->input;
	    mf = &engine.rules[i].antecedents[j]->function;
	    // Evaluate input contained in ruleterm input
	    fuzzyInput = RuleTerm_evaluate(input, mf);
	    // Apply min{u1, u2} implication rule
	    fuzzyOutput = RuleImplication_min(fuzzyOutput, fuzzyInput);
	}
	// Apply COG
	den += fuzzyOutput;
	mf = &engine.rules[i].consecuent->function;
	num += fuzzyOutput * mf->mid;
    }
    fout = (float_t)((float_t)num / (float_t)den);
    iout = (UINT8)((float_t)(fout * 100));
    return iout;
}
