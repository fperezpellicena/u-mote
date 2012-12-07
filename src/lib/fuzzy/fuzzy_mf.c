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

#include "fuzzy_mf.h"

static UINT8 fuzzy;
static UINT8 input;
static MembershipFunction* mf;

static UINT8 fuzzyfyRightBranch(void);

static UINT8 fuzzyfyRightBranch(void) {
    return ((mf->right - input) * 10) / ((mf->right - mf->mid) / 10);
}

static UINT8 fuzzyfyLeftBranch(void);

static UINT8 fuzzyfyLeftBranch(void) {
    return ((input - mf->left) * 10) / ((mf->mid - mf->left) / 10);
}

static UINT8 fuzzyfyRightDefined(void);

static UINT8 fuzzyfyRightDefined(void) {
    if (input < mf->left) {
	fuzzy = 100;
    } else if (input > mf->right) {
	fuzzy = 0;
    } else {
	fuzzy = fuzzyfyRightBranch();
    }
}


static UINT8 fuzzyfyLeftDefined(void);

static UINT8 fuzzyfyLeftDefined(void) {
    if (input > mf->right) {
	fuzzy = 100;
    } else if (input < mf->left) {
	fuzzy = 0;
    } else {
	fuzzy = fuzzyfyLeftBranch();
    }
}

static UINT8 isInputInLeftBranch(void);

static UINT8 isInputInLeftBranch(void) {
    return input > mf->left && input < mf->mid;
}

static UINT8 isInputInRightBranch(void);

static UINT8 isInputInRightBranch(void) {
    return input > mf->mid && input < mf->right;
}

static UINT8 fuzzyfyFullDefined(void);

static UINT8 fuzzyfyFullDefined(void) {
    if (isInputInLeftBranch()) {
	fuzzy = fuzzyfyLeftBranch();
    } else if (isInputInRightBranch()) {
	fuzzy = fuzzyfyRightBranch();
    } else {
	fuzzy = 0;
    }
}

static UINT8 isRightDefined(void);

static UINT8 isRightDefined(void) {
    return mf->mid == mf->left;
}

static UINT8 isLeftDefined(void);

static UINT8 isLeftDefined(void) {
    return mf->mid == mf->right;
}

/*..........................................................................*/

/* Triangular fuzzify function */
UINT8 triangularFuzzify(UINT8 value, MembershipFunction* function) {
    fuzzy = 0;
    input = value;
    mf = function;
    if (isRightDefined()) {
	fuzzyfyRightDefined();
    } else if (isLeftDefined()) {
	fuzzyfyLeftDefined();
    } else {
	fuzzyfyFullDefined();
    }
    return fuzzy;
}
