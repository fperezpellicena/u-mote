#ifndef D_Fir_H
#define D_Fir_H

#include "GenericTypeDefs.h"

#define FILTER_LENGTH	3
#define SIGNAL_LENGTH	9
#define BUFFER_LENGTH	FILTER_LENGTH + SIGNAL_LENGTH - 1

void Fir_Create(float* input, float* output, 
	float* coefficients);
void Fir_Destroy(void);

float* Fir_Coefficients(void);
float* Fir_Input(void);

void Fir_Filter(void);

#endif  /* D_FakeFir_H */
