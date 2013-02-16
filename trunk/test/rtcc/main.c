/************************************************************************
 *
 * Deep Sleep with INT0 wake up and RTCC operation during power down.
 *
 * This code example shows how to enter/exit the Deep Sleep extreme low
 * power mode available on PIC18F46J11/50 devices. It enables the INT0
 * wakeup source, allowing the user to wake up the device with the push
 * of a button. It provides a serial user interface on UART1 pins. The
 * hardware Real Time Clock Calendar can be setup to continue keeping
 * time, even while the processor is powered down into Deep Sleep.
 *
 * Further information about getting started with Deep Sleep is
 * available in the form of a Web Seminar titled:
 * "Deep Sleep Mode on Microchip PIC18 and PIC24 microcontrollers"
 *
 * Microchip Web Seminars are available at:
 * http://www.microchip.com/webinar/
 *
 * Minimal hardware is required, but the code is configured to work
 * immediately using the PICDEM PIC18 Explorer demo board and a
 * PIC18F46J11 PIM. The PICDEM PIC18 Explorer demo board provides
 * easy access to serial data using RS232 or USB transceivers and
 * a push button for the RB0/INT0 signal.
 *
 * I/O Pins Used:
 * 	RB0/INT0 - INT0 wake up input (attach 15K pull up)
 *   RC7/RX1 - 19.2Kbps UART Receive
 *   RC6/TX1 - 19.2Kbps UART Transmit
 *
 * All other I/O pins, aside from pins that may be driven by PIC18
 * Explorer circuitry, are configured as outputs driven low to minimize
 * power consumption. Pins left floating can cause unnecessary current
 * draw, so always pull up/down unused I/O pins in your application.
 *
 *
 *************************************************************************
 * FileName:     DeepSleep.c
 * Dependencies: stdio.h (comes with MCC18)
 *               usart.h (comes with MCC18)
 *               timers.h (comes with MCC18)
 * Compiler:     MPLAB C18, v3.30 or higher
 * Company:      Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright © 2009 Microchip Technology Inc. All rights reserved.
 *
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).
 *
 * You should refer to the license agreement accompanying this Software for
 * additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A
 * PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE
 * LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY,
 * CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY
 * DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY
 * INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST
 * PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY,
 * SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO
 * ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *
 * Author        Date        Comment
 *************************************************************************
 * E. Schlunder  2009/04/07  Version 1.0.0 - Initial Release
 ************************************************************************/

#include <p18cxxx.h>
#include <stdio.h>
#include <usart.h>
#include <timers.h>

// -------------------------------------------------------------------------------
// BAUDRG is calculated as  = Fosc / (4 * Desired Baudrate) - 1
// So, 4MHz / (4 * 19200) - 1 = 52 (approx.)
#define BAUDRG 25					// 19.2Kbps from 4MHz INTOSC

// If you do not have an RS232 transceiver, you might find this line useful:
//#define INVERT_UART
// Warning: If you do not have an RS232 voltage level converter, connecting to
// a PC serial port may apply voltages higher than the absolute maximum ratings
// of your PIC's I/O pins and cause permanent damage to the PIC!

// Uncomment this line if an external 32.768KHz crystal is available for use as
// the T1 Oscillator (this is true on PICDEM PIC18 Explorer boards).
//#define USE_32KHZ_CRYSTAL

// If your RTCC clock source is too fast or too slow, you can adjust the
// RTCC calibration value to add or subtract extra RTC clock pulses each minute
#define RTCCALIBRATION 0
// -------------------------------------------------------------------------------

// Set up the Configuration words
#if defined (__18F46J11) || defined (__18F46J50)
#ifdef USE_32KHZ_CRYSTAL
#pragma config RTCOSC   = T1OSCREF
#else
#pragma config RTCOSC   = INTOSCREF
#endif

#pragma config WDTEN = OFF          //WDT disabled (enabled by SWDTEN bit)
#pragma config STVREN = ON          //stack overflow/underflow reset enabled
#pragma config XINST = OFF           //Extended instruction set disabled
#pragma config CPUDIV = OSC4_PLL6   //CPU system clock divide 6
#pragma config CP0 = OFF            //Program memory is not code-protected
#pragma config OSC = INTOSCPLL      //Internal OSC(8mhz), PLL enabled by soft
#pragma config PLLDIV = 2           //Divide By 2(4 Mhz) to generate 96Mhz
#pragma config FCMEN = OFF          //Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF           //Two-Speed Start-up disabled
#pragma config WDTPS = 32768        //1:32768
#pragma config DSWDTOSC = INTOSCREF //DSWDT uses INTOSC/INTRC as clock

