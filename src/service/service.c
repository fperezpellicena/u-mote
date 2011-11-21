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

#include "service.h"
#include "bsp.h"
#include "hw_adc.h"

/**
 * Init mote resources(XBee, Serial, XBeeProxy).
 * Init BSP package.
 *
 * @param mote		pointer to mote struct
 * @param serial	pointer to serial struct
 * @param xbee		ponter to xbee struct
 * @param proxy		ponter to proxy struct
 */
void Service_initMote(Mote* mote, Serial* serial, XBee* xbee, XBeeProxy* proxy) {
	/* BSP init */
	BSP_init();
	
	/* Create XBee */
	XBee_create(xbee);

	/* Create Serial */
	Serial_create(serial, XBEE_SERIAL, DEFAULT_UART_CONFIG, 25);
	
	/* Init serial */
	Serial_init(serial);

	/* Create Proxy */
	XBeeProxy_create(proxy, serial, xbee);

	/* Init ADC */
	Adc_init(DEFAULT_ADC_CONFIG, DEFAULT_ADC_CONFIG2, 15);

	/* Create Mote */
	Mote_create(mote, proxy);
}