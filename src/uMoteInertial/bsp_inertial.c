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
#include "bsp_inertial.h"
#include "wdt.h"
#include "digi_api.h"
#include "sensor_proxy.h"
#include "adxl.h"
#include <delays.h>
#if RTCC_ENABLED
#include "rtc.h"
#include "timer.h"
#endif
#include "nmea_output.h"
#include "gps.h"
#include "gps_api.h"
#include "gps_isr.h"
#include "rtc.h"
#include "digi_api.h"
#include "fall_interrupt.h"
#ifdef _DEBUG_
#include <string.h>
#include <stdio.h>
#endif

extern BOOL humanProcessingSignal;
extern BOOL eeProcessingSignal;
extern BOOL eeFallDetectedSignal;
extern BOOL gpsPacketReceived;

static const UINT8 XBEE_SINK_ADDRESS[8] = {0x00, 0x13, 0xA2, 0x00, 0x40, 0x70, 0xCF, 0x56};

static enum MainState state = INIT;

static Payload payload;

static BOOL fallDetectedFlag = FALSE;

#pragma udata isr_xbee
static XBeePacket xbeePacket;
#pragma udata

void BSP_inertialInit(void) {
    BSP_init(); // Initializes mote API
    XBee_init();
    Adxl_init();
    Timer0_init();
    Gps_init();
    GpsInterrupt_init();
    FallInterrupt_init();
    BSP_initInterrupts();
    BSP_onPowerUp();
    Timer0_start();
    Wdt_enable();
}

void BSP_initInterrupts(void) {
    RCONbits.IPEN = 1;
    INTCONbits.GIEH = 1; /* Enable interrupts*/
    INTCONbits.GIEL = 1; /* Enable interrupts*/
}

/*...........................................................................*/
void BSP_onPowerUp(void) {
#if RTCC_ENABLED
    // Enable RTCC
    Rtc_init();
#endif
    Delay10KTCYx(10);
    XBee_join();
}

/*...........................................................................*/
void BSP_onMclr(void) {
    return;
}

/*...........................................................................*/
void BSP_onWakeUp(void) {
    return;
}

/*...........................................................................*/
void BSP_onDsWdtWakeUp(void) {
    return;
}

void BSP_mainProcess(void) {
    Wdt_clear();
    switch (state) {
        case INIT:
#ifdef _DEBUG_
            printf("INIT \n");
#endif
            state = IDLE;
            break;
        case IDLE:
#ifdef _DEBUG_
            printf("IDLE \n");
#endif
            if (humanProcessingSignal == TRUE) {
                state = HUMAN_PROCESSING;
            }
            if (gpsPacketReceived == TRUE) {
                state = GPS_PROCESSING;
            }
            break;
        case HUMAN_PROCESSING:
#ifdef _DEBUG_
            printf("HUMAN_PROCESSING \n");
#endif
            FallInterrupt_HumanProcessing();
            if (eeProcessingSignal == TRUE) {
                state = EE_PROCESSING;
            } else {
                state = IDLE;
            }
            break;
        case GPS_PROCESSING:
#ifdef _DEBUG_
            printf("GPS_PROCESSING \n");
#endif
            GpsInterrupt_process();
            state = IDLE;
            break;
        case EE_PROCESSING:
#ifdef _DEBUG_
            printf("EE_PROCESSING \n");
#endif
            FallInterrupt_EEProcessing();
            if (eeFallDetectedSignal == TRUE) {
                state = EE_FALLDETECTED;
            } else {
                state = IDLE;
            }
            break;
        case EE_FALLDETECTED:
            fallDetectedFlag = TRUE;
#ifdef _DEBUG_
            printf("FALL!!");
#endif
            state = IDLE;
            break;
    }
}

void BSP_sendPacket(void) {
    Payload_init(&payload);
    Rtc_addCurrentTimeToPayload(&payload);
    GpsInterrupt_addInfoToPayload(&payload);
    FallInterrupt_addInfoToPayload(&payload);
    XBee_createTransmitRequestPacket(&xbeePacket, 0x06, (UINT8*) XBEE_SINK_ADDRESS,
            XBEE_RADIOUS, XBEE_OPTIONS, payload.data, payload.size);
    XBee_sendPacket(&xbeePacket);
}
