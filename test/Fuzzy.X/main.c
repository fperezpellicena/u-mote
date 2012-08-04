#include <p18cxxx.h>

#include "fuzzy.h"
#include "fuzzy_mf.h"

#pragma config XINST = OFF          //Extended instruction set disabled

#pragma udata mf
MembershipFunction lowTemp = {-40, 0, 20};
MembershipFunction midTemp = {0, 20, 40};
MembershipFunction highTemp = {30, 40, 125};

MembershipFunction lowCo2 = {0, 0, 50};
MembershipFunction midCo2 = {0, 50, 100};
MembershipFunction highCo2 = {50, 100, 100};

MembershipFunction lowRisk = {0, 0, 50};
MembershipFunction midRisk = {0, 50, 100};
MembershipFunction highRisk = {50, 100, 100};
#pragma udata

#pragma udata rp
RuleTerm ifHighTemp = {&highTemp, 0, 0};
RuleTerm andHighCo2 = {&highCo2, 0, 0};
RuleTerm thenHighRisk = {&highRisk, 0, 1};

RuleTerm andlowCo2 = {&lowCo2, 0, 0};
RuleTerm thenMidRisk = {&midRisk, 0, 1};
#pragma udata

#pragma udata rule
RuleTerm* antecedents1[MAX_ANTECEDENTS] = {&ifHighTemp, &andHighCo2};
Rule ifHighTempAndHighCo2ThenHighRisk = {antecedents1, &thenHighRisk, 2};

RuleTerm* antecedents2[MAX_ANTECEDENTS] = {&ifHighTemp, &andlowCo2};
Rule ifHighTempAndlowCo2ThenMidRisk = {antecedents2, &thenMidRisk, 2};
#pragma udata

#pragma udata ruleEngine
Rule* rules[2] = {&ifHighTempAndHighCo2ThenHighRisk, &ifHighTempAndlowCo2ThenMidRisk};
RuleEngine engine = {rules, 2};
#pragma udata




void main(void) {
    UINT8 risk;
    // Set crisp inputs
    ifHighTemp.input = 50;
    andHighCo2.input = 60;
    andlowCo2.input = 60;
  
    // Print output engine
    risk = RuleEngine_run(&engine);
    if(risk == 100) {
        risk = 0;
    } else {
        risk = 50;
    }
    while(1);
}
