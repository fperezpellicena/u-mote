#ifndef D_Median_H
#define D_Median_H

#include "GenericTypeDefs.h"

#define SIGNAL_LENGTH       100
#define WINDOW_LENGTH       5
#define WINDOW_MIDDLE       WINDOW_LENGTH / 2
#define EXTENDED_DELTA		WINDOW_LENGTH / 2
#define EXTENDED_LENGTH     SIGNAL_LENGTH + (WINDOW_LENGTH  - 1)

void Median_Create(UINT16* input, UINT16* output);
void Median_Destroy(void);

void Median_Filter(void);
UINT16* Median_Extended(void);

#endif  /* D_FakeMedian_H */
