/**********************************************************************
* 2009 Microchip Technology Inc.
*
* FileName:        	deep_sleep.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       	PIC18F
* Compiler:        	MCC18 v3.30 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all
* ownership and intellectual property rights in the code accompanying
* this message and in all derivatives hereto.  You may use this code,
* and any derivatives created by any person or entity by or on your
* behalf, exclusively with Microchip's proprietary products.  Your
* acceptance and/or use of this code constitutes agreement to the
* terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS". NO
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
* NOT LIMITED TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT,
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS
* CODE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS, COMBINATION WITH
* ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE
* LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR
* BREACH OF STATUTORY DUTY), STRICT LIABILITY, INDEMNITY,
* CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR
* EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER
* CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE
* DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW,
* MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
* CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP
* SPECIFICALLY TO HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and
* determining its suitability.  Microchip has no obligation to modify,
* test, certify, or support the code.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author        	Date      	Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Harsha.J.M	04/04/09	First release of source file
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* Code uses the Peripheral library support available with MCC18 Compiler
* Code Tested on:
* PIC18F46J50 controller
* The Processor starts with the External Crystal (8 Mhz).
*
*
* Hardware Implementation detalis
*
*----- Switch is connected to PORTA<3> pin which initiates sequence to put the device into deep sleep upon switch pressing
*-----  PORTB<0> is connected to LED used for source of inidication
*
*	LED Indication			Source of Wake up from deep sleep
*
*		1 blink	------------>		Power on RESET
*		2 blinks	------------>		MCLR (active low signal on master Reset pin)
*		3 blinks	------------>		RTCC (Real time clock calender module)
*		4 blinks	------------>		DSWDT (Deep Sleep watch dog timer)
*		5 blinks	------------>		Fault in configuring Deep Sleep
*		6 blinks	------------>		Interrupt0 (INT0)
*		7  blinks	------------>		Brown out reset (BOR)
*		8  blinks	------------>		Ultra Low Power Wake UP (ULPWK)
*
**********************************************************************/
/****************************************************************
*  To set CONFIG4 using the macro, the following line
*  can be pasted only at the beginning source code,
*  immediately below the '#include' directive.
*
** The following constants can be used to set CONFIG3L.
**
**
;   DSWDT Clock Select:
;     DSWDTOSC = T1OSCREF  DSWDT uses T1OSC/T1CKI
;     DSWDTOSC = INTOSCREF DSWDT uses INTRC
;
;   RTCC Clock Select:
;     RTCOSC = INTOSCREF   RTCC uses INTRC
;     RTCOSC = T1OSCREF    RTCC uses T1OSC/T1CKI
;
;   Deep Sleep BOR:
;     DSBOREN = OFF        Disabled
;     DSBOREN = ON         Enabled
;
;   Deep Sleep Watchdog Timer:
;     DSWDTEN = OFF        Disabled
;     DSWDTEN = ON         Enabled
;
;   Deep Sleep Watchdog Postscaler :
;     DSWDTPS = 2          1:2 (2.1 ms)
;     DSWDTPS = 8          1:8 (8.3 ms)
;     DSWDTPS = 32         1:32 (33 ms)
;     DSWDTPS = 128        1:128 (132 ms)
;     DSWDTPS = 512        1:512 (528 ms)
;     DSWDTPS = 2048       1:2,048 (2.1 seconds)
;     DSWDTPS = 8192       1:8,192 (8.5 seconds)
;     DSWDTPS = K32        1:32,768 (34 seconds)
;     DSWDTPS = K131       1:131,072 (135 seconds)
;     DSWDTPS = K524       1:524,288 (9 minutes)
;     DSWDTPS = M2         1:2,097,152 (36 minutes)
;     DSWDTPS = M8         1:8,388,608 (2.4 hours)
;     DSWDTPS = M33        1:33,554,432 (9.6 hours)
;     DSWDTPS = M134       1:134,217,728 (38.5 hours)
;     DSWDTPS = M536       1:536,870,912 (6.4 days)
;     DSWDTPS = G2         1:2,147,483,648 (25.7 days)
;
;   T1OSCEN Enforcement:
;     T1DIG = OFF          Secondary Oscillator clock source may not be selected
;     T1DIG = ON           Secondary Oscillator clock source may be selected
;
;   Low-Power Timer1 Oscillator    :
;     LPT1OSC = ON         Low-power operation
;     LPT1OSC = OFF        High-power operation

	* Oscillator is configured as HS
	* Fail safe monitor is enabled
	* watch dog timer is disabled
	* Extended instruction mode is disabled
	* oscillator switch over is enabled
	* CPU clock is not devided
*****************************************************************/

#define USE_OR_MASKS
#include "p18cxxx.h"
#include "dpslp.h"
#include "portb.h"

#define TRUE 1

//************Configuration in program*****************************

