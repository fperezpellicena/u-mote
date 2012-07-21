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

#include "hw_adc.h"
#include "bsp.h"

/**
 * Configura el ADC
 */
void Adc_init(void) {
    ADCON0bits.VCFG = 0;
    ADCON1bits.ADFM = 0; /* Left adjust */
    ADCON1bits.ACQT = 0; /* Automatic acquisition time */
    ADCON1bits.ADCS = 0; /* Clk Fosc/2 */
#if ADC_INT_ENABLED
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    INTCONbits.PEIE = 1;
#endif
    ADCON0bits.ADON = 0; /* Start power off */
}

void Adc_close(void) {
    ADCON0bits.ADON = 0; /* Power off */
}

static void Adc_dummy(void);

/**
 * Realiza una conversión dummy para compensar el ruido de las entradas.
 */
static void Adc_dummy(void) {
    ADCON1bits.ADCAL = 1;
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO);
    ADCON1bits.ADCAL = 0;
}

/**
 * Inicia una conversión, precedida de una dummy.
 */
void Adc_startConversion(UINT8 channel) {
    ANCON0 = channel >> 8;  /* VBG Selection and port configuration */
    ANCON1 = channel;       /* Port configuration */
    Adc_dummy();
    ADCON0bits.GO = 1;
}

/**
 * Lee el valor del conversor.
 *
 * @return conversión
 */
UINT16 Adc_readValue(void) {
    while (ADCON0bits.GO);
    return (((UINT16) ADRESH) << 8) | (ADRESL);
}

/**
 * Devuelve una muestra sampleada del ADC
 * Inicia el proceso tomando una muestra dummy para compensar el efecto del ruido
 * de las entradas.
 * A continuación, toma una muestra
 *
 * @return muestra con ruido de entrada compensado
 */
UINT16 Adc_convert(UINT8 channel) {
    ANCON0 = channel >> 8;  /* VBG Selection and port configuration */
    ANCON1 = channel;       /* Port configuration */
    Adc_dummy();
    ADCON0bits.GO = 1;
    return Adc_readValue();
}

/**
 * Devuelve una muestra promedio de AVERAGE_FACTOR muestras.
 * Inicia el proceso tomando una muestra dummy para compensar el efecto del ruido
 * de las entradas.
 * A continuación, promedia tantas muestras como indica el factor de conversión.
 *
 * @return conversión promediada y compensada en ruido
 */
UINT16 Adc_convertAveragedValue(UINT8 channel) {
    UINT32 tmp = 0;
    UINT8 i = AVERAGE_FACTOR;
    ANCON0 = channel >> 8;  /* VBG Selection and port configuration */
    ANCON1 = channel;       /* Port configuration */
    Adc_dummy();
    while (i--) {
        ADCON0bits.GO = 1;
        tmp += Adc_readValue();
    }
    return tmp >> DIV_AVERAGE;
}