#pragma config DSBOREN = OFF        //Zero-Power BOR disabled in Deep Sleep
#pragma config DSWDTEN = OFF        //Disabled
#pragma config DSWDTPS = 8192       //1:8,192 (8.5 seconds)
#pragma config IOL1WAY = OFF        //IOLOCK bit can be set and cleared
#pragma config MSSP7B_EN = MSK7     //7 Bit address masking
#pragma config WPFP = PAGE_1        //Write Protect Program Flash Page 0
#pragma config WPEND = PAGE_0       //Start protection at page 0
#pragma config WPCFG = OFF          //Write/Erase last page protect Disabled
#pragma config WPDIS = OFF          //WPFP[5:0], WPEND, and WPCFG bits ignored
#else
#error Please specify configuration bits for the selected device.
#endif

// These are defined later in this .c source file
void InitializeIO(void);
void DoApplicationTasks(void);
void Flush1USART(void);
void EnterDeepSleep(void);
void EnableINT0(void);
void EnableSecondaryOscillator();
void EnableRTCCAlarmEverySecond(void);
void DisableRTCCAlarm(void);
void RB0ReleasedWait(void);
void PrintMenu(void);
void PrintRTCC(void);
void SetDate(void);
void SetTime(void);
void wait(void);

void main(void) {
    // Enable UART for serial output display
    Open1USART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_EIGHT_BIT & USART_ASYNCH_MODE & USART_ADDEN_OFF, BAUDRG);

#ifdef INVERT_UART
    baud1USART(BAUD_IDLE_TX_PIN_STATE_LOW & BAUD_IDLE_RX_PIN_STATE_LOW & BAUD_AUTO_OFF & BAUD_WAKEUP_OFF & BAUD_16_BIT_RATE & USART_RX_INT_ON);
#else
    baud1USART(BAUD_IDLE_TX_PIN_STATE_HIGH & BAUD_IDLE_RX_PIN_STATE_HIGH & BAUD_AUTO_OFF & BAUD_WAKEUP_OFF & BAUD_16_BIT_RATE & USART_RX_INT_ON);
#endif

    // I/O states will be held until DSCONL.RELEASE = 0, but we must still initialize
    // to what we want before clearing the RELEASE bit.
    InitializeIO();

#ifdef USE_32KHZ_CRYSTAL
    // Enable the Secondary Oscillator for RTCC use.
    OpenTimer1(TIMER_INT_OFF & T1_SOURCE_PINOSC & T1_PS_1_1 & T1_OSC1EN_ON & T1_SYNC_EXT_OFF, 0);
#endif

    // Did we wake up from Deep Sleep? Or is this the first initial power on?
    if (WDTCONbits.DS) {
        // woke up from Deep Sleep
        DSCONLbits.RELEASE = 0; // release control and data bits for all I/Os

        wait(); // allow time for INTOSC to stablize before using UART
        printf("Woke from Deep Sleep via ");
        if (DSWAKEHbits.DSINT0 != 0) {
            printf("RB0/INT0\r\n");
        } else if (DSWAKELbits.DSMCLR != 0) {
            printf("MCLR\r\n");
        } else if (DSWAKELbits.DSFLT != 0) {
            printf("DSFLT\n");
        } else if (DSWAKELbits.DSWDT != 0) {
            printf("DSWDT\n");
        } else if (DSWAKELbits.DSRTC != 0) {
            printf("RTCC Alarm\n");
        }
    } else {
        // first initial power up of the device.
        // unlock the RTCC registers so that we can write to them
        EECON2 = 0x55;
        EECON2 = 0xAA;
        RTCCFGbits.RTCWREN = 1;

        // reset RTCC date/time (only on first power on)
        RTCCFGbits.RTCPTR1 = 1;
        RTCCFGbits.RTCPTR0 = 1;
        RTCVALL = 0x09; // reserved | year
        RTCVALH = 0xFF;

        RTCVALL = 0x01; // month | day
        RTCVALH = 0x01;

        RTCVALL = 0x12; // weekday | hours
        RTCVALH = 0x01;

        RTCVALL = 0x00; // minutes | seconds
        RTCVALH = 0x00;

        RTCCFGbits.RTCEN = 1; // enable RTCC module
        RTCCAL = RTCCALIBRATION;

        wait(); // allow time for INTOSC to stablize before using UART
    }

    // Application Tasks
    DoApplicationTasks();

    // Power down into Deep Sleep mode.
    printf("Powering down into deep sleep, push RB0/INT0 down to wake...\r\n");
    Flush1USART();
    Close1USART();
    RB0ReleasedWait(); // wait for user to release INT0 so we don't get spurious wake up

