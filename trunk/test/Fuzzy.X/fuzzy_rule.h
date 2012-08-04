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

#ifndef fuzzy_rule_h
#define fuzzy_rule_h

#include "fuzzy_mf.h"

#define MAX_ANTECEDENTS     0x02

#define DECLARE_RT(name, mf)\
RuleTerm name = {mf, 0, 1}

#define DECLARE_RULE(name, consecuent, size, ...)\
RuleTerm* name##ruleTerms[size] = {__VA_ARGS__};\
Rule name = {name##ruleTerms, consecuent, size}

/*****************************************************************************
 *  Let R1:
 *      If temperature is high and gas concentration is high then risk is high
 *
 * The rule has two antecedents:
 *  1.- temperature is high
 *  2.- gas concentration is high
 *
 * And one consecuent:
 *  1.- risk is high
 *
 * So an Antecedent is defined by the membership function related to the
 * linguistic variable "Temperature" that means "High temperature".
 * By this, an Antecedent is in practical terms, a membership function.
 *
 * A Consecuent is, in practical terms, the same as an Antecedent, so it can be
 * modeled as the same class.
 ****************************************************************************/

/*..........................................................................*/
/* Antecedent/Consecuent class */
typedef struct RuleTerm RuleTerm;

struct RuleTerm {
    MembershipFunction* membershipFunction;
    UINT8 input;
    float_t fuzzy;
};

/*..........................................................................*/
/* Evaluate term on input value */
float_t RuleTerm_evaluate(RuleTerm* ruleTerm, UINT8 input);


/*****************************************************************************
 *  Let R1:
 *      If temperature is high and gas concentration is high then risk is high
 *
 * The rule has two antecedents:
 *  1.- temperature is high
 *  2.- gas concentration is high
 *
 * And one consecuent:
 *  1.- risk is high
 *
 * So one rule is formed by one or more antecedents, mapping the inputs and
 * at least one consecuent, for MISO systems
 * There is no practical difference between Antecedent and Consecuent classes,
 * because they represent the input and out membership functions, respectively.
 ****************************************************************************/

/* Implication rule function prototype */
float_t RuleImplication(float_t fuzzyInputA, float_t fuzzyInputB);

/*..........................................................................*/
/** Rule class */
typedef struct Rule Rule;

struct Rule {
    RuleTerm** antecedents;
    RuleTerm* consecuent;
    UINT8 antecedentsSize;
};

/*..........................................................................*/
/* Evaluates rule */
void Rule_evaluate(Rule* rule);

#endif /* fuzzy_rule_h */