#include "fir.h"

#include "stdlib.h"
#include "string.h"
#include "stdio.h"

static INT32 acc;                // accumulator for MACs
static INT16* coeffp;            // pointer to coefficients
static INT16* inputp;            // pointer to input samples
static UINT8 n;

static INT16* _coefficients;
static INT16* _input;
static INT16* _output;
static INT16 _insamp[BUFFER_LENGTH];

static void Fir_initBuffer(void);
static void Fir_prepareVariables(void);
static void Fir_processInput(void);
static void Fir_calculateMAC(void);
static void Fir_saturate(void);
static void Fir_shiftBuffer(void) ;

void Fir_Create(INT16* input, INT16* output, INT16* coefficients) {
    _input = input;
    _output = output;
    _coefficients = coefficients;
}

INT16* Fir_Coefficients(void) {
	return _coefficients;
}

INT16* Fir_Input(void) {
	return _input;
}

void Fir_Filter(void) {
    Fir_initBuffer();
    Fir_processInput();
    Fir_shiftBuffer();
}

/** Init buffer and put the new samples at the high end of the buffer */
static void Fir_initBuffer(void) {
    memset(_insamp,0,BUFFER_LENGTH*(sizeof(INT16)));
    memcpy( &_insamp[FILTER_LENGTH - 1], _input, SIGNAL_LENGTH*sizeof(INT16));
}

static void Fir_processInput(void) {
    for ( n = 0; n < SIGNAL_LENGTH; n++ ) {
        Fir_prepareVariables();
        Fir_calculateMAC();
        Fir_saturate();
    }
}

static void Fir_prepareVariables(void) {
    coeffp = _coefficients;
    inputp = &_insamp[FILTER_LENGTH - 1 + n];
}

/** Perform the multiply-accumulate */
static void Fir_calculateMAC(void) {
    UINT8 k;
    acc = 0;
    for ( k = 0; k < FILTER_LENGTH; k++ ) {
        acc += (INT32)(*coeffp++) * (INT32)(*inputp--);
    }
}

/** Saturate accumulator */
static void Fir_saturate(void) {
    if ( acc > 0x3fffffff ) {
        acc = 0x3fffffff;
    } else if ( acc < -0x40000000 ) {
        acc = -0x40000000;
    }
    _output[n] = (INT16)(acc);
}

/** Shift input samples back in time for next time */
static void Fir_shiftBuffer(void) {
    memmove( &_insamp[0], &_insamp[SIGNAL_LENGTH], (size_t)(FILTER_LENGTH - 1));
}

void Fir_Destroy(void)
{
}