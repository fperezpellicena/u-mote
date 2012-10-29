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

#if SENSING_MODE == FUZZY_DRIVEN
#pragma udata tmp_mf
IF(HighTemp, 30, 40, 255);
IF(MidTemp, 0, 20, 40);
IF(LowTemp, -40, 0, 20);
#pragma udata

#pragma udata co2_mf
IF(HighCo2, 50, 100, 255);
IF(MidCo2, 0, 50, 100);
IF(LowCo2,  0, 0, 50);
#pragma udata

#pragma udata risk_mf
THEN(HighRisk, 50, 100, 255);
THEN(MidRisk, 0, 50, 100);
THEN(LowRisk,  0, 0, 50);
#pragma udata

#pragma udata rules
RULE(ifHighTempThenHighRisk, &thenHighRisk, &ifHighTemp);
RULE(ifMidTempThenMidRisk, &thenMidRisk, &ifMidTemp);
RULE(ifLowTempThenLowRisk, &thenLowRisk, 1, &ifLowTemp);
//RULE(ifHighTempAndHighCo2ThenHighRisk, &thenHighRisk, 2, &ifHighTemp, &andHighCo2);
//RULE(ifHighTempAndlowCo2ThenMidRisk, &thenMidRisk, 2, &ifHighTemp, &andLowCo2);
#pragma udata

#pragma udata ruleEngine
ENGINE(engine, &ifHighTempThenHighRisk,
        &ifMidTempThenMidRisk, &ifLowTempThenLowRisk);
//ENGINE(engine, 2, &ifHighTempAndHighCo2ThenHighRisk, &ifHighTempAndlowCo2ThenMidRisk);
#pragma udata

#endif
