#include <p18f46j50.h>
#include <spi.h>

// Conversion start words
#define PRESSURE        0x0F40
#define TEMPERATURE     0x0F20

// Calibration words
#define READ_CAL_W1     0x1D50
#define READ_CAL_W2     0x1D60
#define READ_CAL_W3     0x1D90
#define READ_CAL_W4     0x1DA0

// Reset word
#define RESET           0x155540


#pragma config WDTEN=OFF, OSC=EC


void init(void) {
    
}

void reset(void) {

}

void main(void) {



}