#if defined(__18F46J50)		//If the selected device if PIC18F46J50, then apply below settings else user will have to set
#pragma config DSWDTOSC = INTOSCREF, RTCOSC = INTOSCREF, DSBOREN = ON,  DSWDTEN = ON, DSWDTPS = 8192
#pragma config T1DIG = ON, LPT1OSC = OFF
#pragma config OSC=HS, FCMEN=ON, WDTEN=OFF, IESO=ON, XINST=OFF, CPUDIV=OSC1
#endif


//Function Prototypes
void user_main(void);
unsigned char GetKey(void);
void DebounceDelay(void);
void SwitchInit(void);
void Blink_LED(unsigned int number_of_blink);

//Global structures used in deep sleep library

SRC ptr;
CONTEXT read_state;

//main function
void main(void)
{
	if(IsResetFromDeepSleep()==0xFF) //if this is the reset after the deep_sleep wakup...then do this
		{
		ReadDSGPR( &read_state);		//Read the deep sleep GPR
		DeepSleepWakeUpSource(&ptr);	//Check the deep sleep wakup soruce (if required)
		ReleaseDeepSleep();				//Release the Deep sleep (IO configuration)

		TRISE = 0x01;						//configure the IO [TRIS and LAT register] to output ssignal for LED
		SwitchInit();					//This function configures Switch IO as input pin
		if( (read_state.Reg0!=0x67) || (read_state.Reg1!=0x7A) )		//0x67 & 0x7A represents the state of device before going to deep sleep
			{
			while(1)
				{
				Blink_LED(150);	//Continuously Toggle LED to indicate the error in GPR reading. State of device corrupted
				}
			}

		if((ptr.WK_SRC.DS_POR)==TRUE)
			{
			//deep sleep wakeup source is DSPOR
			Blink_LED(1);
			}
		if(ptr.WK_SRC.DS_MCLR==TRUE)
			{
			//deep sleep wakeup source is MCLR
			Blink_LED(2);
			}
		if(ptr.WK_SRC.DS_RTC==TRUE)
			{
			//deep sleep wakeup source is RTCC
			Blink_LED(3);
			}
		if(ptr.WK_SRC.DS_WDT==TRUE)
			{
			//deep sleep wakeup source is DSWDT
			Blink_LED(4);
			}
		if(ptr.WK_SRC.DS_FLT==TRUE)
			{
			//deep sleep wakeup source is Falut in deep sleep configuration
			Blink_LED(5);
			}
		if(ptr.WK_SRC.DS_INT0==TRUE)
			{
			//deep sleep wakeup source is INT0
			Blink_LED(6);
			}
		if(ptr.WK_SRC.DS_BOR==TRUE)
			{
			//deep sleep wakeup source is DSBOR
			Blink_LED(7);
			}
		if(ptr.WK_SRC.DS_ULP==TRUE)
			{
			//deep sleep wakeup source is DSULP
			Blink_LED(8);
			}
		user_main();					//call the "user_main" -- The User application program
		}
	else							//else... this is the Normal (pure) Power_on Reset...do the normal init
		{
		SwitchInit();					//This function configures Switch IO as input pin
		user_main();					//call the "user_main" -- The User application program
		}
while(1);
}

void user_main(void)
{
unsigned int config=0,i=0;
char Key;
while(1)
	{
		Key = !GetKey();		//check if Switch is pressed
		if(Key)					//if key pressed then call the deep sleep function
		{
		//Normal POR init program here
		TRISE = 0x01;
		LATE = 0xFE;
		
		Write_DSGPR(0x67,0x7A);		//Save state of system prior to deep sleep

		for(i = 0; i < 10000; i++) Nop();
//**************** configure INT0 with pullups enabled, falling edge *********************************
		config = PORTB_CHANGE_INT_ON | FALLING_EDGE_INT | PORTB_PULLUPS_ON;
	    OpenRB0INT(config );					//configures INT0 & enables it
//*************** configure deep sleep wake up sources ********************************************
		config = ( DPSLP_ULPWU_DISABLE | DPSLP_RTCC_WAKEUP_DISABLE);
		INTCONbits.GIE = 0;
		GotoDeepSleep(config);		//This function puts the device into deep sleep

		}
	}
}

void DebounceDelay(void)
{
	unsigned int i = 0x00FF;
	while(i--);
}

unsigned char GetKey()
{
	if(!PORTAbits.RA5)			//this RA3 should be checked and changes as in the hardware
		return 1; //TRUE;
	else
	{
		while(PORTAbits.RA5);	//this RA3 should be checked and changes as in the hardware
		//DebounceDelay();
		return 0; //FALSE;
	}
}

void SwitchInit(void)
{
	TRISAbits.TRISA5 = 1;	//  PORTA<4> as input, SW2 (on PIC Dem2+  board). This PORTA<4> should be checked and changes as in the hardware
	ANCON0 = 0xFF;			//configure ports to be digital
}

void Blink_LED(unsigned int number_of_blink)
{
	unsigned int m=0,n=0;

// ************ BLINK LED *******************************
	for(n=0;n<number_of_blink;n++)
		{
		LATD = 0x00;
		for(m=0;m<60000;m++);
		LATD = 0xFE;
		for(m=0;m<60000;m++);
		}
}
