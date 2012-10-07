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

#ifndef timer_h
#define timer_h

#define TIMER_8_MODE        0b01000000
#define TIMER_CKI_SRC       0b00100000
#define TIMER_BYPASS_PRESC  0b00001000


typedef enum Prescaler {
    PRESCALER_2 = 0b00000000, PRESCALER_4 = 0b00000001,
    PRESCALER_8 = 0b00000010, PRESCALER_16 = 0b00000011,
    PRESCALER_32 = 0b00000100, PRESCALER_64 = 0b00000101,
    PRESCALER_128 = 0b00000110, PRESCALER_256 = 0b00000111
};

/*...........................................................................*/
void Timer0_init(Prescaler prescaler, UINT8 mode, UINT8 clkSrc, UINT8 bypassPresc,
        BOOL enableInt);

/*...........................................................................*/
void Timer0_on();

/*...........................................................................*/
void Timer0_off();



#endif /* timer_h */