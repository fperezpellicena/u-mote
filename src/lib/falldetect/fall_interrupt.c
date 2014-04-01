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

#include "fall_interrupt.h"

#include "median.h"
#include "fir.h"
#include "ee.h"

#include "timer.h"
#include "adxl.h"

#include "isr.h"
#include "bsp_inertial.h"

#ifdef _DEBUG_
    #include <string.h>
    #include <stdio.h>
#endif

INT16 FIR_COEFFS[BHPF_FIRCOEFLEN]= { 
    TO_INT16(-0.014029679416517803),TO_INT16(-0.015353146769406889),TO_INT16(-0.016821731454366731),
    TO_INT16(-0.018464703937430434),TO_INT16(-0.020320051623746581),TO_INT16(-0.022437966939499551),
    TO_INT16(-0.024886155198915114),TO_INT16(-0.027758175443984078),TO_INT16(-0.031187049076401664),
    TO_INT16(-0.035368474460942427),TO_INT16(-0.040602613070882851),TO_INT16(-0.047374447294806388),
    TO_INT16(-0.056521801107109249),TO_INT16(-0.06962738805349751 ),TO_INT16(-0.090082941177665729),
    TO_INT16(-0.12670725431989477 ),TO_INT16(-0.21183638785016976 ),TO_INT16(-0.63649634090376972 ),
    TO_INT16( 0.63649634090376972 ),TO_INT16( 0.21183638785016976 ),TO_INT16( 0.12670725431989477 ),
    TO_INT16( 0.090082941177665729),TO_INT16( 0.06962738805349751 ),TO_INT16( 0.056521801107109249),
    TO_INT16( 0.047374447294806388),TO_INT16( 0.040602613070882851),TO_INT16( 0.035368474460942427),
    TO_INT16( 0.031187049076401664),TO_INT16( 0.027758175443984078),TO_INT16( 0.024886155198915114),
    TO_INT16( 0.022437966939499551),TO_INT16( 0.020320051623746581),TO_INT16( 0.018464703937430434),
    TO_INT16( 0.016821731454366731),TO_INT16( 0.015353146769406889),TO_INT16( 0.014029679416517803)
};

UINT8 humanProcessingSignal = FALSE;
UINT8 eeProcessingSignal = FALSE;
UINT8 gu8DP_HPFilterSignal = FALSE;
UINT8 eeFallDetectedSignal = FALSE;

#pragma udata median_buffer_x
static MedianBuffer medianBufferX;
#pragma udata

#pragma udata median_buffer_y
static MedianBuffer medianBufferY;
#pragma udata

#pragma udata fir_buffer_x
static FirBuffer firBufferX;
#pragma udata

#pragma udata fir_buffer_y
static FirBuffer firBufferY;
#pragma udata

#pragma udata ee_buffer_x
static EEBuffer eeBufferX;
#pragma udata

#pragma udata ee_buffer_y
static EEBuffer eeBufferY;
#pragma udata

void FallInterrupt_init(void) {
    Median_Create(&medianBufferX);
    Median_Create(&medianBufferY);
    Fir_Create(&firBufferX, FIR_COEFFS);
    Fir_Create(&firBufferY, FIR_COEFFS);
    EE_Create(&eeBufferX);
    EE_Create(&eeBufferY);
}

/* This function is called in interrupt context triggered by a hw. timer */
void FallInterrupt_readAccelerations(void) {
    if (Median_IsFull(&medianBufferX) && gu8DP_HPFilterSignal == FALSE) {
        // Process x-coord. median
        Median_BufferExport(&medianBufferX);
        Fir_Add(&firBufferX, Median_Calculate(&medianBufferX));
        Median_ReadIndexOffsetAdjust(&medianBufferX);
        // Process y-coord. median
        Median_BufferExport(&medianBufferY);
        Fir_Add(&firBufferY, Median_Calculate(&medianBufferY));
        Median_ReadIndexOffsetAdjust(&medianBufferY);
        if (Fir_IsFull(&firBufferX) == FIR_BUFFER_FULL) {
            LATAbits.LATA3 = !LATAbits.LATA3;
            humanProcessingSignal = TRUE;
        } else {
            humanProcessingSignal = FALSE;
        }
    } else {
        Median_Add(&medianBufferX, Adxl_accelerationX());
        Median_Add(&medianBufferY, Adxl_accelerationY());
    }
}

void FallInterrupt_HumanProcessing(void) {
    gu8DP_HPFilterSignal = TRUE;
    eeFallDetectedSignal = FALSE;
    Fir_BufferExport(&firBufferX);
    EE_Add(&eeBufferX, Fir_Calculate(&firBufferX));
    Fir_ReadIndexOffsetAdjust(&firBufferX);

    Fir_BufferExport(&firBufferY);
    EE_Add(&eeBufferY, Fir_Calculate(&firBufferY));
    Fir_ReadIndexOffsetAdjust(&firBufferY);
    if(EE_IsFull(&eeBufferX)) {
        eeProcessingSignal = TRUE;
    }
    gu8DP_HPFilterSignal = FALSE;
    LATCbits.LATC0 = !LATCbits.LATC0;
}

UINT32 energyExpenditure = 0;

void FallInterrupt_EEProcessing(void) {
    EE_BufferExport(&eeBufferX);
    energyExpenditure = EE_Calculate(&eeBufferX);       // SUM(x?)
    EE_ReadIndexOffsetAdjust(&eeBufferX);

    EE_BufferExport(&eeBufferY);
    energyExpenditure += EE_Calculate(&eeBufferY);       // SUM(y?)
    EE_ReadIndexOffsetAdjust(&eeBufferY);
   
    if (energyExpenditure >= EE_MAXTHRESHOLD) {
        eeFallDetectedSignal = TRUE;
    }
    eeProcessingSignal = FALSE;
}

void FallInterrupt_addInfoToPayload(Payload* payload) {
//    Payload_putByte(payload, (UINT8) energyExpenditure >> 24);
//    Payload_putByte(payload, (UINT8) energyExpenditure >> 16);
//    Payload_putByte(payload, (UINT8) energyExpenditure >> 7);
//    Payload_putByte(payload, (UINT8) energyExpenditure);
    if (eeFallDetectedSignal == TRUE) {
        Payload_putByte(payload, EE_FALL);
        eeFallDetectedSignal = FALSE;
    } else {
        Payload_putByte(payload, EE_NO_FALL);
    }
}