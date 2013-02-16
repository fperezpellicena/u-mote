#ifndef D_Fir_H
#define D_Fir_H

#include "GenericTypeDefs.h"

#define FILTER_LENGTH	3
#define SIGNAL_LENGTH	9
#define BUFFER_LENGTH	FILTER_LENGTH + SIGNAL_LENGTH - 1

void Fir_Create(INT16* input, INT16* output, INT16* coefficients);
void Fir_Destroy(void);

INT16* Fir_Coefficients(void);
INT16* Fir_Input(void);

void Fir_Filter(void);

#endif  /* D_FakeFir_H */
