#include <p18f26j50.h>

#pragma config WDTEN = OFF          /* Disabled - Controlled by SWDTEN bit                                                       */
#pragma config PLLDIV = 2           /* Divide by 2 (8 MHz oscillator input)                                                      */
#pragma config STVREN = ON          /* Reset on stack overflow/underflow enabled                                                 */
#pragma config XINST = OFF          /* Instruction set extension and Indexed Addressing mode disabled (Legacy mode)              */
#pragma config CPUDIV = OSC1        /* No CPU system clock divide                                                                */
#pragma config CP0 = OFF            /* Program memory is not code-protected                                                      */
#pragma config OSC = INTOSCPLL      /* INTOSC with PLL enabled(S/W control), port function on RA6 and RA7, INTOSCPLL used by USB */
#pragma config T1DIG = OFF          /* Secondary Oscillator clock source may not be selected                                     */
#pragma config LPT1OSC = OFF        /* Timer1 oscillator configured for higher power operation                                   */
#pragma config FCMEN = OFF          /* Fail-Safe Clock Monitor disabled                                                          */
#pragma config IESO = OFF           /* Two-Speed Start-up disabled                                                               */
#pragma config WDTPS = 1            /* 1:1                                                                                       */
#pragma config DSWDTOSC = INTOSCREF /* DSWDT uses INTOSC/INTRC as reference clock                                                */
#pragma config RTCOSC = T1OSCREF    /* RTCC uses T1OSC/T1CKI as reference clock                                                  */
#pragma config DSBOREN = OFF        /* Zero-Power BOR disabled in Deep Sleep (does not affect operation in non-Deep Sleep modes) */
#pragma config DSWDTEN = ON         /* Deep Sleep Watchdog Timer Enabled                                                         */
#pragma config DSWDTPS = 8192       /* 1:8,192 (8.5 seconds)                                                                     */
#pragma config IOL1WAY = OFF        /* The IOLOCK bit (PPSCON<0>) can be set and cleared as needed                               */
#pragma config MSSP7B_EN = MSK7     /* 7 Bit address masking                                                                     */
#pragma config WPFP = PAGE_31      /* Write Protect Program Flash Page 31                                                       */
#pragma config WPEND = PAGE_WPFP    /* Write/Erase protect Flash Memory pages starting at page WPFP[5:0]                         */
#pragma config WPCFG = OFF          /* Write/Erase Protection of last page Disabled                                              */
#pragma config WPDIS = OFF          /* WPFP[5:0], WPEND, and WPCFG bits ignored                                                  */

void InterruptHandlerHigh(void);

void InterruptHandlerLow(void);

/*
 ** High priority interrupt vector
 */

#pragma code InterruptVectorHigh = 0x08

void InterruptVectorHigh(void) {
    _asm goto InterruptHandlerHigh _endasm
}

/*
 ** Low priority interrupt vector
 */
#pragma code InterruptVectorLow=0x18

void InterruptVectorLow(void) {
    _asm goto InterruptHandlerLow _endasm
}

/* return to the default code section */
#pragma code

/*
 **   High priority interrupt handlers
 */

#pragma interrupt InterruptHandlerHigh

void InterruptHandlerHigh(void) {
    /* hang here if we get an unhandled interrupt */
    for (;;);
}

/*
 ** Low priority interrupt handlers
 */
#pragma interruptlow InterruptHandlerLow

void InterruptHandlerLow(void) {
    /* hang here if we get an unhandled interrupt */
    for (;;);
}

/*
 ** Initialize this PIC hardware
 */
