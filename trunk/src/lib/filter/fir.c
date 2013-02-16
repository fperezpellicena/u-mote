#include "fir.h"

#include "stdlib.h"
#include "string.h"
#include "stdio.h"

static float acc; // accumulator for MACs
static float* coeffp; // pointer to coefficients
static float* inputp; // pointer to input samples
static UINT8 n;

static float* _output;
static float* _input;
static float* _coefficients;
static float _insamp[BUFFER_LENGTH];

static void Fir_initBuffer(void);
static void Fir_prepareVariables(void);
static void Fir_processInput(void);
static void Fir_calculateMAC(void);
static void Fir_shiftBuffer(void);

void Fir_Create(float* input, float* output,
        float* coefficients) {
    _input = input;
    _output = output;
    _coefficients = coefficients;
}

float* Fir_Coefficients(void) {
    return _coefficients;
}

float* Fir_Input(void) {
    return _input;
}

void Fir_Filter(void) {
    Fir_initBuffer();
    Fir_processInput();
    Fir_shiftBuffer();
}

/** Init buffer and put the new samples at the high end of the buffer */
static void Fir_initBuffer(void) {
    memset(_insamp, 0, BUFFER_LENGTH * (sizeof (float)));
    memcpy(&_insamp[FILTER_LENGTH - 1], _input, SIGNAL_LENGTH * sizeof (float));
}

static void Fir_processInput(void) {
    for (n = 0; n < SIGNAL_LENGTH; n++) {
        Fir_prepareVariables();
        Fir_calculateMAC();
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
    for (k = 0; k < FILTER_LENGTH; k++) {
        acc += (*coeffp++) * (*inputp--);
    }
    _output[n] = acc;
}

/** Shift input samples back in time for next time */
static void Fir_shiftBuffer(void) {
    memmove(&_insamp[0], &_insamp[SIGNAL_LENGTH], (FILTER_LENGTH - 1) * sizeof (float));
}

void Fir_Destroy(void) {
}


