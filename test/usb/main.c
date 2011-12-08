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

#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "usb_config.h"
#include "usb_device.h"
#include "usb_profile.h"
#include "usb.h"
#include "usb_callback.h"

#pragma config WDTEN = OFF          //WDT disabled (enabled by SWDTEN bit)
#pragma config PLLDIV = 3           //Divide by 3 (12 MHz oscillator input)
#pragma config STVREN = ON            //stack overflow/underflow reset enabled
#pragma config XINST = OFF          //Extended instruction set disabled
#pragma config CPUDIV = OSC1        //No CPU system clock divide
#pragma config CP0 = OFF            //Program memory is not code-protected
#pragma config OSC = HSPLL          //HS oscillator, PLL enabled, HSPLL used by USB
//#pragma config T1DIG = ON           //Sec Osc clock source may be selected
//#pragma config LPT1OSC = OFF        //high power Timer1 mode
#pragma config FCMEN = OFF          //Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF           //Two-Speed Start-up disabled
#pragma config WDTPS = 32768        //1:32768
#pragma config DSWDTOSC = INTOSCREF //DSWDT uses INTOSC/INTRC as clock
#pragma config RTCOSC = T1OSCREF    //RTCC uses T1OSC/T1CKI as clock
#pragma config DSBOREN = OFF        //Zero-Power BOR disabled in Deep Sleep
#pragma config DSWDTEN = OFF        //Disabled
#pragma config DSWDTPS = 8192       //1:8,192 (8.5 seconds)
#pragma config IOL1WAY = OFF        //IOLOCK bit can be set and cleared
#pragma config MSSP7B_EN = MSK7     //7 Bit address masking
#pragma config WPFP = PAGE_1        //Write Protect Program Flash Page 0
#pragma config WPEND = PAGE_0       //Start protection at page 0
#pragma config WPCFG = OFF          //Write/Erase last page protect Disabled
#pragma config WPDIS = OFF          //WPFP[5:0], WPEND, and WPCFG bits ignored

unsigned int pll_startup_counter = 600;
char reg;

//These are your actual interrupt handling routines.
#pragma interrupt YourHighPriorityISRCode

void YourHighPriorityISRCode() {
	//Check which interrupt flag caused the interrupt.
	//Service the interrupt
	//Clear the interrupt flag
	//Etc.
#if defined(USB_INTERRUPT)
	if (USB_BUS_SENSE && INTCONbits.RBIF) {
		reg = PORTB;
		_asm nop _endasm
		INTCONbits.RBIF = 0;
	}
#endif

} //This return will be a "retfie fast", since this is in a #pragma interrupt section
#pragma interruptlow YourLowPriorityISRCode

void YourLowPriorityISRCode() {
	//Check which interrupt flag caused the interrupt.
	//Service the interrupt
	//Clear the interrupt flag
	//Etc.

} //This return will be a "retfie", since this is in a #pragma interruptlow section

#pragma code HIGH_INTERRUPT_VECTOR = 0x08

void High_ISR(void) {
	_asm goto YourHighPriorityISRCode _endasm
}
#pragma code LOW_INTERRUPT_VECTOR = 0x18

void Low_ISR(void) {
	_asm goto YourLowPriorityISRCode _endasm
}

void main() {
	ANCON0 = 0xFF; // Default all pins to digital
	ANCON1 = 0xFF; // Default all pins to digital

	OSCTUNEbits.PLLEN = 1; //Enable the PLL and wait 2+ms until the PLL locks before enabling USB module
	while (pll_startup_counter--);

	USBDeviceInit();
	TRISEbits.TRISE0 = 0;
	LATEbits.LATE0 = 0;
	TRISBbits.TRISB4 = 1;


	INTCONbits.RBIE = 1;
	INTCON2bits.RBIP = 1;
	RCONbits.IPEN = 1;
	INTCONbits.GIEH = 1;
	INTCONbits.RBIF = 0;

	while (1) {
		OSCCONbits.IDLEN = 0;
		Sleep();
		Nop();
		if (USBGetDeviceState() == DETACHED_STATE ) {
			LATEbits.LATE0 = 1;
			USBDeviceAttach();
			//USBDeviceTasks();
		} else if (USBGetDeviceState() == ATTACHED_STATE) {
			LATEbits.LATE0 = 0;
			USBDeviceDetach();
		}
	}//end while
}