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

#include "bsp_ambiental.h"

#if SENSING_MODE == FUZZY_DRIVEN

#    include "fuzzy_rule.h"
#    include "rules.h"


#include "fuzzy_rule.h"
#include "rules.h"

IF(HighTemp,{30, 50, 255});
IF(MidTemp,{0, 30, 50});
IF(LowTemp,{0, 0, 40});

IF(HighCo2,{50, 100, 100});
IF(MidCo2,{0, 50, 100});
IF(LowCo2,{0, 0, 50});

THEN(HighRisk,{50, 100, 100});
THEN(MidRisk,{0, 50, 100});
THEN(LowRisk,{0, 0, 50});

RULE(ifHighTempAndLowCo2ThenMidRisk);
RULE(ifHighTempAndMidCo2ThenHighRisk);
RULE(ifHighTempAndHighCo2ThenHighRisk);
RULE(ifMidTempAndLowCo2ThenLowRisk);
RULE(ifMidTempAndMidCo2ThenMidRisk);
RULE(ifMidTempAndHighCo2ThenMidRisk);
RULE(ifLowTempAndLowCo2ThenLowRisk);
RULE(ifLowTempAndMidCo2ThenLowRisk);
RULE(ifLowTempAndHighCo2ThenMidRisk);

static void Fuzzy_initRule(Rule* rule, RuleTerm* antecedent1,
	RuleTerm* antecedent2, RuleTerm* consecuent);

static void Fuzzy_initRule(Rule* rule, RuleTerm* antecedent1,
	RuleTerm* antecedent2, RuleTerm* consecuent) {
    Rule_addAntedecent(rule, antecedent1);
    Rule_addAntedecent(rule, antecedent2);
    Rule_setConsecuent(rule, consecuent);
    RuleEngine_addRule(rule);
}

void Fuzzy_initRules(void) {
    Fuzzy_initRule(&ifHighTempAndLowCo2ThenMidRisk, &ifHighTemp, &ifLowCo2, &thenMidRisk);
    Fuzzy_initRule(&ifHighTempAndMidCo2ThenHighRisk, &ifHighTemp, &ifMidCo2, &thenHighRisk);
    Fuzzy_initRule(&ifHighTempAndHighCo2ThenHighRisk, &ifHighTemp, &ifHighCo2, &thenHighRisk);
    Fuzzy_initRule(&ifMidTempAndLowCo2ThenLowRisk, &ifMidTemp, &ifLowCo2, &thenLowRisk);
    Fuzzy_initRule(&ifMidTempAndMidCo2ThenMidRisk, &ifMidTemp, &ifMidCo2, &thenMidRisk);
    Fuzzy_initRule(&ifMidTempAndHighCo2ThenMidRisk, &ifMidTemp, &ifHighCo2, &thenMidRisk);
    Fuzzy_initRule(&ifLowTempAndLowCo2ThenLowRisk, &ifLowTemp, &ifLowCo2, &thenLowRisk);
    Fuzzy_initRule(&ifLowTempAndMidCo2ThenLowRisk, &ifLowTemp, &ifMidCo2, &thenLowRisk);
    Fuzzy_initRule(&ifLowTempAndHighCo2ThenMidRisk, &ifLowTemp, &ifHighCo2, &thenMidRisk);
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
    IrcA1_addCO2Term(irca, &ifLowCo2);
    IrcA1_addCO2Term(irca, &ifMidCo2);
    IrcA1_addCO2Term(irca, &ifHighCo2);
}
#    endif

#endif