EnterDeepSleepMode:
    EnableINT0();
    EnterDeepSleep();

    // Execution should normally never reach here, as Deep Sleep powers up
    // at the Reset Vector.
    //
    // However, there exists a small possibility an INT0 wake up is triggered
    // before the processor has fully enter Deep Sleep mode. As a result, gaurd
    // code is required here to handle re-initiating Deep Sleep entry.
    goto EnterDeepSleepMode;
}

/************************************************************************
DoApplicationTasks

This is the main application code that executes upon each power on and
wake up. It handles displaying a menu of choices while updating the
current date/time display on the serial port. If the user presses the
RB0/INT0 button, the application will exit and Deep Sleep mode will be
entered by the main() function above.

Parameters:     None
Return:         None
Side Effects:   None
 ************************************************************************/
void DoApplicationTasks(void) {
    unsigned char exit = 0;

    PrintMenu();

    LATDbits.LATD7 = 1; // turn on LED
    PIE1bits.RC1IE = 1; // enable USART receive interrupt (for Sleep wakeup)
    RB0ReleasedWait();

    // Enter IDLE mode on Sleep() instruction. Idle mode is used here so
    // that the Timer 0 and UART can continue operating while the
    // processor is powered down.
    OSCCONbits.IDLEN = 1;
    while (exit == 0) {
        // wait for user to make a selection
        EnableINT0();
        EnableRTCCAlarmEverySecond();
        RCSTA1bits.CREN = 0; // reset UART continuous receive to avoid overrun state
        RCSTA1bits.CREN = 1;
        Sleep();

        while (DataRdy1USART()) {
            switch (Read1USART()) {
                case '1':
                    RTCCFGbits.RTCEN ^= 1;
                    PrintMenu();
                    break;

                case '2':
                    SetDate();
                    PrintMenu();
                    break;

                case '3':
                    SetTime();
                    PrintMenu();
                    break;

                case '4':
                    exit = 1;
                    break;
            }
        }

        if (RTCCFGbits.RTCEN) {
            PrintRTCC();
        }

        if (INTCONbits.INT0IF) {
            printf("\r\nRB0/INT0 power down requested.");
            exit = 1;
        }
    }

    DisableRTCCAlarm();
    printf("\r\n");
    LATDbits.LATD7 = 0; // turn off LED
}

/************************************************************************
InitializeIO

This routine initializes the I/O pin states. Unused I/O pins that would
be left floating are configured as outputs driven low (to save power).
Floating pins can cause unwanted current draw if the floating pin
voltage hovers around the input buffer switching threshold.

Parameters:     None
Return:         None
Side Effects:   None
 ************************************************************************/
void InitializeIO(void) {
    // tristate RA5 - has 15K pull up on PIC18 Explorer, connected to GND through S2 button
    // tristate RA1 - MCP9701A temperature sensor output on PIC18 Explorer
    // tristate RA0 - R3 POT on PIC18 Explorer
    LATA = 0;
    TRISA = (1 << 5) | (1 << 1);

    // tristate RB0/INT0
    // tristate RB1 - PIC18 Explorer serial CTS
    LATB = 0;
    TRISB = 1 | (1 << 1);

    // By default, all analog pins are configured to disable digital input
    // functions. We need RB0/AN12/INT0 configured to allow digital input for
    // the INT0 signal.
    ANCON1bits.PCFG12 = 1; // configure RB0/INT0/AN12 as digital input

    // tristate RC4 - 25LC256 SPI EEPROM output pin
    // tristate RC7/RX1 - Serial input pin
    LATC = 0;
    TRISC = (1 << 4) | (1 << 7);

    LATD = 0;
    TRISD = 0;

    LATE = 0;
    TRISE = 0;
}

/************************************************************************
EnterDeepSleep

Prepares microcontroller registers for entering Deep Sleep mode and
then powers down into Deep Sleep. Ordinarily, this function will never
return. Instead, the microcontroller will power back on at the reset
vector when woken from Deep Sleep.

There is one exception to that, however. If a wake up event is
triggered just before Deep Sleep mode is fully entered, the processor
may abort Deep Sleep entry. In that case, this function will return,
and calling code should therefore be written to handle such a situation.

Parameters:     None
Return:         None
Side Effects:   None
 ************************************************************************/
