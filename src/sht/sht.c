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

#include "sht.h"

#if SHT_ENABLED
#include <delays.h>
#include <stdio.h>
#include <math.h>
#include "payload.h"

void Sht11_init() {
    SHT_DATA_CNF = 1;
    SHT_SCK_CNF = 1;
    SHT_DATA = 1;
    SHT_SCK_DDR = 0;
    SHT_DATA_DDR = 0;
    SHT_SCK = 0;
}

UINT8 Sht11_write(UINT8 value) {
    UINT8 i, error = 0;
    //shift bit for masking
    for (i = 0x80; i > 0; i /= 2) {
        if (i & value) {
            SHT_DATA = 1; //masking value with i , write to SENSI-BUS
        } else {
            SHT_DATA = 0;
        }
        Delay10TCYx(10); //observe setup time
        SHT_SCK = 1; //clk for SENSI-BUS
        Delay10TCYx(10); //pulswith approx. 5 us
        SHT_SCK = 0;
        Delay10TCYx(10); //observe hold time
    }
    SHT_DATA = 1; //release SHT_DATA-line
    Delay10TCYx(10); //observe setup time
    SHT_SCK = 1; //clk #9 for ack
    Delay10TCYx(10);
    // To read SHT ack(low), change pin direction
    SHT_DATA_DDR = 1;
    // Read pin
    error = SHT_DATA_PIN; //check ack (SHT_DATA will be pulled down by SHT11)
    SHT_SCK = 0;
    return error; //error=1 in case of no acknowledge
}

UINT8 Sht11_read(UINT8 ack) {
    UINT8 i, val = 0;
    SHT_DATA_DDR = 1;
    SHT_DATA = 1;
    for (i = 0x80; i > 0; i /= 2) //shift bit for masking
    {
        Delay10TCYx(10);
        SHT_SCK = 1; //clk for SENSI-BUS
        if (SHT_DATA_PIN) {
            val = (val | i); //read bit
        }
        Delay10TCYx(10);
        SHT_SCK = 0;
    }

    if (ack) {
        SHT_DATA_DDR = 0;
        SHT_DATA = 0; //in case of "ack==1" pull down SHT_DATA-Line
    }
    Delay10TCYx(10); //observe setup time
    SHT_SCK = 1; //clk #9 for ack
    Delay10TCYx(10); //pulswith approx. 5 us
    SHT_SCK = 0;
    Delay10TCYx(10); //observe hold time
    return val;
}

//----------------------------------------------------------------------------------

void Sht11_start(void) {
    SHT_DATA = 1;
    SHT_SCK = 0; //Initial state
    Delay10TCYx(10);
    SHT_SCK = 1;
    Delay10TCYx(10);
    SHT_DATA = 0;
    Delay10TCYx(10);
    SHT_SCK = 0;
    Delay10TCYx(10);
    SHT_SCK = 1;
    Delay10TCYx(10);
    SHT_DATA = 1;
    Delay10TCYx(10);
    SHT_SCK = 0;
}

void Sht11_reset(void) {
    UINT8 i;
    SHT_DATA = 1;
    SHT_SCK = 0; //Initial state
    //9 SHT_SCK cycles
    for (i = 0; i < 9; i++) {
        SHT_SCK = 1;
        Delay10TCYx(10);
        SHT_SCK = 0;
        Delay10TCYx(10);
    }
    Delay10TCYx(10);
    Sht11_start(); //transmission start
}

UINT8 Sht11_softReset(void) {
    UINT8 error = 0;
    Sht11_reset(); //reset communication
    error += Sht11_write(SHT_RESET); //send SHT_RESET-command to sensor
    return error; //error=1 in case of no response form the sensor
}

UINT8 Sht11_readStatusRegister(UINT8 *p_value, UINT8 *p_checksum) {
    UINT8 error = 0;
    Sht11_start(); //transmission start
    error = Sht11_write(SHT_STAT_REG_R); //send command to sensor
    *p_value = Sht11_read(SHT_ACK); //read status register (8-bit)
    *p_checksum = Sht11_read(SHT_NACK); //read checksum (8-bit)
    return error; //error=1 in case of no response form the sensor
}

UINT8 Sht11_writeStatusRegister(UINT8 p_value) {
    UINT8 error = 0;
    Sht11_start(); //transmission start
    error += Sht11_write(SHT_STAT_REG_W); //send command to sensor
    SHT_DATA_DDR = 0;
    error += Sht11_write(p_value); //send value of status register
    return error; //error>=1 in case of no response form the sensor
}

UINT8 Sht11_measureAndCalculate(Sht* sht) {
    UINT8 error = Sht11_measure(sht);
    //converts integer to float
    sht->data->temperature.f = (float) sht->data->temperature.i;
    //converts integer to float
    sht->data->humidity.f = (float) sht->data->humidity.i;
    //calculate humidity,temperature
    Sht11_calculate(&sht->data->humidity.f, &sht->data->temperature.f);
    return error;
}