void PICInit(void) {
    OSCCON = 0x70; /* Use Primary clock source selected by config bits */
    OSCTUNEbits.PLLEN = 1; /* Use PLL to make Fosc clock run at 48MHz */

    ANCON0 = 0xFF; /* Turn off all analog inputs */
    ANCON1 = 0x1F;
    CM1CON = 0x00;
    CM2CON = 0x00;

    /*  UnLock Registers */
    EECON2 = 0x55;
    EECON2 = 0xAA;
    PPSCONbits.IOLOCK = 0;
    /*  Unlock ends */

    RPINR1 = 0x1F; /* INT1    <= Not mapped */
    RPINR2 = 0x1F; /* INT2    <= Not mapped */
    RPINR3 = 0x1F; /* INT3    <= Not mapped */
    RPINR4 = 0x1F; /* T0CLKI  <= Not mapped */
    RPINR6 = 0x1F; /* T3CKI   <= Not mapped */
    RPINR7 = 0x1F; /* CCP1    <= Not mapped */
    RPINR8 = 0x1F; /* CCP2    <= Not mapped */
    RPINR12 = 0x1F; /* T1G     <= Not mapped */
    RPINR13 = 0x1F; /* T3G     <= Not mapped */
    RPINR16 = 0x1F; /* RX2/DT2 <= Not mapped */
    RPINR17 = 0x1F; /* CK2     <= Not mapped */
    RPINR21 = 0x1F; /* SDI2    <= Not mapped */
    RPINR22 = 0x1F; /* SCK2IN  <= Not mapped */
    RPINR23 = 0x1F; /* SS2IN   <= Not mapped */
    RPINR24 = 0x1F; /* FLT0    <= Not mapped */

    RPOR0 = 0x00; /* unmap all outputs */
    RPOR1 = 0x00;
    RPOR2 = 0x00;
    RPOR3 = 0x00;
    RPOR4 = 0x00;
    RPOR5 = 0x00;
    RPOR6 = 0x00;
    RPOR7 = 0x00;
    RPOR8 = 0x00;
    RPOR9 = 0x00;
    RPOR10 = 0x00;
    RPOR11 = 0x00;
    RPOR12 = 0x00;
    RPOR13 = 0x00;
    RPOR17 = 0x00;
    RPOR18 = 0x00;
#ifdef __18F46J50_H
    RPOR19 = 0x00;
    RPOR20 = 0x00;
    RPOR21 = 0x00;
    RPOR22 = 0x00;
    RPOR23 = 0x00;
    RPOR24 = 0x00;
#endif

    /*  Lock Registers */
    EECON2 = 0x55;
    EECON2 = 0xAA;
    PPSCONbits.IOLOCK = 1;
    /*  Lock Registers ends */

    TRISA = 0xFF; /*RA0-7 are digital inputs */
    TRISB = 0xFF; /*RB0-7 are digital inputs */
    TRISC = 0xFF; /*RC0-7 are digital inputs */
#ifdef __18F46J50_H
    TRISD = 0xFF; /*RD0-7 are digital inputs */
    TRISE = 0xFF; /*RE0-2 are digital inputs */
#endif

    RCONbits.IPEN = 1; /* Enable High/Low interrupt priority model. */

    if (WDTCONbits.DS) {
        /* handle stuff specific to wake up from deep sleep */
        TRISCbits.TRISC6 = 0;
        LATCbits.LATC6 = 0;
#ifdef __18F46J50_H
        /* Make RA0 an out bit */
        TRISE &= ~0x01;
        LATE &= ~0x01;
#endif
    }
    WDTCONbits.DS = 0;

    if (DSCONLbits.RELEASE) {
        TRISCbits.TRISC6 = 0;
        LATC = PORTC;
#ifdef __18F46J50_H
        /* restore LATA state from before deep sleep wakup */
        TRISE &= ~0x01;
        LATE = PORTE;
#endif

        /* clear deep sleep I/O config lock out */
        DSCONLbits.RELEASE = 0;
    } else {
        TRISCbits.TRISC6 = 0;
        LATCbits.LATC6 = 0;
#ifdef __18F46J50_H
        /* Make RA0 an out bit */
        TRISE &= ~0x01;
        LATE &= ~0x01;
#endif
    }
}

void main(void) {
    /* Disable interrupts */
    INTCONbits.GIEH = 0;

    /* Initial this hardware */
    PICInit();

    ClrWdt();
    LATCbits.LATC6 = !LATCbits.LATC6;
#ifdef __18F46J50_H
    /* toggle output bit RA0 on wake from deep sleep */
    LATE ^= 0x01;
#endif

    /* prepare to enter deep sleep */
    INTCONbits.GIEH = 0; /* disable interrupts */
    WDTCONbits.SWDTEN = 1; /* turn on the watch dog timer */
    DSCONLbits.ULPWDIS = 1; /* disable ULP wake up */
    DSCONLbits.DSBOR = 0; /* disable Brownout wake up */
    DSCONHbits.DSULPEN = 0; /* disable ULP pull up */
    DSCONHbits.RTCWDIS = 1; /* disable RTCC wake up */
    WDTCONbits.REGSLP = 1; /* turn off core regulator for deep sleep */
    OSCCONbits.IDLEN = 0; /* enter sleep mode, not idle */

    /* Check the generated code, the SLEEP instruction
     ** must be the next instruction after the DSEN bit
     ** is set in the DSCONH register.
     ** If not you must use inline assembly or call
     ** an assembly function to enter deep sleep
     */
    /* go to deep sleep, DSWDT or reset is the only way out in this demo */
    DSCONHbits.DSEN = 1;
    Sleep();

    /* wait here forever */
    /* do a conventional sleep mode if we did not enter deep sleep */
    for (;;) {
        if (!DSCONHbits.DSEN) {
            Sleep(); /* go to sleep */
            Nop(); /* in general it's best to do nothing for one or */
            Nop(); /* two instruction cycles after a WDT wake up    */
            /* toggle output bit RA0 on wake from sleep */
            LATCbits.LATC6 ^= 0x01;
#ifdef __18F46J50_H
            /* toggle output bit RA0 on wake from sleep */
            LATE ^= 0x01;
#endif
        }
    }
}
