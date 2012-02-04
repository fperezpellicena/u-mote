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

#include "sht11.h"
#include <p18f46j50.h>
#include <usart.h>
#include <stdio.h>
#include <delays.h>
#include <math.h>
#include <stdlib.h>

char write(unsigned char value) {
    unsigned char i, error = 0;
    //shift bit for masking
    for (i = 0x80; i > 0; i /= 2) {
        if (i & value) {
            DATA = 1; //masking value with i , write to SENSI-BUS
        } else {
            DATA = 0;
        }
        Delay1KTCYx(1); //observe setup time
        SCK = 1; //clk for SENSI-BUS
        Delay1KTCYx(3); //pulswith approx. 5 us
        SCK = 0;
        Delay1KTCYx(1); //observe hold time
    }
    DATA = 1; //release DATA-line
    Delay1KTCYx(1); //observe setup time
    SCK = 1; //clk #9 for ack
    Delay1KTCYx(2);
    // To read SHT ack(low), change pin direction
    DATA_DDR = 1;
    // Read pin
    error = PORTBbits.RB1; //check ack (DATA will be pulled down by SHT11)
    SCK = 0;
    return error; //error=1 in case of no acknowledge
}

unsigned char read(unsigned char ack) {
    unsigned char i, val = 0;
    //printf("Valor inicial: %u \r\n", val);
    DATA_DDR = 1;
    DATA = 1;
    //Write1USART('X');
    for (i = 0x80; i > 0; i /= 2) //shift bit for masking
    {
        Delay1KTCYx(3);
        SCK = 1; //clk for SENSI-BUS
        if (PORTBbits.RB1) {
            val = (val | i); //read bit
            //      Write1USART('1');
        } else {
            //    Write1USART('0');
        }
        Delay1KTCYx(3);
        SCK = 0;
    }
    //Write1USART('Y');

    //Write1USART('Z');
    if (ack) {
        DATA_DDR = 0;
        DATA = 0; //in case of "ack==1" pull down DATA-Line
    }
    Delay1KTCYx(2); //observe setup time
    SCK = 1; //clk #9 for ack
    Delay1KTCYx(3); //pulswith approx. 5 us
    SCK = 0;
    Delay1KTCYx(1); //observe hold time
    //printf("\r\nValor leído: %u \r\n", val);
    return val;
}

//----------------------------------------------------------------------------------

void start(void) {
    DATA = 1;
    SCK = 0; //Initial state
    Delay1KTCYx(1);
    SCK = 1;
    Delay1KTCYx(1);
    DATA = 0;
    Delay1KTCYx(1);
    SCK = 0;
    Delay1KTCYx(3);
    SCK = 1;
    Delay1KTCYx(1);
    DATA = 1;
    Delay1KTCYx(1);
    SCK = 0;
}

void reset(void) {
    unsigned char i;
    DATA = 1;
    SCK = 0; //Initial state
    for (i = 0; i < 9; i++) //9 SCK cycles
    {
        SCK = 1;
        Delay1KTCYx(1);
        SCK = 0;
    }
    start(); //transmission start
}

char softReset(void) {
    unsigned char error = 0;
    reset(); //reset communication
    error += write(RESET); //send RESET-command to sensor
    return error; //error=1 in case of no response form the sensor
}

char readStatusRegister(unsigned char *p_value, unsigned char *p_checksum) {
    unsigned char error = 0;
    start(); //transmission start
    error = write(STAT_REG_R); //send command to sensor
    *p_value = read(ACK); //read status register (8-bit)
    *p_checksum = read(NACK); //read checksum (8-bit)
    return error; //error=1 in case of no response form the sensor
}

char writeStatusRegister(unsigned char *p_value) {
    unsigned char error = 0;
    start(); //transmission start
    error += write(STAT_REG_W); //send command to sensor
    error += write(*p_value); //send value of status register
    return error; //error>=1 in case of no response form the sensor
}

char measure(int *p_value, unsigned char *p_checksum, unsigned char mode) {
    unsigned char error = 0;
    // unsigned int i;
    int val;
    start(); //transmission start
    switch (mode) { //send command to sensor
        case TEMP: error += write(MEASURE_TEMP);
            break;
        case HUMI: error += write(MEASURE_HUMI);
            break;
        default: break;
    }
    Delay1KTCYx(5);
    DATA_DDR = 1;
    while (PORTBbits.RB1 == 1);
    LATDbits.LATD1 = 1;

    //    val = read(ACK);
    //    val = val << 8;
    //    printf("\nValor devuelto 1: %i \r\n", val);
    //
    //    val = val | read(ACK);
    //     printf("\nValor devuelto 2: %u \r\n", val);
    //     printf("\nValor devuelto 3: %u \r\n", read(NACK));

    *(p_value) = read(ACK); //read the first byte (MSB)
    *(p_value) = *(p_value) << 8;
    *(p_value) += read(ACK); //read the second byte (LSB)
    // printf("Valor: %u \r\n", *(p_value));
    *p_checksum = read(NACK); //read checksum

    return error;
}

void calculate(float *p_humidity, float *p_temperature) {
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

void calculateHumidity(float* p_humidity) {
    const float C1 = 4;
    const float C2 = -0.0405;
    const float C3 = 0.0000028;
    float measure = *p_humidity;
    *p_humidity = C1 * measure + C2 * measure + C3 * measure * measure;
}

void calculateTemperature(float* p_temp) {
    const float D1 = 39.64;
    const float D2 = -0.01;
    float measure = *p_temp;
    *p_temp = D1 + D2 * measure;
}
