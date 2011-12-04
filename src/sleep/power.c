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

#include "power.h"
#include "bsp.h"

/*..........................................................................*/

/**
 * The PRI_RUN mode is the normal, full-power execution mode of the
 * microcontroller. This is also the default mode upon a device Reset
 * unless Two-Speed Start-up is enabled
 * (see Section 27.4 “Two-Speed Start-up” for details).
 * In this mode, the OSTS bit is set
 * (see Section 3.5.1 “Oscillator Control Register”).
 */
void runPrimaryMode(void) {
	DSCONHbits.DSEN = 0;
	OSCCONbits.SCS = 0;
}

/*..........................................................................*/

/**
 * The SEC_RUN mode is the compatible mode to the “clock switching” feature
 * offered in other PIC18 devices.
 * In this mode, the CPU and peripherals are clocked from the Timer1 oscillator.
 * This gives users the option of low-power consumption while still using a
 * high-accuracy clock source.
 * SEC_RUN mode is entered by setting the SCS<1:0> bits to ‘01’.
 * The device clock source is switched to the Timer1 oscillator (see Figure 4-1),
 * the primary oscillator is shut down, the T1RUN bit (T1CON<6>) is set
 * and the OSTS bit is cleared.
 *
 * NOTE:
 * The Timer1 oscillator should already be running prior to entering SEC_RUN mode.
 * If the T1OSCEN bit is not set when the SCS<1:0> bits are set to ‘01’, entry to
 * SEC_RUN mode will not occur.
 * If the Timer1 oscillator is enabled, but not yet running, device clocks
 * will be delayed until the oscillator has started.
 * In such situations, initial oscillator operation is far from stable
 * and unpredictable operation may result.
 */
void runSecondaryMode(void) {
	DSCONHbits.DSEN = 0;
	OSCCONbits.SCS = 1;
}

/*..........................................................................*/

/**
 * In RC_RUN mode, the CPU and peripherals are clocked from the internal
 * oscillator; the primary clock is shut down.
 * This mode provides the best power conservation of all the Run modes
 * while still executing code.
 * It works well for user applications, which are not highly timing-sensitive
 * or do not require high-speed clocks at all times.
 * This mode is entered by setting the SCS<1:0> bits (OSCCON<1:0>) to ‘11’.
 * When the clock source is switched to the internal oscillator block,
 * the primary oscillator is shutdown and the OSTS bit is cleared.
 */
void runRcMode(void) {
	DSCONHbits.DSEN = 0;
	OSCCONbits.SCS = 3;
}

/*..........................................................................*/

/**
 * This mode is unique among the three low-power Idle modes,
 * in that it does not disable the primary device clock.
 * For timing-sensitive applications, this allows for the fastest resumption
 * of device operation with its more accurate primary clock source,
 * since the clock source does not have to “warm up” or transition from another
 * oscillator.
 * PRI_IDLE mode is entered from PRI_RUN mode by setting the IDLEN bit
 * and executing a SLEEP instruction.
 * If the device is in another Run mode, set IDLEN first, then set the SCS bits
 * to ‘00’ and execute SLEEP.
 * Although the CPU is disabled, the peripherals continue to be clocked
 * from the primary clock source specified by the FOSC<1:0> Configuration bits.
 * The OSTS bit remains set.
 * When a wake event occurs, the CPU is clocked from the primary clock source.
 * After the wake-up, the OSTS bit remains set.
 * The IDLEN and SCS bits are not affected by the wake-up.
 */
void idlePrimaryMode(void) {
	OSCCONbits.IDLEN = 1;
	// If PRI_RUN is not enabled(SCS != '00') then set SCS = 0x00
	if (OSCCONbits.SCS != 0) {
		OSCCONbits.SCS = 0;
	}
	Sleep();
}

/*..........................................................................*/

/**
 * In SEC_IDLE mode, the CPU is disabled but the peripherals continue
 * to be clocked from the Timer1 oscillator.
 * This mode is entered from SEC_RUN by setting the IDLEN bit and executing
 * a SLEEP instruction.
 * If the device is in another Run mode, set IDLEN first,
 * then set SCS<1:0> to ‘01’ and execute SLEEP.
 * When the clock source is switched to the Timer1 oscillator, the
 * primary oscillator is shut down (unless some other peripheral is still
 * requesting it), the OSTS bit is cleared and the T1RUN bit is set.
 * When a wake event occurs, the peripherals continue to be clocked from
 * the Timer1 oscillator. After a wake event, the CPU begins executing code
 * being clocked by the Timer1 oscillator.
 * The IDLEN and SCS bits are not affected by the wake-up;
 * the Timer1 oscillator continues to run.
 *
 * NOTE:
 * The Timer1 oscillator should already be running prior to entering SEC_IDLE
 * mode.
 * If the T1OSCEN bit is not set when the SLEEP instruction is executed, the
 * SLEEP instruction will be ignored and entry to SEC_IDLE mode will not occur.
 * If the Timer1 oscillator is enabled, but not yet running, peripheral clocks
 * will be delayed until the oscillator has started.
 * In such situations, initial oscillator operation is far from stable and
 * unpredictable operation may result.
 */