void EnterDeepSleep(void) {
    // Need to clear Deep Sleep wake source flags prior to
    // entering Deep Sleep so that we can determine which wakeup
    // source woke us up afterward.
    DSWAKEL = 0;
    DSWAKEH = 0;

    // disable IDLE mode on Sleep() instruction
    OSCCONbits.IDLEN = 0;

    // Note: these two statements must be executed back-to-back for Deep Sleep
    // to work.
    DSCONHbits.DSEN = 1; // set the deep sleep enable bit
    Sleep(); // enter deep sleep
}

/************************************************************************
SetRTCCTime

Sets the hardware Real Time Clock Calendar (RTCC) registers to the
provided time.

Parameters:     hour
                A Binary Coded Decimal (BCD) value specifying the
                current hour.

                min
                BCD value specifying current minute.

                sec
                BCD value specifying the current second.

Return:         None
Side Effects:   RTCPTR will be modified and not restored.
 ************************************************************************/
void SetRTCCTime(unsigned char hour, unsigned char min, unsigned char sec) {
    unsigned char byte;

    RTCCFGbits.RTCPTR1 = 0;
    RTCCFGbits.RTCPTR0 = 1;
    RTCVALL = hour;
    byte = RTCVALH; // dummy read of RTCVALH to auto-decrement RTCPTR
    RTCVALL = sec;
    RTCVALH = min;
}

/************************************************************************
SetRTCCDate

Sets the hardware Real Time Clock Calendar (RTCC) registers to the
provided date.

Parameters:     year
                A Binary Coded Decimal (BCD) value specifying the
                current year (two digits only)

                month
                BCD value specifying current month.

                day
                BCD value specifying the current day of the month.

Return:         None
Side Effects:   RTCPTR will be modified and not restored.
 ************************************************************************/
void SetRTCCDate(unsigned char year, unsigned char month, unsigned char day) {
    unsigned char byte;

    RTCCFGbits.RTCPTR1 = 1;
    RTCCFGbits.RTCPTR0 = 1;
    RTCVALL = year;
    byte = RTCVALH; // dummy read of RTCVALH to auto-decrement RTCPTR
    RTCVALL = day;
    RTCVALH = month;
}

/************************************************************************
ReadBCD

Reads a two digit number from the serial port and stores it in
Binary Coded Decimal (BCD) formatted byte return value.

Parameters:     None
Return:         Two digit number entered from UART, stored in BCD form.
Side Effects:   None
 ************************************************************************/
unsigned char ReadBCD(void) {
    unsigned char byte = 0;
    unsigned char bcd;

    while (byte < '0' || byte > '9') {
        while (DataRdy1USART() == 0);
        byte = Read1USART();
    }
    Write1USART(byte);
    byte -= '0';
    bcd = byte << 4;

    while (byte < '0' || byte > '9') {
        while (DataRdy1USART() == 0);
        byte = Read1USART();
    }
    Write1USART(byte);
    byte -= '0';
    bcd |= byte;

    return bcd;
}

/************************************************************************
SetTime

Reads time entered from UART and sets the RTCC to the new time.

Parameters:     None
Return:         None
Side Effects:   RTCC time modified.
 ************************************************************************/
void SetTime(void) {
    unsigned char hour, min, sec;

    printf("\r\nEnter Time (hh:mm:ss): ");

    hour = ReadBCD();
    Write1USART(':');

    min = ReadBCD();
    Write1USART(':');

    sec = ReadBCD();
    printf("\r\n");

    SetRTCCTime(hour, min, sec);
}

/************************************************************************
SetDate

Reads a date entered from UART and sets the RTCC to the new date.

Parameters:     None
Return:         None
Side Effects:   RTCC date modified.
 ************************************************************************/
void SetDate(void) {
    unsigned char year, month, day;

    printf("\r\nEnter Date (yy/mm/dd): ");

    year = ReadBCD();
    Write1USART('/');

    month = ReadBCD();
    Write1USART('/');

    day = ReadBCD();
    printf("\r\n");

    SetRTCCDate(year, month, day);
}

/************************************************************************
PrintRTCC

Prints out the current date and time to the UART, as reported by the
hardware RTCC. Date/time is formatted as YYYY/MM/DD HH:MM:SS with the
upper two digits of the year (YYYY) hard coded to 20.

Parameters:     None
Return:         None
Side Effects:   RTCPTR modified and not restored.
 ************************************************************************/