UINT8 Sht11_measure(Sht* sht) {
    UINT8 error = 0;
    // Get measures
    error += Sht11_measureParam(&sht->data->temperature.i, &sht->data->tempChk,
            SHT_MEASURE_TEMP);
    error += Sht11_measureParam(&sht->data->humidity.i, &sht->data->humiChk,
            SHT_MEASURE_HUMI);
    return error;
}

UINT8 Sht11_measureParam(UINT16 *p_value, UINT8 *p_checksum, UINT8 mode) {
    UINT8 error = 0;
    SHT_DATA_DDR = 0;
    SHT_DATA = 1;
    Sht11_start(); //transmission start
    switch (mode) { //send command to sensor
        case SHT_MEASURE_TEMP: error += Sht11_write(SHT_MEASURE_TEMP);
            break;
        case SHT_MEASURE_HUMI: error += Sht11_write(SHT_MEASURE_HUMI);
            break;
        default: break;
    }
    Delay10TCY();
    SHT_DATA_DDR = 1;
    // FIXME Long busy wait could block usb, use interrupts instead.
    while (SHT_DATA_PIN == 1);
    // Read two bytes response
    *(p_value) = Sht11_read(SHT_ACK); //read the first byte (MSB)
    *(p_value) = *(p_value) << 8;
    *(p_value) += Sht11_read(SHT_ACK); //read the second byte (LSB)
    *p_checksum = Sht11_read(SHT_NACK); //read checksum

    return error;
}

void Sht11_calculate(float *p_humidity, float *p_temperature) {
    const float C1 = -2.0468; // for 12 Bit RH
    const float C2 = +0.0367; // for 12 Bit RH
    const float C3 = -0.0000015955; // for 12 Bit RH
    const float T1 = +0.01; // for 12 Bit RH
    const float T2 = +0.00008; // for 12 Bit RH
    float rh = *p_humidity; // rh:      Humidity [Ticks] 12 Bit
    float t = *p_temperature; // t:       Temperature [Ticks] 14 Bit
    float rh_lin; // rh_lin:  Humidity linear
    float rh_true; // rh_true: Temperature compensated humidity
    float t_C; // t_C   :  Temperature [°C]
    t_C = t * 0.01 - 40.1; //calc. temperature[°C]from 14 bit temp.ticks @5V
    rh_lin = C3 * rh * rh + C2 * rh + C1; //calc. humidity from ticks to [%RH]
    rh_true = (t_C - 25)*(T1 + T2 * rh) + rh_lin; //calc. temperature compensated humidity [%RH]
    if (rh_true > 100)rh_true = 100; //cut if the value is outside of
    if (rh_true < 0.1)rh_true = 0.1; //the physical possible range
    *p_temperature = t_C; //return temperature [°C]
    *p_humidity = rh_true; //return humidity[%RH]
}

/*...........................................................................*/

/* Add temperature and humidity to payload */
void Sht11_addMeasuresToPayload(Sht* sht, Payload* payload) {
    Payload_putWord(payload, sht->data->temperature.i);
    Payload_putWord(payload, sht->data->humidity.i);
}

/*...........................................................................*/

/* Add temperature and humidity to payload */
void Sht11_addMeasuresCalculatedToPayload(Sht* sht, Payload* payload) {
    // Calculate
    Sht11_calculate(&sht->data->humidity.f, &sht->data->temperature.f);
    // Build strings and append to payload
    sprintf(payload->data, (const MEM_MODEL rom char*) "Humedad relativa: %d.%2u % \n\r",
            (int) sht->data->humidity.f / 100,
            fabs(((sht->data->humidity.f - (int) sht->data->humidity.f)*100)));
    sprintf(payload->data, (const MEM_MODEL rom char*) "Temperatura: %d.%2u % \n\r",
            (UINT16) sht->data->temperature.f,
            fabs(((sht->data->temperature.f - (int) sht->data->temperature.f)*100)));
}

#if SENSING_MODE == FUZZY_DRIVEN
/*...........................................................................*/

/* Put measures into rule terms */
void Sht11_prepareFuzzyInputs(Sht* sht) {
    UINT8 i;
    float temperature;
    float humidity;
    ShtFuzzyTerms* terms = sht->terms;
    // Calculate values
    Sht11_calculate(&sht->data->humidity.f, &sht->data->temperature.f);
    temperature = (float) sht->data->temperature.i;
    humidity = (float) sht->data->humidity.i;
    // Put temperature measured into rule terms
    for (i = 0; i < terms->tempRulesSize; i++) {
        // Set integer term (Intentional precision loss)
        terms->tempRules[i]->input = (int) (temperature / 100);
    }
    // Put humidity measured into rule terms
    for (i = 0; i < terms->humiRulesSize; i++) {
        // Set integer term (Intentional precision loss)
        terms->humiRules[i]->input = (int) (humidity / 100);
    }
}
#endif
#endif
