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
#include "hw_serial.h"
#include "digi_proxy.h"
#include "isr.h"
#include "sensor_proxy.h"
#include "list.h"
#include <rtcc.h>

#ifdef SHT_ENABLED
#include "sht11.h"
#endif

#ifdef GPS_ENABLED
#include "gps_isr.h"
#include "gps_api.h"
#endif

#ifdef RTCC_ENABLED
#include "rtc.h"
#endif

#pragma udata digi_isr_data
static rtccTimeDate timestampData; /* Timestamp data */

#ifdef SHT_ENABLED
static Sht11 shtData; /* SHT11 sensor data */
#endif

#ifdef GPS_ENABLED
static NMEAOutput gpsData; /* GPS data */
#endif



#pragma udata

static List list;
static Serial xbeeProxySerial;
static XBeePacket xbeeProxyPacket;
#if MODE == TEST
static UINT8 payload[11] = {'H', 'O', 'L', 'A', ' ', 'M', 'U', 'N', 'D', 'O', '\0'};
#else
static UINT8 payload[100];
#endif
static UINT8 XBEE_SINK_ADDRESS[8] = {0x00, 0x13, 0xA2, 0x00, 0x40, 0x70, 0xCF, 0x56};

/*..........................................................................*/

/* XBee proxy constructor */
void XBeeProxy_init(void) {
    List_init(&list);
    Serial_init(&xbeeProxySerial, XBEE_SERIAL, EUSART_9600);
    XBeeProxy_installInterrupt();
}


/*..........................................................................*/

/* Send XBee packet */
BOOL XBeeProxy_sendPacket(XBeePacket * const packet) {
    UINT8* p = (UINT8*) packet;
    Serial_send(&xbeeProxySerial, START_DELIMITER);
    // send the most significant bit
    Serial_send(&xbeeProxySerial, (packet->length >> 8) & 0xFF);
    // then the LSB
    Serial_send(&xbeeProxySerial, packet->length & 0xFF);
    // just in case it hasn't been initialized.
    packet->checksum = 0;
    //Generalizando para cualquier paquete
    while (packet->length--) {
        Serial_send(&xbeeProxySerial, *p);
        packet->checksum += *p++;
    }
    Serial_send(&xbeeProxySerial, (0xFF - packet->checksum));
    Serial_send(&xbeeProxySerial, NULL);
    return TRUE;
}

/*..........................................................................*/

/* Read XBee packet(generalized)*/
BOOL XBeeProxy_readPacket(XBeePacket * const packet) {
    UINT8 data;
    XBee_resetPacket(packet);
    while (Serial_available(&xbeeProxySerial)) {
        data = Serial_read(&xbeeProxySerial);
        switch (packet->rxState) {
            case XBEE_PACKET_RX_START:
                if (data == XBEE_PACKET_RX_START)
                    packet->rxState = XBEE_PACKET_RX_LENGTH_1;
                break;
            case XBEE_PACKET_RX_LENGTH_1:
                packet->length = data;
                packet->length <<= 8;
                packet->rxState = XBEE_PACKET_RX_LENGTH_2;
                break;
            case XBEE_PACKET_RX_LENGTH_2:
                packet->length += data;
                // in case we somehow get some garbage
                if (packet->length > MAX_PACKET_SIZE) {
                    packet->rxState = XBEE_PACKET_RX_START;
                    return FALSE; //LENGTH Error
                } else {
                    packet->rxState = XBEE_PACKET_RX_PAYLOAD;
                }
                packet->checksum = 0;
                break;
            case XBEE_PACKET_RX_PAYLOAD:
                *packet->dataPtr++ = data;
                if (++packet->index >= packet->length) {
                    packet->rxState = XBEE_PACKET_RX_CRC;
                }
                packet->checksum += data;
                break;
            case XBEE_PACKET_RX_CRC:
                packet->checksum += data;
                packet->rxState = XBEE_PACKET_RX_START;
                if (packet->checksum == 0xFF) {
                    return TRUE; //Everything OK!
                } else {
                    return FALSE; //CRC Error
                }
        }
    }
    return FALSE;
}

/* Read XBee packet(generalized)*/
BOOL XBeeProxy_read(void) {
    UINT8 data = xbeeProxyPacket.lastByte;
    switch (xbeeProxyPacket.rxState) {
        case XBEE_PACKET_RX_START:
            if (data == XBEE_PACKET_RX_START)
                xbeeProxyPacket.rxState = XBEE_PACKET_RX_LENGTH_1;
            break;
        case XBEE_PACKET_RX_LENGTH_1:
            xbeeProxyPacket.length = data;
            xbeeProxyPacket.length <<= 8;
            xbeeProxyPacket.rxState = XBEE_PACKET_RX_LENGTH_2;
            break;
        case XBEE_PACKET_RX_LENGTH_2:
            xbeeProxyPacket.length += data;
            // in case we somehow get some garbage
            if (xbeeProxyPacket.length > MAX_PACKET_SIZE) {
                xbeeProxyPacket.rxState = XBEE_PACKET_RX_START;
                return FALSE; //LENGTH Error
            } else {
                xbeeProxyPacket.rxState = XBEE_PACKET_RX_PAYLOAD;
            }
            xbeeProxyPacket.checksum = 0;
            break;
        case XBEE_PACKET_RX_PAYLOAD:
            *xbeeProxyPacket.dataPtr++ = data;
            if (++xbeeProxyPacket.index >= xbeeProxyPacket.length) {
                xbeeProxyPacket.rxState = XBEE_PACKET_RX_CRC;
            }
            xbeeProxyPacket.checksum += data;
            break;
        case XBEE_PACKET_RX_CRC:
            xbeeProxyPacket.checksum += data;
            xbeeProxyPacket.rxState = XBEE_PACKET_RX_START;
            if (xbeeProxyPacket.checksum == 0xFF) {
                return TRUE; //Everything OK!
            } else {
                return FALSE; //CRC Error
            }
    }
    return FALSE;
}

