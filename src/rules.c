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

#if SENSING_MODE == FUZZY_DRIVEN
#pragma udata tmp_mf
DECLARE_MF(lowTemp, -40, 0, 20);
DECLARE_MF(midTemp, 0, 20, 40);
DECLARE_MF(highTemp, 30, 40, 255);
DECLARE_RT(ifHighTemp, &highTemp);
DECLARE_RT(ifMidTemp, &midTemp);
DECLARE_RT(ifLowTemp, &lowTemp);
#pragma udata

#pragma udata co2_mf
DECLARE_MF(lowCo2, 0, 0, 50);
DECLARE_MF(midCo2, 0, 50, 100);
DECLARE_MF(highCo2, 50, 100, 255);
DECLARE_RT(ifHighCo2, &highCo2);
DECLARE_RT(ifMidTCo2, &midCo2);
DECLARE_RT(ifLowCo2, &lowCo2);
#pragma udata

#pragma udata risk_mf
DECLARE_MF(lowRisk, 0, 0, 50);
DECLARE_MF(midRisk, 0, 50, 100);
DECLARE_MF(highRisk, 50, 100, 255);
DECLARE_RT(thenHighRisk, &highRisk);
DECLARE_RT(thenMidRisk, &midRisk);
DECLARE_RT(thenLowRisk, &lowRisk);
#pragma udata

#pragma udata rules
DECLARE_RULE(ifHighTempThenHighRisk, &thenHighRisk, 1, &ifHighTemp);
DECLARE_RULE(ifMidTempThenMidRisk, &thenMidRisk, 1, &ifLowTemp);
DECLARE_RULE(ifLowTempThenLowRisk, &thenLowRisk, 1, &ifMidTemp);
//DECLARE_RULE(ifHighTempAndHighCo2ThenHighRisk, &thenHighRisk, 2, &ifHighTemp, &andHighCo2);
//DECLARE_RULE(ifHighTempAndlowCo2ThenMidRisk, &thenMidRisk, 2, &ifHighTemp, &andLowCo2);
#pragma udata

#pragma udata ruleEngine
DECLARE_ENGINE(engine, 3, &ifHighTempThenHighRisk,
        &ifMidTempThenMidRisk, &ifLowTempThenLowRisk);
//DECLARE_ENGINE(engine, 2, &ifHighTempAndHighCo2ThenHighRisk, &ifHighTempAndlowCo2ThenMidRisk);
#pragma udata

#endif
