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

#include <p18f46j50.h>
#include <spi.h>

// Conversion start words
#define PRESSURE        0x0F40
#define TEMPERATURE     0x0F20

// Calibration words
#define READ_CAL_W1     0x1D50
#define READ_CAL_W2     0x1D60
#define READ_CAL_W3     0x1D90
#define READ_CAL_W4     0x1DA0

// Reset word
#define RESET           0x155540


#pragma config WDTEN=OFF, OSC=EC


void init(void) {
    
}

void reset(void) {

}

void main(void) {



}