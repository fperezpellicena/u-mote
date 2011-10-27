/*
 * File:   main.c
 */

#include <p18f46j50.h>
#include "qp_port.h"
#include "xbee.h"

/* Use custom pragma section because of XBee object size */
#pragma udata xbee_section
struct XBee xbee;
#pragma udata

void main(void) {
    return;
}