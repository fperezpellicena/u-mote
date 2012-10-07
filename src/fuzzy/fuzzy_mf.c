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

static float_t fuzzy;

static BOOL isLeftAligned(MembershipFunction* function) {
    return function->mid == function->left;
}

static BOOL isRightAligned(MembershipFunction* function) {
    return function->mid == function->right;
}

static float_t calculateLeftAligned(UINT8 value, MembershipFunction* function) {
    if (value < function->left) {
        return (float_t) 1;
    } else if (value > function->right) {
        return (float_t) 0;
    } else {
        return (float_t) (function->right - value) / (float_t) (function->right - function->mid);
    }
}

static float_t calculateRightAligned(UINT8 value, MembershipFunction* function) {
    if (value > function->right) {
        return (float_t) 1;
    } else if (value < function->left) {
        return (float_t) 0;
    } else {
        return (float_t) (value - function->left) / (float_t) (function->mid - function->left);
    }
}

static float_t calculateCentered(UINT8 value, MembershipFunction* function) {
    if (value > function->left && value < function->mid) {
        return (value - function->left) / (function->mid - function->left);
    } else if (value > function->mid && value < function->right) {
        return (float_t) (function->right - value) / (float_t) (function->right - function->mid);
    } else {
        return (float_t) 1;
    }
}

/*..........................................................................*/

/* Triangular fuzzify function */
float_t triangularFuzzify(UINT8 value, MembershipFunction* function) {
    if (isLeftAligned(function)) {
        return calculateLeftAligned(value, function);
    } else if (isRightAligned(function)) {
        return calculateRightAligned(value, function);
    } else {
        return calculateCentered(value, function);
    }
}
