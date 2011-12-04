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

/**
 *
 * Funcionamiento:
 *	Cada mote puede estar en dos modos de funcionamiento:
 *		1.- DEPLOY
 *		2.- RUN
 *
 * MODO DEPLOY
 *
 *
 *
 * MODO RUN
 *
 *	En modo RUN el mote estÃƒÂ¡ generalmente dormido, tanto el microcontrolador
 *	como el transceptor XBee.
 *	PeriÃƒÂ³dicamente de acuerdo con los parÃƒÂ¡metros de configuraciÃƒÂ³n del transceptor
 *	(SP y ST), el transceptor XBee se despertarÃƒÂ¡ activando el terminal ON/SLEEP.
 *	Esta activaciÃƒÂ³n capturada en una interrupciÃƒÂ³n del microcontrolador, provoca
 *	la entrada en modo activo del mismo.
 *	Durante el modo activo, el microcontrolador debe enviar la trama de datos
 *	correspondiente a las mediciones realizadas e inmediatamente despuÃƒÂ©s
 *	volver al modo de sueÃƒÂ±o.
 *	Debido a que el tiempo que estÃƒÂ¡ despierto el transceptor XBee estÃƒÂ¡ fijado
 *	por la configuraciÃƒÂ³n de la red, y que ÃƒÂ©s el ÃƒÂºnico tiempo disponible para
 *	transmitir, puede ser conveniente que la trama estÃƒÂ© previamente preparada
 *	para envÃƒÂ­o, pudiendo minimizar el tiempo en que el XBee estÃƒÂ¡ despierto.
 *
 *	Para ello:
 *		- Se podrÃƒÂ­a despertar al micro con anticipaciÃƒÂ³n(timer), y preparar la
 *		trama para el siguiente envÃƒÂ­o (trabajando con intervalos de sueÃƒÂ±o de varios
 *		minutos, no creo que sea relevante hablar de un desfase de varios segundos
 *		o incluso minutos entre el instante de envÃƒÂ­o real de la trama y el instante
 *		en que se ha realizado la mediciÃƒÂ³n).
 *
 *		- Se podrÃƒÂ­a realizar la operaciÃƒÂ³n de sensado tras enviar la trama, lo que
 *		significa que se envÃƒÂ­a siempre la medida correspondiente a la activaciÃƒÂ³n
 *		anterior(esto no es un problema). De esta segunda forma nos ahorramos
 *		el timer y los cambios de contexto de la interrupciÃƒÂ³n asociada.
 *
 *			**Â¿Â¿ES NECESARIO REVISAR SI EL ENVÃƒï¿½O HA SIDO CORRECTO??**
 *			**SI EL ENVÃƒï¿½O HA SIDO FALLIDO, QUÃ‰ SE HARÃ�A?**
 *
 * Cuando el transceptor XBee se despierta en sincronismo con la red,
 * pone en alto el pin ON/SLEEP(Diego me comenta que no funciona y por ello
 * han empleado el terminal /CTS).
 * El cambio de estado en el transceptor XBee identificador por el cambio en
 * la salida de terminal correspondiente, se puede recoger en un terminal
 * del microcontrolador.
 *
 * Si el micro estÃƒÂ¡ en modo Deep Sleep:
 *		Este es el modo de menor consumo en el que la CPU estÃ¡ completamente
 *		desactivada debido a que el regulador propio se apaga.
 *		SÃ³lo se puede salir de este estado si ocurre un hard reset, una alarma
 *		del RTCC, una interrupciÃƒÂ³n en INT0, un ULPWU o watchdog reset.
 *		No es necesario un manejador de interrupciones para INT0 y de hecho
 *		es necesario que las interrupciones globales(GIE) estÃ©n deshabilitadas.
 *
 * Para configurar el modo Deep Sleep:
 *	- Set REGSLP en WDTCON
 *	- Clear IDLEN
 *	- Clear GIE
 *	- Set DSEN en DSCONH
 *	- Execute Sleep()
 *
 * En el caso de despertar mediante cambio en INT0:
 *	- Set INTEDG0 for a rise edge change or clear for a falling edge
 *	- Set INT0IE to enable INT0 interrupt 
 */
#include "qp_port.h"
#include "bsp.h"
#include "digi.h"
#include "hw_serial.h"
#include "digi_proxy.h"
#include "digi_api.h"
#include "power.h"
#include "usb_config.h"
#include "usb_profile.h"
#include "service.h"
#include <portb.h>

// FIXME Sacar a un archivo de configuración
#pragma config WDTEN = OFF          //WDT disabled (enabled by SWDTEN bit)


#define TEST

Mote mote;
XBee xbee;
Serial serial;
#pragma udata packet
XBeePacket packet;
#pragma udata
XBeeProxy proxy;

#ifdef TEST
uint8_t atCommand[3] = "ND";
uint8_t coordinatorAddress[8] = {0x00, 0x13, 0xA2, 0x00, 0x01, 0x02, 0x03, 0x04};
uint8_t payload[11] = "0123456789";
uint8_t cluster[3] = "CL";
uint8_t profile[3] = "PF";

void toggleLed(void);
void test(void);

void toggleLed(void) {
	TRISE = 0;
	// Usa el latch para registrar la salida
    LATEbits.LATE1 = ~LATEbits.LATE1;
}

void test(void) {
	// Test send AtCommand
	XBee_createATCommandPacket(&packet, 'A', atCommand);
	XBeeProxy_sendPacket(&proxy, &packet);

	// Test send transmit request
	XBee_createTransmitRequestPacket(&packet, 'B', coordinatorAddress, 'R', 'O',
			payload, 11);
	XBeeProxy_sendPacket(&proxy, &packet);

	// Test send explicit addressing packet
	XBee_createExplicitAddressingPacket(&packet, 'C', coordinatorAddress, 'S',
			'D', cluster, profile, 'R', 'O', payload, 11);
	XBeeProxy_sendPacket(&proxy, &packet);

	// Test remote AtCommand
	XBee_createRemoteAtCommandPacket(&packet, 'D', atCommand, 'P',
			coordinatorAddress, 'O');
	XBeeProxy_sendPacket(&proxy, &packet);
}
#endif


void main() {

	/* Mote init */
	//Service_initMote(&mote, &serial, &xbee, &proxy);

	//TRISBbits.TRISB0 = 1;	// RB0 as input
	//ANCON1bits.PCFG12 = 1;	// RB0 as digital port
	TRISBbits.TRISB2 = 0;	// RB2 as output

	OpenRB0INT(PORTB_CHANGE_INT_ON & RISING_EDGE_INT & PORTB_PULLUPS_ON);
	

	/* Check for power on reset */
	if (BSP_powerOnReset()) {
		/* Clear power-on reset */
		BSP_clearPowerOnReset();
		/* Do Stuff */
#ifdef TEST
		toggleLed();
		LATBbits.LATB2 = ~LATBbits.LATB2;	// RB2 high
#endif
	}
	
	/* Deep sleep mode */
	

	while (1) {
		INTCON2bits.INTEDG0 = 1;   // 0 - interrupt on rising edge (patch off to on)
		INTCONbits.INT0IF = 0;      // clear INT0 interrupt flag
		INTCONbits.INT0IE = 1;      // enable INT0 interrupt
		deepSleep();
	}
}
