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

#ifndef rules_h
#define rules_h

#include "fuzzy.h"

void initRules(RuleEngine* engine);

void initRule(RuleEngine* engine, Rule* rule, RuleTerm* antecedent1,
        RuleTerm* antecedent2, RuleTerm* consecuent);

void setInputs(void);

#endif /* rules_h */