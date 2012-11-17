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

#if SHT_ENABLED
#ifndef sht11_h
#define sht11_h

#include "GenericTypeDefs.h"
#include "sensor_proxy.h"
#include "fuzzy_rule.h"
#include "payload.h"

#define SHT_MEASURE_TEMP    0x03    		/* Measure temperature command */
#define SHT_MEASURE_HUMI    0x05    		/* Measure humidity command */
#define SHT_STAT_REG_R      0x07    		/* Read status register command */
#define SHT_STAT_REG_W      0x06    		/* Write status register command */
#define SHT_RESET           0x1E    		/* Reset */

#define SHT_LOW_POWER       0x01                /* 8bit RH / 12 bit TMP */

#define SHT_ACK             1			/* Send ACK */
#define SHT_NACK            0                   /* Not send ACK */

#if SENSING_MODE == FUZZY_DRIVEN
#define MAX_TERMS   (UINT8)3
/* Sht fuzzy terms */
typedef struct ShtFuzzyTerms ShtFuzzyTerms;

struct ShtFuzzyTerms {
    RuleTerm * tempTerms[MAX_TERMS];
    RuleTerm * humiTerms[MAX_TERMS];
    UINT8 tempTermsSize;
    UINT8 humiTermsSize;
};

#endif
/* Sht data class */
typedef struct ShtData ShtData;

struct ShtData {

    union {
        UINT16 i;
        float f;
    } temperature;

    union {
        UINT16 i;
        float f;
    } humidity;
    UINT8 tempChk;
    UINT8 humiChk;
};

/* Declare sht sensor */
#define DECLARE_SHT(id, name) \
    Sht name = {id, {0,0,0,0}, NULL}

/* Sht11 class */
typedef struct Sht Sht;

struct Sht {
    UINT8 id;
    ShtData data;
    ShtFuzzyTerms terms;
};

/* Init pins and registers*/
void Sht11_init(void);

/**
 * Generates a transmission start
 *       _____         ________
 * DATA:      |_______|
 *           ___     ___
 * SCK : ___|   |___|   |______
 */
void Sht11_start(void);

/**
 *
 * Communication reset: DATA-line=1 and at least 9 SCK cycles followed by transstart
 *       _____________________________________________________         ________
 * DATA:                                                      |_______|
 *          _    _    _    _    _    _    _    _    _        ___     ___
 * SCK : __| |__| |__| |__| |__| |__| |__| |__| |__| |______|   |___|   |______
 */
void Sht11_reset(void);

/* Resets the sensor by a softreset */
UINT8 Sht11_softReset(void);

/* Reads a byte form the Sensibus and gives an acknowledge in case of "ack=1" */
UINT8 Sht11_read(UINT8 ack);

/* Writes a byte on the Sensibus and checks the acknowledge */
UINT8 Sht11_write(UINT8 value);

/* Reads the status register with checksum (8-bit) */
UINT8 Sht11_readStatusRegister(UINT8 *p_value, UINT8 *p_checksum);

/* Writes the status register with checksum (8-bit) */
UINT8 Sht11_writeStatusRegister(UINT8 p_value);

/* Makes a measurement (humidity/temperature) with checksum */
UINT8 Sht11_measureParam(UINT16 *p_value, UINT8 *p_checksum, UINT8 mode);

/* Measures temperature and humidity */
UINT8 Sht11_measure(Sht* sht);

/* Measures temperature and humidity */
UINT8 Sht11_measureAndCalculate(Sht* sht);


/* 
 * Calculates temperature [°C] and humidity [%RH]
 * input :  humi [Ticks] (12 bit)
 *          temp [Ticks] (14 bit)
 * output:  humi [%]
 */
void Sht11_calculate(float *p_humidity, float *p_temperature);

void Sht11_calculateHumidity(float* p_humidity);

void Sht11_calculateTemperature(float* p_temp);

void Sht11_addMeasuresToPayload(Sht* sht, Payload* payload);

void Sht11_addMeasuresCalculatedToPayload(Sht* sht, Payload* payload);

#if SENSING_MODE == FUZZY_DRIVEN
void Sht11_setFuzzyInputs(Sht* sht);
void Sht11_addTempTerm(Sht* sht, RuleTerm* term);
void Sht11_addHumiTerm(Sht* sht, RuleTerm* term);
#endif

#endif /* sht11_h */
#endif