/* Join mote */
BOOL XBeeProxy_join(void) {
    static UINT8 command[3];
    static UINT8 params[2];
    // Send ATCB1
    command[0] = 'C';
    command[1] = 'B';
    command[2] = '\0';
    params[0] = 0x01;
    params[1] = '\0';
    XBee_createCompleteATCommandPacket(&xbeeProxyPacket, 0x01, command, params, 0x01);
    XBeeProxy_sendPacket(&xbeeProxyPacket);
    // Send ATSM8
    command[0] = 'S';
    command[1] = 'M';
    command[2] = '\0';
    params[0] = 0x08;
    params[1] = '\0';
    XBee_createCompleteATCommandPacket(&xbeeProxyPacket, 0x02, command, params, 0x01);
    XBeeProxy_sendPacket(&xbeeProxyPacket);
    XBee_resetPacket(&xbeeProxyPacket);
}

/*..........................................................................*/

/** Interrupt handler section */

void XBeeProxy_installInterrupt(void) {
#if SLEEP_MODE == SLEEP
    // En Deep sleep no se ejecutan las interrupciones
    // Install interrupt handler
    InterruptHandler_addHI((HandleInterrupt) & XBeeProxy_handleTopHalveInterrupt,
            (HandleInterrupt) & XBeeProxy_handleBottomHalveInterrupt,
            (CheckInterrupt) & XBeeProxy_checkInterrupt);
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
void XBeeProxy_handleTopHalveInterrupt(void) {
#if XBEE_INTERRUPT == SERIAL_INTERRUPT
    // If serial interrupt is configured, read last received byte
    // and store it in lastByte field
    UINT8 byte = Serial_read(&xbeeProxySerial);
    xbeeProxyPacket.lastByte = byte;
    // ACK
    Serial_ackInterrupt(&xbeeProxySerial);
#else
    // if ON_SLEEP_INTERRUPT enabled
    //  if modem status enabled
    //      read full data from serial
    //  else
    //      clear ON_SLEEP flag
#if SLEEP_STATUS_MESSAGES
    XBeeProxy_readPacket(&xbeeProxyPacket);
#else
    // ACK
    XBEE_ON_SLEEP_CLEAR_FLAG;
#endif
#endif
}

/* Bottom halve interrupt handler*/
void XBeeProxy_handleBottomHalveInterrupt(void) {
#if XBEE_INTERRUPT == SERIAL_INTERRUPT
    // If serial interrupt is configured, process last received byte
    // and check for valid frame
    // If valid frame received, create new data frame and send
    if (XBeeProxy_read() == TRUE) {
        // Crea una trama y la envía
    }
#else
#   if SENSING_MODE == ALERT_DRIVEN
    // Send frame only on alert condition or on monitoring mode
    if (SensorProxy_alert()) {
        // Prepara la nueva trama
        List_init(&list);
        // Read datetime and put into buffer
        Rtc_readToList(&list);
        // Put sensor ids
        List_add(&list, SensorProxy_getSensorByte());
        // Put sensor payload into buffer
        List_append(&list, (List*)SensorProxy_getMeasures());
        // Send prepared request (hay que prepararla antes para optimizar
        // el tiempo que está despierto el sistema)
        XBee_createTransmitRequestPacket(&xbeeProxyPacket, 0x06, XBEE_SINK_ADDRESS,
                XBEE_RADIOUS, XBEE_OPTIONS, list.data, list.size);
        XBeeProxy_sendPacket(&xbeeProxyPacket);
    }
#   else
    // Prepara la nueva trama
    List_init(&list);
    // Read datetime and put into buffer
    Rtc_readToList(&list);
    // Put sensor ids
    List_add(&list, SensorProxy_getSensorByte());
    // Sense installed sensors
    SensorProxy_sense();
    // Put sensor payload into buffer
    List_append(&list, (List*)SensorProxy_getMeasures());
    // Send prepared request (hay que prepararla antes para optimizar
    // el tiempo que está despierto el sistema)
    XBee_createTransmitRequestPacket(&xbeeProxyPacket, 0x06, XBEE_SINK_ADDRESS,
            XBEE_RADIOUS, XBEE_OPTIONS, list.data, list.size);
    XBeeProxy_sendPacket(&xbeeProxyPacket);
#   endif
#endif
}

BOOL XBeeProxy_checkInterrupt(void) {
#if XBEE_INTERRUPT == SERIAL_INTERRUPT
    // If serial interrupt is configured, check serial
    return Serial_checkInterrupt(&xbeeProxySerial);
#else
    // If pin interrupt is enabled, check pin
    return XBEE_ON_SLEEP_FLAG;
#endif
}
