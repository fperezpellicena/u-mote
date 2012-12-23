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
 
#ifndef bsp_inertial_h
#define bsp_inertial_h

#include "bsp.h"


#define EUSART2_MODE		    EUSART_POLL

#define SENSING_MODE	    MONITORING

/*..........................................................................*/
/* ADXL section */

#define ADXL_ENABLED         1
#define SHT_ID              0x04

#if ADXL_ENABLED

#endif

/*...........................................................................*/
/* GPS section */

#define GPS_ENABLED   1
#define GPS_ID        0x08

#if GPS_ENABLED

#endif

/*..........................................................................*/
/* SENSOR BOARD CONTROL PINS */
#define SENSOR_BOARD_CTRL           LATBbits.LATB3      /* Power on/off pin*/
#define SENSOR_BOARD_CTRL_DDR       TRISBbits.TRISB3      /* Power on/off pin*/
#define SENSOR_BOARD_ON()           SENSOR_BOARD_CTRL = 1   /* Power on */
#define SENSOR_BOARD_OFF()          SENSOR_BOARD_CTRL = 0   /* Power off */

void BSP_inertialInit(void);

#endif /* umote_inertial_bsp_h */