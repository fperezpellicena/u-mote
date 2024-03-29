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
#include "rules.h"

extern RuleEngine engine;

DECLARE_IF(HighTemp,{30, 50, 255});
DECLARE_IF(MidTemp,{0, 30, 50});
DECLARE_IF(LowTemp,{0, 0, 40});

DECLARE_IF(HighCo2,{50, 100, 100});
DECLARE_IF(MidCo2,{0, 50, 100});
DECLARE_IF(LowCo2,{0, 0, 50});

DECLARE_THEN(HighRisk,{50, 100, 100});
DECLARE_THEN(MidRisk,{0, 50, 100});
DECLARE_THEN(LowRisk,{0, 0, 50});

DECLARE_RULE(ifHighTempAndLowCo2ThenMidRisk);
DECLARE_RULE(ifHighTempAndMidCo2ThenHighRisk);
DECLARE_RULE(ifHighTempAndHighCo2ThenHighRisk);
DECLARE_RULE(ifMidTempAndLowCo2ThenLowRisk);
DECLARE_RULE(ifMidTempAndMidCo2ThenMidRisk);
DECLARE_RULE(ifMidTempAndHighCo2ThenMidRisk);
DECLARE_RULE(ifLowTempAndLowCo2ThenLowRisk);
DECLARE_RULE(ifLowTempAndMidCo2ThenLowRisk);
DECLARE_RULE(ifLowTempAndHighCo2ThenMidRisk);


void initRule(Rule* rule, RuleTerm* antecedent1, RuleTerm* antecedent2,
	RuleTerm* consecuent) {
    Rule_addAntedecent(rule, antecedent1);
    Rule_addAntedecent(rule, antecedent2);
    Rule_setConsecuent(rule, consecuent);
    RuleEngine_addRule(rule);
}

void initRules(void) {
    initRule(&ifHighTempAndLowCo2ThenMidRisk, &ifHighTemp, &ifLowCo2, &thenMidRisk);
    initRule(&ifHighTempAndMidCo2ThenHighRisk, &ifHighTemp, &ifMidCo2, &thenHighRisk);
    initRule(&ifHighTempAndHighCo2ThenHighRisk, &ifHighTemp, &ifHighCo2, &thenHighRisk);
    initRule(&ifMidTempAndLowCo2ThenLowRisk, &ifMidTemp, &ifLowCo2, &thenLowRisk);
    initRule(&ifMidTempAndMidCo2ThenMidRisk, &ifMidTemp, &ifMidCo2, &thenMidRisk);
    initRule(&ifMidTempAndHighCo2ThenMidRisk, &ifMidTemp, &ifHighCo2, &thenMidRisk);
    initRule(&ifLowTempAndLowCo2ThenLowRisk, &ifLowTemp, &ifLowCo2, &thenLowRisk);
    initRule(&ifLowTempAndMidCo2ThenLowRisk, &ifLowTemp, &ifMidCo2, &thenLowRisk);
    initRule(&ifLowTempAndHighCo2ThenMidRisk, &ifLowTemp, &ifHighCo2, &thenMidRisk);
}


void setInputs(void) {
    ifHighTemp.input = 40;
    ifMidTemp.input = 40;
    ifLowTemp.input = 40;

    ifHighCo2.input = 75;
    ifMidCo2.input = 75;
    ifLowCo2.input = 75;
}
