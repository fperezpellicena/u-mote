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

#ifndef bsp_constants_h
#define bsp_constants_h

/*..........................................................................*/
/* Operating mode */
#define DEBUG		1
#define NORMAL		2
#define MODE		DEBUG

/*..........................................................................*/
/* Sleep mode */
#define AWAKE           0
#define DEEP_SLEEP      1

/*..........................................................................*/
/* IO PIN STATUS */

#define INPUT_PIN	1
#define OUTPUT_PIN	0

/*..........................................................................*/
/* MODES */
#define MONITORING          1           /* Continuous sensing mode */
#define FUZZY_DRIVEN        2           /* Alert fuzzy based mode */

/*...........................................................................*/
/* GENERAL */
#define MAX_SENSORS                 5                       /* Max sensors */
#define MAX_INTERRUPT_HANDLERS      5            /* Max interrupt handlers */
#define MAX_PAYLOAD                 100

/*..........................................................................*/
/* UART SECTION */
// 1Mhz clock source, brgh = 1, brg16 = 1, sync = 0
//#define EUSART_9600            25	// FIXME Depends on Fosc
// 8Mhz clock source, brgh = 1, brg16 = 1, sync = 0
#define EUSART_9600            207	// FIXME Depends on Fosc

#endif
