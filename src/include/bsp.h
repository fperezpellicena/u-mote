/**
 * This file is part of uMote.
 *
 *  uMote is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *  uMote is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with uMote.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef bsp_h
#define bsp_h

#include <p18f46j50.h>         /* SFR declarations for PIC18F46J50 device */
#include <usart.h>                                        /* UART library */
#include <timers.h>                                     /* Timers library */
#include <adc.h>                                           /* ADC library */

#include "digi.h"                                         /* XBee library */

                                                  /* Oscillator frequency */
#define BSP_FOSC_HZ          12000000UL

#define DEFAULT_UART_CONFIG USART_TX_INT_OFF	& \
                            USART_RX_INT_ON	& \
                            USART_ASYNCH_MODE	& \
                            USART_EIGHT_BIT	& \
                            USART_CONT_RX	& \
                            USART_BRGH_HIGH

#define DEFAULT_ADC_CONFIG  ADC_FOSC_32         & \
                            ADC_RIGHT_JUST      & \
                            ADC_1ANA

#define DEFAULT_ADC_CONFIG2 ADC_CH0             & \
                            ADC_INT_ON

/*--------------------------------------------------------------------------*/
                                               /* the system tick rate [Hz] */
#define BSP_TICKS_PER_SEC    20UL

/*..........................................................................*/
/* Init basic BSP */
void BSP_init(void);

/*..........................................................................*/
/* Check for power-on reset */
boolean BSP_powerOnReset(void);

/*..........................................................................*/
/* Reset power-on reset bit */
void BSP_clearPowerOnReset(void);

/*..........................................................................*/
                                            /* Low level hardware interface */

void Interrupts_enable(void);

void Timer0_init(void);

void Usart1_init(void);

void Usart1_write(uint8_t byte);

#endif                                                             /* bsp_h */
