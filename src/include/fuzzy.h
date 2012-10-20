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

#ifndef fuzzy_h
#define fuzzy_h

#include "GenericTypeDefs.h"
#include "fuzzy_rule.h"

#define MAX_RULES   5

#define DECLARE_ENGINE(name, size, ...)\
Rule* name##rules[size] = {__VA_ARGS__};\
RuleEngine name = {name##rules, size}


/*..........................................................................*/
/* Rules class */
typedef struct RuleEngine RuleEngine;

struct RuleEngine {
    Rule** rules;
    UINT8 size;
};

/*..........................................................................*/
/* Run engine */
UINT8 RuleEngine_run(RuleEngine* ruleEngine);

#endif /* fuzzy_h */
