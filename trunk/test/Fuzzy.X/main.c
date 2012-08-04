#include <p18cxxx.h>

#include "fuzzy.h"
#include "fuzzy_mf.h"

#pragma config XINST = OFF          //Extended instruction set disabled

#pragma udata mf
MembershipFunction highTemp = {30, 40, 125};
MembershipFunction highCo2 = {50, 100, 100};
MembershipFunction highRisk = {50, 100, 100};
#pragma udata

#pragma udata rp
RuleTerm antecedent1 = {&highTemp, 0, 0};
RuleTerm antecedent2 = {&highCo2, 0, 0};
RuleTerm consecuent = {&highRisk, 0, 1};
#pragma udata

#pragma udata rule
RuleTerm* antecedents[MAX_ANTECEDENTS] = {&antecedent1, &antecedent2};
Rule rule = {antecedents, &consecuent, 2};
#pragma udata

#pragma udata ruleEngine
Rule* rules[1] = {&rule};
RuleEngine engine = {rules, 1};
#pragma udata

Rule* r;
RuleTerm* rt;
//MembershipFunction* mf;
UINT8 i;
UINT8 j;
UINT8 value;
UINT8 risk;

void main(void) {
    // Set crisp inputs
    antecedent1.input = 50;
    antecedent2.input = 60;

//    // Test
//    for (i = 0; i < 1; i++) {
//        r = engine.rules[i];
//        for (j = 0; j < r->antecedentsSize; j++) {
//            rt = rule.antecedents[j];
//            value = rt->membershipFunction->left;
//            value = rt->membershipFunction->mid;
//            value = rt->membershipFunction->right;
//            Rule_evaluate(r);
//        }
//    }

  
    // Print output engine
    risk = RuleEngine_run(&engine);
    while(1);
}
