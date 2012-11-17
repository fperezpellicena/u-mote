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

#if SENSING_MODE == FUZZY_DRIVEN

#    include "fuzzy_rule.h"
#    include "rules.h"

DECLARE_IF(HighTemp,{30, 50, 255});
DECLARE_IF(MidTemp,{0, 30, 50});
DECLARE_IF(LowTemp,{0, 0, 40});
DECLARE_IF(HighCo2,{50, 100, 100});
DECLARE_IF(MidCo2,{0, 50, 100});
DECLARE_IF(LowCo2,{0, 0, 50});
DECLARE_THEN(HighRisk,{50, 100, 100});
DECLARE_THEN(MidRisk,{0, 50, 100});
DECLARE_THEN(LowRisk,{0, 0, 50});

#    pragma idata rule_b1
DECLARE_RULE(ifHighTempAndLowCo2ThenMidRisk);
DECLARE_RULE(ifHighTempAndMidCo2ThenHighRisk);
DECLARE_RULE(ifHighTempAndHighCo2ThenHighRisk);
DECLARE_RULE(ifMidTempAndLowCo2ThenLowRisk);
DECLARE_RULE(ifMidTempAndMidCo2ThenMidRisk);
DECLARE_RULE(ifMidTempAndHighCo2ThenMidRisk);
DECLARE_RULE(ifLowTempAndLowCo2ThenLowRisk);
DECLARE_RULE(ifLowTempAndMidCo2ThenLowRisk);
DECLARE_RULE(ifLowTempAndHighCo2ThenMidRisk);
#    pragma idata

static void Fuzzy_initRule(RuleEngine* engine, Rule* rule, RuleTerm* antecedent1,
	RuleTerm* antecedent2, RuleTerm* consecuent);

static void Fuzzy_initRule(RuleEngine* engine, Rule* rule, RuleTerm* antecedent1,
	RuleTerm* antecedent2, RuleTerm* consecuent) {
    Rule_addAntedecent(rule, antecedent1);
    Rule_addAntedecent(rule, antecedent2);
    Rule_setConsecuent(rule, consecuent);
    RuleEngine_addRule(engine, rule);
}

void Fuzzy_initRules(RuleEngine* engine) {
    Fuzzy_initRule(engine, &ifHighTempAndLowCo2ThenMidRisk, &ifHighTemp,
	    &ifLowCo2, &thenMidRisk);
    Fuzzy_initRule(engine, &ifHighTempAndMidCo2ThenHighRisk, &ifHighTemp,
	    &ifMidCo2, &thenHighRisk);
    Fuzzy_initRule(engine, &ifHighTempAndHighCo2ThenHighRisk, &ifHighTemp,
	    &ifHighCo2, &thenHighRisk);
    Fuzzy_initRule(engine, &ifMidTempAndLowCo2ThenLowRisk, &ifMidTemp,
	    &ifLowCo2, &thenLowRisk);
    Fuzzy_initRule(engine, &ifMidTempAndMidCo2ThenMidRisk, &ifMidTemp,
	    &ifMidCo2, &thenMidRisk);
    Fuzzy_initRule(engine, &ifMidTempAndHighCo2ThenMidRisk, &ifMidTemp,
	    &ifHighCo2, &thenMidRisk);
    Fuzzy_initRule(engine, &ifLowTempAndLowCo2ThenLowRisk, &ifLowTemp,
	    &ifLowCo2, &thenLowRisk);
    Fuzzy_initRule(engine, &ifLowTempAndMidCo2ThenLowRisk, &ifLowTemp,
	    &ifMidCo2, &thenLowRisk);
    Fuzzy_initRule(engine, &ifLowTempAndHighCo2ThenMidRisk, &ifLowTemp,
	    &ifHighCo2, &thenMidRisk);
}

#    if SHT_ENABLED

void Fuzzy_initSht(Sht* sht) {
    Sht11_addTempTerm(sht, &ifLowTemp);
    Sht11_addTempTerm(sht, &ifMidTemp);
    Sht11_addTempTerm(sht, &ifHighTemp);
}
#    endif

#    if IRCA1_ENABLED

void Fuzzy_initIrca(IrcA1* irca) {

}
#    endif

#endif
