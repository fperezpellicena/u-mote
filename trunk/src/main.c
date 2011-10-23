/*
 * File:   main.c
 */

#include <p18f46j50.h>
#include "qp_port.h"

void main(void) {
    QF_init();     /* initialize the framework and the underlying RT kernel */
    QF_run();      /*                               /* run the QF application */
}