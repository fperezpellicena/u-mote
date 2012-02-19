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

#include <p18cxxx.h>
#include <stdio.h>
#include <usart.h>
#include <timers.h>
#include <rtcc.h>

/* Pragmas */
#pragma config WDTEN = OFF          //WDT disabled (enabled by SWDTEN bit)
//#pragma config PLLDIV = 3           //Divide by 3 (12 MHz oscillator input)
#pragma config STVREN = ON          //stack overflow/underflow reset enabled
#pragma config XINST = OFF           //Extended instruction set disabled
#pragma config CPUDIV = OSC1        //No CPU system clock divide
#pragma config CP0 = OFF            //Program memory is not code-protected
//#pragma config OSC = HSPLL          //HS oscillator, PLL enabled, HSPLL used by USB

#pragma config OSC = INTOSCPLL      // Internal OSC(8mhz), PLL enabled by soft
#pragma config PLLDIV = 2           // Divide By 2(4 Mhz) to generate 96Mhz

#pragma config FCMEN = OFF          //Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF           //Two-Speed Start-up disabled
#pragma config WDTPS = 32768        //1:32768
#pragma config DSWDTOSC = INTOSCREF //DSWDT uses INTOSC/INTRC as clock
#pragma config RTCOSC = INTOSCREF   //RTCC uses INTRC
#pragma config DSBOREN = OFF        //Zero-Power BOR disabled in Deep Sleep
#pragma config DSWDTEN = OFF        //Disabled
#pragma config DSWDTPS = 8192       //1:8,192 (8.5 seconds)
#pragma config IOL1WAY = OFF        //IOLOCK bit can be set and cleared
#pragma config MSSP7B_EN = MSK7     //7 Bit address masking
#pragma config WPFP = PAGE_1        //Write Protect Program Flash Page 0
#pragma config WPEND = PAGE_0       //Start protection at page 0
#pragma config WPCFG = OFF          //Write/Erase last page protect Disabled
#pragma config WPDIS = OFF          //WPFP[5:0], WPEND, and WPCFG bits ignored  

// BAUDRG is calculated as  = Fosc / (4 * Desired Baudrate) - 1
// So, 4MHz / (4 * 19200) - 1 = 52 (approx.)
//#define BAUDRG 51                   // 19.2Kbps from 4MHz INTOSC
//#define BAUDRG 103                  // 115.2Kbps from 48MHz
//#define BAUDRG 89                   // 115.2Kbps from 41.66MHz
//#define BAUDRG 85                   // 115.2Kbps from 40MHz
//#define BAUDRG 68                   // 115.2Kbps from 32MHz
#define BAUDRG 16                   // 115.2Kbps from 8MHz
//#define BAUDRG 11                   // 1Mbps from 48MHz
//#define BAUDRG 9                    // 1Mbps from 40MHz
//#define BAUDRG 4                    // 2Mbps from 40MHz
//#define BAUDRG 3                    // 3Mbps from 48MHz
//#define BAUDRG 1                    // 6Mbps from 48MHz
#ifndef BAUDRG
#define BAUDRG brg              // use bootloader autobaud BRG if constant not provided
#endif
//#define INVERT_UART                 // If you don't have an RS232 transceiver, you might want this option

void PrintTime(void);
void high_isr(void);
void low_isr(void);

rtccTimeDate d;

unsigned int brg;

void main() {
    OSCTUNEbits.PLLEN = 0; // disable PLL
    OSCCON = 0b01110011; // 8MHz INTOSC

#ifdef INVERT_UART
    LATCbits.LATC6 = 0; // drive TXD pin low for RS-232 IDLE state
#else
    LATCbits.LATC6 = 1; // drive TXD pin high for RS-232 IDLE state
#endif
    TRISCbits.TRISC6 = 0;

    // Enable the Secondary Oscillator for RTCC use.
    //OpenTimer1(TIMER_INT_OFF & T1_SOURCE_PINOSC & T1_PS_1_1 & T1_OSC1EN_ON & T1_SYNC_EXT_OFF, 0);
    //T1CONbits.T1OSCEN = 1;

    // Did we wake up from Deep Sleep? Or is this the first initial power on?
    if (WDTCONbits.DS) {
        // woke up from Deep Sleep
        DSCONLbits.RELEASE = 0; // release control and data bits for all I/Os
    } else if (RCONbits.POR == 0) {
        RCONbits.POR = 1;
//        // first initial power up of the device.
//        // unlock the RTCC registers so that we can write to them
        EECON2 = 0x55;
        EECON2 = 0xAA;
        RTCCFGbits.RTCWREN = 1;

        
        d.f.hour = 0x0A;
        d.f.min = 9;
        d.f.sec = 0x1E;
        d.f.year = 12;
        d.f.mon = 2;
        d.f.mday = 18;
        d.f.wday = 6;

        RtccWriteTimeDate(&d, TRUE);

        RTCCFGbits.RTCEN = 1; // enable RTCC module

        //RTCCFGbits.RTCOE = 1;
    }
    // PADCFG1 = 0b100; // enable source clock as output on RTCC pin (RB1)

    IPR1bits.RC1IP = 0; // USART RX interrupt as low priority interrupt
    RCONbits.IPEN = 1; // enable interrupt priority
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;

    Open1USART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_EIGHT_BIT & USART_ASYNCH_MODE & USART_ADDEN_OFF, BAUDRG);
#ifdef INVERT_UART
    baud1USART(BAUD_IDLE_TX_PIN_STATE_LOW & BAUD_IDLE_RX_PIN_STATE_LOW & BAUD_AUTO_OFF & BAUD_WAKEUP_OFF & BAUD_16_BIT_RATE);
#else
    baud1USART(BAUD_IDLE_TX_PIN_STATE_HIGH & BAUD_IDLE_RX_PIN_STATE_HIGH & BAUD_AUTO_OFF & BAUD_WAKEUP_OFF & BAUD_16_BIT_RATE);
#endif

    printf((far rom char*) "\r\nConnect scope to RB1 for 32.768KHz RTC clock output signal.\r\n");

    // infinite loop, interrupts will handle incoming UART communications.
    while (1) {
        PrintTime();
    }
}

void PrintTime(void) {
    // unsigned char year, yearh, month, day, hour, min, sec;
    while (RTCCFGbits.RTCSYNC != 1); // if already in the middle of SYNC, wait for next period
    while (RTCCFGbits.RTCSYNC == 1); // wait while RTCC registers are unsafe to read
    RtccReadTimeDate(&d);
    printf("\r20%02d/%02d/%02d %02X:%02X:%02X", d.f.year, d.f.mon, d.f.mday, d.f.hour, d.f.min, d.f.sec);
}