void idleSecondaryMode(void) {
	// FIXME ¿Should check for timer 1 status?
	OSCCONbits.IDLEN = 1;
	// If SEC_RUN is not enabled(SCS = '01') then set SCS = 0x01
	if (OSCCONbits.SCS != 1) {
		OSCCONbits.SCS = 1;
	}
	Sleep();
}

/*..........................................................................*/

/**
 * In RC_IDLE mode, the CPU is disabled but the peripherals continue to be
 * clocked from the internal oscillator block.
 * This mode allows for controllable power conservation during Idle periods.
 * From RC_RUN, this mode is entered by setting the IDLEN bit and executing
 * a SLEEP instruction.
 * If the device is in another Run mode, first set IDLEN, then clear the SCS
 * bits and execute SLEEP.
 * When the clock source is switched to the INTOSC block, the primary oscillator
 * is shutdown and the OSTS bit is cleared.
 * When a wake event occurs, the peripherals continue to be clocked from the
 * internal oscillator block.
 * After a wake event, the CPU begins executing code being clocked by the INTRC.
 * The IDLEN and SCS bits are not affected by the wake-up.
 * The INTRC source will continue to run if either the WDT or the FSCM is enabled.
 */
void idleRcMode(void) {
	// FIXME ¿Should check for timer 1 status?
	OSCCONbits.IDLEN = 1;
	// If RC_RUN is not enabled(SCS = '11') then set SCS = 0x11
	if (OSCCONbits.SCS != 3) {
		OSCCONbits.SCS = 3;
	}
	Sleep();
}

/*..........................................................................*/

/**
 * The power-managed Sleep mode is identical to the legacy Sleep mode offered
 * in all other PIC devices.
 * It is entered by clearing the IDLEN bit (the default state on device Reset)
 * and executing the SLEEP instruction.
 * This shuts down the selected oscillator.
 * All clock source status bits are cleared.
 * Entering the Sleep mode from any other mode does not require a clock switch.
 * This is because no clocks are needed once the controller has entered Sleep mode.
 * If the WDT is selected, the INTRC source will continue to operate.
 * If the Timer1 oscillator is enabled, it will also continue to run.
 */
void sleep(void) {
	OSCCONbits.IDLEN = 0;
	Sleep();
}

/*..........................................................................*/

/**
 * Deep Sleep mode brings the device into its lowest power consumption state
 * without requiring the use of external switches to remove power from the device.
 * During Deep Sleep, the on-chip VDDCORE voltage regulator is powered down,
 * effectively disconnecting power to the core logic of the microcontroller.
 *
 * NOTE:
 * Since Deep Sleep mode powers down the microcontroller by turning off the
 * on-chip VDDCORE voltage regulator, Deep Sleep capability is available only
 * on PIC18FXXJ members in the device family.
 * The on-chip voltage regulator is not available on PIC18LFXXJ members of the
 * device family, and therefore, they do not support Deep Sleep.
 *
 * On devices that support it, the Deep Sleep mode is entered by:
 *	- Setting the REGSLP (WDTCON<7>) bit
 *	- Clearing the IDLEN bit
 *	- Clearing the GIE bit
 *	- Setting the DSEN bit (DSCONH<7>)
 *	- Executing the SLEEP instruction immediately after setting DSEN
 *
 * In order to minimize the possibility of inadvertently entering Deep Sleep,
 * the DSEN bit is cleared in hardware, two instruction cycles after having been set.
 * Therefore, in order to enter Deep Sleep, the SLEEP instruction must be executed
 * in the immediate instruction cycle after setting DSEN.
 * If DSEN is not set when Sleep is executed, the device will enter conventional
 * Sleep mode instead.
 * During Deep Sleep, the core logic circuitry of the microcontroller is powered
 * down to reduce leakage current. Therefore, most peripherals and functions of
 * the microcontroller become unavailable during Deep Sleep.
 * However, a few specific peripherals and functions are powered directly from
 * the VDD supply rail of the microcontroller, and therefore, can continue to
 * function in Deep Sleep.
 * Entering Deep Sleep mode clears the DSWAKEL register.
 * However, if the Real-Time Clock and Calendar (RTCC) is enabled prior to
 * entering Deep Sleep, it will continue to operate uninterrupted.
 * The device has a dedicated Brown-out Reset (DSBOR) and Watchdog Timer Reset
 * (DSWDT) for monitoring voltage and time-out events in Deep Sleep.
 * The DSBOR and DSWDT are independent of the standard BOR and WDT used with
 *  other power-managed modes(Run, Idle and Sleep).
 * When a wake event occurs in Deep Sleep mode (by MCLR Reset, RTCC alarm,
 * INT0 interrupt, ULPWU or DSWDT), the device will exit Deep Sleep mode and
 * perform a Power-on Reset (POR). When the device is released from Reset,
 *  code execution will resume at the device’s Reset vector.
 */
void deepSleep(void) {
	WDTCONbits.REGSLP = 1;
	OSCCONbits.IDLEN = 0;
	INTCONbits.GIE = 0;
	DSCONHbits.DSEN = 1;
	Sleep();
	Nop();
}