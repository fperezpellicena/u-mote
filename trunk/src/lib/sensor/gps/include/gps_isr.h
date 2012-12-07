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
 *
 */

#ifndef gps_isr_h
#define gps_isr_h

#include "GenericTypeDefs.h"

/**
 * Handle Gps interrupt
 */
void Gps_handleInterrupt(void);

/**
 * Check for Gps valid signal
 *
 * @return TRUE when gps gets valid satellite fixed position
 */
BOOL Gps_checkInterrupt(void);

/**
 * Clear interrupt flag after ISR processing
 */
void Gps_clearInterruptFlag(void);

/**
 * Install Gps interrupt on interrupt vector
 */
void Gps_installInterrupt(void);

#endif /* gps_h */
