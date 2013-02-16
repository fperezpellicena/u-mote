#include "median.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

static UINT16* _input;
static UINT16* _output;

static UINT16 _window[WINDOW_LENGTH];
static UINT16 _extension[EXTENDED_LENGTH];

static void Median_extendInput(void);
static BOOL Median_checkBounds(void);
static void Median_processSignal(void);
static void Median_fillWindow(UINT16 signalIndex);
static void Median_orderWindow(void);
static void Median_putMedian(UINT16 signalIndex);
static UINT16 Median_findMinimum(UINT16 currentWindowIndex);
static void Median_putMinimum(UINT16 currentWindowIndex, UINT16 minElementPosition);

void Median_Create(UINT16* input, UINT16* output) {
    _input = input;
    _output = output;
}

UINT16* Median_Extended(void) {
    return _extension;
}

/** Main filter processing */
void Median_Filter(void) {
    if (Median_checkBounds()) {
        return;
    }
    Median_extendInput();
    Median_processSignal();
}

/** Check for null input and special cases */
static BOOL Median_checkBounds(void) {
    //   Check arguments
    if (!_input || SIGNAL_LENGTH < 1) {
        return TRUE;
    }
    //   Treat special case N = 1
    if (SIGNAL_LENGTH == 1 && _output) {
        _output[0] = _input[0];
        return TRUE;
    }
    return FALSE;
}

/** Create signal extension */
static void Median_extendInput(void) {
    UINT16 extensionIndex;
    memcpy(_extension + EXTENDED_DELTA, _input, SIGNAL_LENGTH * sizeof (UINT16));
    for (extensionIndex = 0; extensionIndex < EXTENDED_DELTA; ++extensionIndex) {
        _extension[extensionIndex] = _input[EXTENDED_DELTA - extensionIndex];
        _extension[SIGNAL_LENGTH + EXTENDED_DELTA + extensionIndex] = _input[SIGNAL_LENGTH - 2 - extensionIndex];
    }
}

/** Iterates over the input signal and processes window */
static void Median_processSignal(void) {
    UINT16 signalIndex;
    for (signalIndex = WINDOW_MIDDLE; signalIndex < EXTENDED_LENGTH - WINDOW_MIDDLE; ++signalIndex) {
        Median_fillWindow(signalIndex);
        Median_orderWindow();
        Median_putMedian(signalIndex);
    }
}

/** Fill current window picking the first element in the given signal index */
static void Median_fillWindow(UINT16 signalIndex) {
    UINT16 windowIndex;
    for (windowIndex = 0; windowIndex < WINDOW_LENGTH; ++windowIndex) {
        _window[windowIndex] = _extension[signalIndex + windowIndex - 2];
    }
}

/** Order window elements (only half of them) because we want only find the mid element */
static void Median_orderWindow(void) {
    UINT16 windowIndex;
    for (windowIndex = 0; windowIndex <= WINDOW_MIDDLE; ++windowIndex) {
        // Find position of minimum element and put found element in its place
        Median_putMinimum(windowIndex, Median_findMinimum(windowIndex));
    }
}

/** Put the median value found in the window into the output array */
static void Median_putMedian(UINT16 signalIndex) {
    _output[signalIndex - WINDOW_MIDDLE] = _window[WINDOW_MIDDLE];
}

/** Find position of minimum element for a given window iteration */
static UINT16 Median_findMinimum(UINT16 currentWindowIndex) {
    UINT16 minIndex;
    UINT16 minElementPosition = currentWindowIndex;
    for (minIndex = (UINT16) (currentWindowIndex + 1); minIndex < WINDOW_LENGTH; ++minIndex) {
        if (_window[minIndex] < _window[minElementPosition]) {
            minElementPosition = minIndex;
        }
    }
    return minElementPosition;
}

static void Median_putMinimum(UINT16 currentWindowIndex, UINT16 minElementPosition) {
    UINT16 tmp = _window[currentWindowIndex];
    _window[currentWindowIndex] = _window[minElementPosition];
    _window[minElementPosition] = tmp;
}

void Median_Destroy(void) {
}


