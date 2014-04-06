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

#ifndef bsp_config_h
#define	bsp_config_h

#include <p18cxxx.h>
#include "bsp_constants.h"

#define SLEEP_MODE      DEEP_SLEEP

/*...........................................................................*/
#define USB_ENABLED         0
#define RTCC_ENABLED        1
#define ADC_INT_ENABLED     0

/*...........................................................................*/
/* XBEE SECTION */
#define XBEE_BAUDRATE	    EUSART_9600

/*...........................................................................*/
/* USB attach detector */
#define USB_PLUG_POR        PORTBbits.RB4
#define USB_PLUG_TRI        TRISBbits.TRISB4

/*...........................................................................*/
/* ADC SECTION */
#define AVERAGE_FACTOR	    16
#define DIV_AVERAGE	    4

/*...........................................................................*/
/* USART2 SECTION */
#define USART2_RX_RP        2
#define USART2_TX_RP        5


#endif