void PrintRTCC(void) {
    unsigned char year, yearh, month, day, hour, min, sec;

    RTCCFGbits.RTCPTR1 = 1;
    RTCCFGbits.RTCPTR0 = 1;

    while (RTCCFGbits.RTCSYNC != 1); // if already in the middle of SYNC, wait for next period
    while (RTCCFGbits.RTCSYNC == 1); // wait while RTCC registers are unsafe to read

    year = RTCVALL;
    day = RTCVALH; // dummy read to decrement RTCPTR
    day = RTCVALL;
    month = RTCVALH;
    hour = RTCVALL;
    sec = RTCVALH; // dummy read to decrement RTCPTR
    sec = RTCVALL;
    min = RTCVALH;

    printf("\r20%02X/%02X/%02X %02X:%02X:%02X", year, month, day, hour, min, sec);
}

/************************************************************************
EnableRTCCAlarmEverySecond

Enables the RTCC alarm interrupt to go off every time the current
date/time has changed. This is used to wake up from IDLE mode to
display an updated date/time on the serial display.

Parameters:     None
Return:         None
Side Effects:   None
 ************************************************************************/
void EnableRTCCAlarmEverySecond(void) {
    PIR3bits.RTCCIF = 0;
    PIE3bits.RTCCIE = 1;
    ALRMCFG = 0b11000000;
}

/************************************************************************
DisableRTCCAlarm

Turns off the RTCC alarm interrupt so that we don't wake up from
Deep Sleep.

Parameters:     None
Return:         None
Side Effects:   None
 ************************************************************************/
void DisableRTCCAlarm(void) {
    ALRMCFG = 0;
    PIR3bits.RTCCIF = 0;
    PIE3bits.RTCCIE = 0;
}

/************************************************************************
PrintMenu

Displays a list of menu choices for the user on the serial port.

Parameters:     None
Return:         None
Side Effects:   None
 ************************************************************************/
void PrintMenu(void) {
    printf("\r\nSelect task:\r\n");
    if (RTCCFGbits.RTCEN) {
        printf("1. Stop RTCC\r\n");
    } else {
        printf("1. Enable RTCC\r\n");
    }

    printf("2. Set date\r\n");
    printf("3. Set time\r\n");
    printf("4. Power down into Deep Sleep\r\n");
}

/************************************************************************
RB0ReleasedWait

Waits until the RB0/INT0 pin is no longer being pulled down. Uses Timer0
to filter out switch bounce.

Parameters:     None
Return:         None
Side Effects:   Timer0 will be used and left disabled by this routine.
 ************************************************************************/
void RB0ReleasedWait(void) {
    OpenTimer0(TIMER_INT_OFF & T0_8BIT & T0_SOURCE_INT & T0_PS_1_8);
    while (INTCONbits.T0IF == 0) {
        // wait for user to release RB0/INT0
        if (PORTBbits.RB0 == 0) {
            TMR0L = 0; // reset timer if button is bouncing.
        }
    }
    CloseTimer0();
}

/************************************************************************
EnableINT0

Sets up INT0 to interrupt on falling edge. Clears the INT0 interrupt
flag and enables the interrupt.

Note: In order to use the RB0 pin for INT0, the analog input function
must be disabled through the ANCONx registers. This is already done
in the InitializeIO function.

Parameters:     None
Return:         None
Side Effects:   None
 ************************************************************************/
void EnableINT0(void) {
    INTCON2bits.INTEDG0 = 0; // 0 - interrupt on falling edge
    INTCONbits.INT0IF = 0; // clear INT0 interrupt flag
    INTCONbits.INT0IE = 1; // enable INT0 interrupt
}

/************************************************************************
Flush1USART

Waits until the UART is fully done with transmitting data in the TXREG1
register. This may be necessary to avoid corrupting the last byte of
data being transferred by the UART before shutting down the UART and
entering Deep Sleep mode.

Parameters:     None
Return:         None
Side Effects:   None
 ************************************************************************/
void Flush1USART(void) {
    int i;
    for (i = 0; i < 10000; i++) {
        if (Busy1USART() == 0) {
            break;
        }
    }
}

/************************************************************************
wait

Waits for a period time. Used to avoid sending data out of the UART
before the internal oscillator has stablized on start up.

Parameters:     None
Return:         None
Side Effects:   Timer 0 will be used and turned off. OSCCON.IDLEN will
                be set to 1.
 ************************************************************************/
void wait(void) {
    unsigned int i;

    OpenTimer0(TIMER_INT_ON & T0_8BIT & T0_SOURCE_INT & T0_PS_1_4);

    // Enter IDLE mode. Idle mode is used here so that the Timer 0
    // can continue operating while the processor is powered down.
    OSCCONbits.IDLEN = 1;
    Sleep();

    CloseTimer0();
}