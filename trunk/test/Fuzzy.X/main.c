#include <p18cxxx.h>

#include "fuzzy.h"
#include "fuzzy_mf.h"

#pragma config XINST = OFF          //Extended instruction set disabled

#pragma udata mf
DECLARE_MF(lowTemp, -40, 0, 20);
DECLARE_MF(midTemp, 0, 20, 40);
DECLARE_MF(highTemp, 30, 40, 125);

DECLARE_MF(lowCo2, 0, 0, 50);
DECLARE_MF(midCo2, 0, 50, 100);
DECLARE_MF(highCo2, 50, 100, 100);

DECLARE_MF(lowRisk, 0, 0, 50);
DECLARE_MF(midRisk, 0, 50, 100);
DECLARE_MF(highRisk, 50, 100, 100);
#pragma udata

#pragma udata rp
DECLARE_RT(ifHighTemp, &highTemp);
DECLARE_RT(andHighCo2, &highCo2);
DECLARE_RT(thenHighRisk, &highRisk);

DECLARE_RT(andlowCo2, &lowCo2);
DECLARE_RT(thenMidRisk, &midRisk);
#pragma udata

#pragma udata rule
DECLARE_RULE(ifHighTempAndHighCo2ThenHighRisk, &thenHighRisk, 2, &ifHighTemp, &andHighCo2);
DECLARE_RULE(ifHighTempAndlowCo2ThenMidRisk, &thenMidRisk, 2, &ifHighTemp, &andlowCo2);
#pragma udata

#pragma udata ruleEngine
DECLARE_ENGINE(engine, 2, &ifHighTempAndHighCo2ThenHighRisk, &ifHighTempAndlowCo2ThenMidRisk);
#pragma udata

void main(void) {
    UINT8 risk = 0;
    // Set crisp inputs
    ifHighTemp.input = 50;
    andHighCo2.input = 60;
    andlowCo2.input = 60;

    // Print output engine
    risk = RuleEngine_run(&engine);
    
    while(1);
}
