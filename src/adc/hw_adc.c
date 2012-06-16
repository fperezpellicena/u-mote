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
#include <adc.h>

/**
 * Configura el ADC
 */
void Adc_init(UINT8 config, UINT8 config2, UINT8 channel) {
    while (BusyADC());
    CloseADC();
    OpenADC(config, config2, channel);
}

void Adc_close() {
    CloseADC();
}

static void Adc_dummy();

/**
 * Realiza una conversión dummy para compensar el ruido de las entradas.
 */
static void Adc_dummy() {
    ADCON1bits.ADCAL = 1;
    ConvertADC();
    while (BusyADC());
    ADCON1bits.ADCAL = 0;
}

/**
 * Inicia una conversión, precedida de una dummy.
 */
void Adc_startConversion() {
    Adc_dummy();
    ConvertADC();
}

/**
 * Lee el valor del conversor.
 *
 * @return conversión
 */
UINT16 Adc_readValue() {
    while (BusyADC());
    return ReadADC();
}

/**
 * Devuelve una muestra sampleada del ADC
 * Inicia el proceso tomando una muestra dummy para compensar el efecto del ruido
 * de las entradas.
 * A continuación, toma una muestra
 *
 * @return muestra con ruido de entrada compensado
 */
UINT16 Adc_getValue() {
    Adc_dummy();
    ConvertADC();
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
UINT16 Adc_getAveragedValue() {
    UINT32 tmp = 0;
    UINT8 i = AVERAGE_FACTOR;
    Adc_dummy();
    while (i--) {
        ConvertADC();
        tmp += Adc_readValue();
    }
    return tmp >> DIV_AVERAGE;
}
