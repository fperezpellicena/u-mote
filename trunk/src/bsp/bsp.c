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

#include "qp_port.h"
#include "bsp.h"

/*..........................................................................*/
/* Init basic BSP */
void BSP_init(void) {
    TRISE = 0;                  /* data direction for Port E (LEDs): output */
    LATE = 0;
	
    RCONbits.IPEN = 1;                            /* enable priority levels */
}

/*..........................................................................*/
/* Check for power-on reset */
boolean BSP_powerOnReset(void) {
	return WDTCONbits.DS == 1 ? true : false;
}

/*..........................................................................*/
/* Reset power-on reset bit */
void BSP_clearPowerOnReset(void) {
	WDTCONbits.DS = 0;
}

/*..........................................................................*/
void Interrupts_enable(void) {
    INTCONbits.GIE = 1;
}

/*..........................................................................*/
void Timer0_init(void) {
    OpenTimer0(TIMER_INT_ON & T0_SOURCE_INT & T0_16BIT & T0_PS_1_32);
}

/*..........................................................................*/
void Usart1_init(void) {
    Open1USART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE &
            USART_EIGHT_BIT & USART_CONT_RX, 25);
}

/*..........................................................................*/
void Usart1_write(uint8_t byte) {
    Write1USART(byte);
}
