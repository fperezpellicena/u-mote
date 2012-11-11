#include <p18cxxx.h>

#include "fuzzy.h"
#include "rules.h"

#pragma config XINST = OFF          //Extended instruction set disabled

#pragma idata ruleEngine
DECLARE_ENGINE(engine);
#pragma idata

#pragma code
void main(void) {
    UINT8 risk = 0;
    initRules(&engine);
    setInputs();
    // Print output engine
    risk = RuleEngine_run(&engine);

    while (1);
}
