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

#ifndef sht11_h
#define sht11_h

#define MEASURE_TEMP    0x03    		/* Measure temperature command */
#define MEASURE_HUMI    0x05    		/* Measure humidity command */
#define STAT_REG_R      0x07    		/* Read status register command */
#define STAT_REG_W      0x06    		/* Write status register command */
#define RESET           0x1E    		/* Reset */

#define ACK 1							/* Send ACK */
#define NACK 0							/* Not send ACK */

#define DATA        TRISBbits.TRISB1	/* Data tx/rx pin */
#define DATA_DDR    LATBbits.LATB1		/* Data direction register */
#define SCK         LATBbits.LATB2		/* Clk pin */
#define SCK_DDR     TRISBbits.TRISB2	/* Clk direction register */

/* Sensor measures temperature and humidity */
enum Modes {
    TEMP, HUMI
};

typedef union {
    unsigned int i;
    float f;
} value;

/**
 *	Generates a transmission start
 *       _____         ________
 * DATA:      |_______|
 *           ___     ___
 * SCK : ___|   |___|   |______
 */
void start(void);

/**
 *
 * Communication reset: DATA-line=1 and at least 9 SCK cycles followed by transstart
 *       _____________________________________________________         ________
 * DATA:                                                      |_______|
 *          _    _    _    _    _    _    _    _    _        ___     ___
 * SCK : __| |__| |__| |__| |__| |__| |__| |__| |__| |______|   |___|   |______
 */
void reset(void);

/* Resets the sensor by a softreset */
char softReset(void);

/* Reads a byte form the Sensibus and gives an acknowledge in case of "ack=1" */
unsigned char read(unsigned char ack);

/* Writes a byte on the Sensibus and checks the acknowledge */
char write(unsigned char value);

/* Reads the status register with checksum (8-bit) */
char readStatusRegister(unsigned char *p_value, unsigned char *p_checksum);

/* Writes the status register with checksum (8-bit) */
char writeStatusRegister(unsigned char *p_value);

/* Makes a measurement (humidity/temperature) with checksum */
char measure(int *p_value, unsigned char *p_checksum, unsigned char mode);

/* 
 * Calculates temperature [°C] and humidity [%RH]
 * input :  humi [Ticks] (12 bit)
 *          temp [Ticks] (14 bit)
 * output:  humi [%]
 */
void calculate(float *p_humidity, float *p_temperature);

void calculateHumidity(float* p_humidity);

void calculateTemperature(float* p_temp);


#endif /* sht11_h */