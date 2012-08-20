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
#include <delays.h>

void Sht11_init(void) {
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
        Delay1KTCYx(1); //observe setup time
        SHT_SCK = 1; //clk for SENSI-BUS
        Delay1KTCYx(3); //pulswith approx. 5 us
        SHT_SCK = 0;
        Delay1KTCYx(1); //observe hold time
    }
    SHT_DATA = 1; //release SHT_DATA-line
    Delay1KTCYx(1); //observe setup time
    SHT_SCK = 1; //clk #9 for ack
    Delay1KTCYx(2);
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
        Delay1KTCYx(3);
        SHT_SCK = 1; //clk for SENSI-BUS
        if (SHT_DATA_PIN) {
            val = (val | i); //read bit
        }
        Delay1KTCYx(3);
        SHT_SCK = 0;
    }

    if (ack) {
        SHT_DATA_DDR = 0;
        SHT_DATA = 0; //in case of "ack==1" pull down SHT_DATA-Line
    }
    Delay1KTCYx(2); //observe setup time
    SHT_SCK = 1; //clk #9 for ack
    Delay1KTCYx(3); //pulswith approx. 5 us
    SHT_SCK = 0;
    Delay1KTCYx(1); //observe hold time
    return val;
}

//----------------------------------------------------------------------------------

void Sht11_start(void) {
    SHT_DATA = 1;
    SHT_SCK = 0; //Initial state
    Delay1KTCYx(1);
    SHT_SCK = 1;
    Delay1KTCYx(1);
    SHT_DATA = 0;
    Delay1KTCYx(1);
    SHT_SCK = 0;
    Delay1KTCYx(3);
    SHT_SCK = 1;
    Delay1KTCYx(1);
    SHT_DATA = 1;
    Delay1KTCYx(1);
    SHT_SCK = 0;
}

void Sht11_reset(void) {
    UINT8 i;
    SHT_DATA = 1;
    SHT_SCK = 0; //Initial state
    //9 SHT_SCK cycles
    for (i = 0; i < 9; i++) {
        SHT_SCK = 1;
        Delay1KTCYx(1);
        SHT_SCK = 0;
    }
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
    error += Sht11_measureParam((int*) &sht->data->temperature.i,
            &sht->data->temp_chk, SHT_MEASURE_TEMP);
#ifdef __18F46J50_H
    LATDbits.LATD1 = !LATDbits.LATD1;
#endif
    SHT_DATA_DDR = 0;
    SHT_DATA = 1;
    error += Sht11_measureParam((int*) &sht->data->humidity.i,
            &sht->data->humi_chk, SHT_MEASURE_HUMI);

    return error;
}

UINT8 Sht11_measureParam(int *p_value, UINT8 *p_checksum, UINT8 mode) {
    UINT8 error = 0;
    Sht11_start(); //transmission start
    switch (mode) { //send command to sensor
        case SHT_MEASURE_TEMP: error += Sht11_write(SHT_MEASURE_TEMP);
            break;
        case SHT_MEASURE_HUMI: error += Sht11_write(SHT_MEASURE_HUMI);
            break;
        default: break;
    }
    Delay1KTCYx(5);
    SHT_DATA_DDR = 1;
    // Long busy wait can block usb

    while (SHT_DATA_PIN == 1);

    // Read two bytes response
    // FIXME No es crítico: No funciona con usb, intentar habilitar una interrupción de pin
    // y encapsular las siguientes líneas dentro de la ISR.
    if (mode == SHT_MEASURE_HUMI && SHT_LOW_POWER) {
        *(p_value) = Sht11_read(SHT_ACK); //read the first byte (MSB)
    } else {
        *(p_value) = Sht11_read(SHT_ACK); //read the first byte (MSB)
        *(p_value) = *(p_value) << 8;
        *(p_value) += Sht11_read(SHT_ACK); //read the second byte (LSB)
    }
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

void Sht11_calculateHumidity(float* p_humidity) {
    const float C1 = 4;
    const float C2 = -0.0405;
    const float C3 = 0.0000028;
    float measure = *p_humidity;
    *p_humidity = C1 * measure + C2 * measure + C3 * measure * measure;
}

void Sht11_calculateTemperature(float* p_temp) {
    const float D1 = 39.64;
    const float D2 = -0.01;
    float measure = *p_temp;
    *p_temp = D1 + D2 * measure;
}
