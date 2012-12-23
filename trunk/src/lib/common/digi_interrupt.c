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

#include "bsp.h"
#include "digi_api.h"
#include "digi_interrupt.h"
#include "sensor_proxy.h"
#include "payload.h"
#include "rtc.h"

static Payload payload;
static XBeePacket packet;
static const UINT8 XBEE_SINK_ADDRESS[8] = {0x00, 0x13, 0xA2, 0x00, 0x40, 0x70, 0xCF, 0x56};

/*..........................................................................*/

/** Interrupt handler section */

void XBeeInterrupt_install(void) {
    Payload_init(&payload);
#if SLEEP_MODE == SLEEP
    // En Deep sleep no se ejecutan las interrupciones
    // Install interrupt handler
    InterruptHandler_addHI((HandleInterrupt) & XBeeInterrupt_handleTopHalve,
	    (HandleInterrupt) & XBeeInterrupt_handleBottomHalve,
	    (CheckInterrupt) & XBeeInterrupt_check);
#endif
#if XBEE_INTERRUPT == ON_SLEEP_INTERRUPT
    // If ON_SLEEP_INTERRUPT enabled, configure pin interruption
    XBEE_ON_SLEEP_PIN;
    XBEE_ON_SLEEP_EDGE; // Rising edge
    XBEE_ON_SLEEP_CLEAR_FLAG; // Clear flag
    XBEE_ON_SLEEP_INT; // Enable interrupt
#endif
}

/* Top halve interrupt handler */
void XBeeInterrupt_handleTopHalve(void) {
#if XBEE_INTERRUPT == EUSART_SERIAL_INTERRUPT
    // If serial interrupt is configured, read last received byte
    // and store it in lastByte field
    UINT8 byte = XBeeSerial_read();
    packet.lastByte = byte;
    // ACK
    XBeeSerial_ackInterrupt();
#else
    // Comment:
    // if ON_SLEEP_INTERRUPT enabled
    //  if modem status enabled
    //      read full data from serial
    //  else
    //      clear ON_SLEEP flag
#    if SLEEP_STATUS_MESSAGES
    XBee_readPacket(&packet);
#    else
    // ACK
    XBEE_ON_SLEEP_CLEAR_FLAG;
#    endif
#endif
}

// TODO Move out to a custom handler
/* Bottom halve interrupt handler*/
void XBeeInterrupt_handleBottomHalve(void) {
#if XBEE_INTERRUPT == EUSART_SERIAL_INTERRUPT
    // If serial interrupt is configured, process last received byte
    // and check for valid frame
    // If valid frame received, create new data frame and send
    if (XBee_read() == TRUE) {
	// Crea una trama y la envía
    }
#else
    // Prepara la nueva trama
    Payload_init(&payload);
    // Read datetime and put into buffer
    Rtc_addTimeToPayload(&payload);
    // Put sensor ids
    SensorProxy_addSensorIdentifiersToPayload(&payload);
    // Sense installed sensors
    SensorProxy_sense();
    // Put sensor payload into buffer
    SensorProxy_addMeasuresToPayload(&payload);
    // Send prepared request (hay que prepararla antes para optimizar
    // el tiempo que está despierto el sistema)
    XBee_createTransmitRequestPacket(&packet, 0x06, (UINT8*) XBEE_SINK_ADDRESS,
	    XBEE_RADIOUS, XBEE_OPTIONS, payload.data, payload.size);
    XBee_sendPacket(&packet);
#endif
}

BOOL XBeeInterrupt_check(void) {
#if XBEE_INTERRUPT == EUSART_SERIAL_INTERRUPT
    // If serial interrupt is configured, check serial
    return XBeeSerial_checkInterrupt();
#else
    // If pin interrupt is enabled, check pin
    return XBEE_ON_SLEEP_FLAG;
#endif
}