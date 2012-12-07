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
 
/*...........................................................................*/
/* Pragmas */
/* Watchdog*/
#pragma config WDTEN = OFF          //WDT disabled (enabled by SWDTEN bit)
#pragma config WDTPS = 1024         //1:1024 (4 seconds)
#pragma config DSWDTOSC = INTOSCREF //DSWDT uses INTOSC/INTRC as clock
#pragma config DSWDTEN = ON         //Enabled
#pragma config DSWDTPS = K524       //9 minutes

#pragma config STVREN = ON          //stack overflow/underflow reset enabled
#pragma config XINST = OFF          //Extended instruction set disabled
#pragma config CP0 = OFF            //Program memory is not code-protected

#pragma config OSC = INTOSCPLL      //Internal OSC(8mhz), PLL enabled by soft
#pragma config PLLDIV = 2           //Divide By 2(4 Mhz) to generate 96Mhz
#pragma config CPUDIV = OSC4_PLL6   //CPU system clock divide 6

#pragma config FCMEN = OFF          //Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF           //Two-Speed Start-up disabled

#pragma config RTCOSC = INTOSCREF   //RTCC uses INTRC
#pragma config DSBOREN = OFF        //Zero-Power BOR disabled in Deep Sleep

#pragma config IOL1WAY = OFF        //IOLOCK bit can be set and cleared
#pragma config MSSP7B_EN = MSK7     //7 Bit address masking
#pragma config WPFP = PAGE_1        //Write Protect Program Flash Page 0
#pragma config WPEND = PAGE_0       //Start protection at page 0
#pragma config WPCFG = OFF          //Write/Erase last page protect Disabled
#pragma config WPDIS = OFF          //WPFP[5:0], WPEND, and WPCFG bits ignored  
