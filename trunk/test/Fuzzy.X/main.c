#include <p18cxxx.h>

#include "fuzzy.h"
#include "rules.h"

#pragma config XINST = OFF          //Extended instruction set disabled

void main(void) {
    UINT8 risk = 0;
    initRules();
    setInputs();

    risk = RuleEngine_run();

    while (1);
}
