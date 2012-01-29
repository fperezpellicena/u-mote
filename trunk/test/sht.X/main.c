#include <p18f46j50.h>
#include <usart.h>
#include <stdio.h>
#include <delays.h>
#include <math.h>
#include <stdlib.h>

#pragma config WDTEN = OFF

#define MEASURE_TEMP    0x03    /* Measure temperature command */
#define MEASURE_HUMI    0x05    /* Measure humidity command */
#define STAT_REG_R      0x07    /* Read status register command */
#define STAT_REG_W      0x06    /* Write status register command */
#define RESET           0x1E    /* Reset */

#define ACK 1
#define NACK 0

#define DATA        TRISBbits.TRISB1
#define DATA_DDR    LATBbits.LATB1
#define SCK         LATBbits.LATB2
#define SCK_DDR     TRISBbits.TRISB2

enum Modes {
    TEMP, HUMI
};

// writes a byte on the Sensibus and checks the acknowledge

static char write(unsigned char value) {
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

//----------------------------------------------------------------------------------

unsigned char read(unsigned char ack)
//----------------------------------------------------------------------------------
// reads a byte form the Sensibus and gives an acknowledge in case of "ack=1"
{
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

void start(void)
//----------------------------------------------------------------------------------
// generates a transmission start
//       _____         ________
// DATA:      |_______|
//           ___     ___
// SCK : ___|   |___|   |______
{
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
//----------------------------------------------------------------------------------

void reset(void)
//----------------------------------------------------------------------------------
// communication reset: DATA-line=1 and at least 9 SCK cycles followed by transstart
//       _____________________________________________________         ________
// DATA:                                                      |_______|
//          _    _    _    _    _    _    _    _    _        ___     ___
// SCK : __| |__| |__| |__| |__| |__| |__| |__| |__| |______|   |___|   |______
{
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

//----------------------------------------------------------------------------------

char softReset(void)
//----------------------------------------------------------------------------------
// resets the sensor by a softreset
{
    unsigned char error = 0;
    reset(); //reset communication
    error += write(RESET); //send RESET-command to sensor
    return error; //error=1 in case of no response form the sensor
}

//----------------------------------------------------------------------------------

char readStatusRegister(unsigned char *p_value, unsigned char *p_checksum)
//----------------------------------------------------------------------------------
// reads the status register with checksum (8-bit)
{
    unsigned char error = 0;
    start(); //transmission start
    error = write(STAT_REG_R); //send command to sensor
    *p_value = read(ACK); //read status register (8-bit)
    *p_checksum = read(NACK); //read checksum (8-bit)
    return error; //error=1 in case of no response form the sensor
}
//----------------------------------------------------------------------------------

char writeStatusRegister(unsigned char *p_value)
//----------------------------------------------------------------------------------
// writes the status register with checksum (8-bit)
{
    unsigned char error = 0;
    start(); //transmission start
    error += write(STAT_REG_W); //send command to sensor
    error += write(*p_value); //send value of status register
    return error; //error>=1 in case of no response form the sensor
}

//----------------------------------------------------------------------------------

char measure(int *p_value, unsigned char *p_checksum, unsigned char
        mode)
//----------------------------------------------------------------------------------
// makes a measurement (humidity/temperature) with checksum
{
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

//----------------------------------------------------------------------------------

void calculate(float *p_humidity, float *p_temperature)
//----------------------------------------------------------------------------------
// calculates temperature [°C] and humidity [%RH]
// input :  humi [Ticks] (12 bit)
//          temp [Ticks] (14 bit)
// output:  humi [%RH]
//          temp [°C]
{
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

void calculateSimpleHumi(float* p_humidity) {
    const float C1 = 4;
    const float C2 = -0.0405;
    const float C3 = 0.0000028;
    float measure = *p_humidity;
    *p_humidity = C1 * measure + C2 * measure + C3 * measure * measure;
}

void calculateSimpleTemp(float* p_temp) {
    const float D1 = 39.64;
    const float D2 = -0.01;
    float measure = *p_temp;
    *p_temp = D1 + D2 * measure;
}

typedef union {
    unsigned int i;
    float f;
} value;

void main(void) {
    value humi_val, temp_val;
    unsigned char error, checksum;
    unsigned int i;
    DATA_DDR = 0;
    SCK_DDR = 0;
    DATA = 1;
    SCK = 0;

    TRISDbits.TRISD1 = 0;
    LATDbits.LATD1 = 0;

    ANCON1bits.PCFG10 = 1;
    ANCON1bits.PCFG8 = 1;

    Open1USART(USART_TX_INT_OFF &
            USART_RX_INT_OFF &
            USART_ASYNCH_MODE &
            USART_EIGHT_BIT &
            USART_CONT_RX &
            USART_BRGH_HIGH,
            25);
    reset();
    //softReset();
    while (1) {
        //start();
        error = 0;
        DATA_DDR = 0;
        DATA = 1;
        error += measure(&humi_val.i, &checksum, HUMI); //measure humidity
        //printf("%u", humi_val.i);
        DATA_DDR = 0;
        DATA = 1;
        error += measure(&temp_val.i, &checksum, TEMP); //measure temperature
        if (error != 0) {
            Write1USART('D');
            reset(); //in case of an error: connection reset
        } else {
            humi_val.f = (float) humi_val.i; //converts integer to float
            temp_val.f = (float) temp_val.i; //converts integer to float
            //calculate(&humi_val.f, &temp_val.f); //calculate humidity,temperature
            calculateSimpleHumi(&humi_val.f);
            printf("Humedad relativa:%d.%2u % \n\r",
                    (int) humi_val.f / 100, fabs(((humi_val.f - (int) humi_val.f)*100)));
            calculateSimpleTemp(&temp_val.f);
            printf("Temperatura: %d.%2u % \n\r",
                    (int)temp_val.f, fabs(((temp_val.f - (int)temp_val.f)*100)));
        }


        for (i = 0; i < 40000; i++);
    }
}
