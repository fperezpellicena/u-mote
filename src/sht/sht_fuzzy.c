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

#include "sht_fuzzy.h"
#ifdef SHT_ENABLED
#include "sensor_proxy.h"

#pragma udata sht_mf
DECLARE_MF(lowTemp, -40, 0, 20);
DECLARE_MF(midTemp, 0, 20, 40);
DECLARE_MF(highTemp, 30, 40, 255);
DECLARE_RT(ifHighTemp, &highTemp);
DECLARE_RT(ifMidTemp, &midTemp);
DECLARE_RT(ifLowTemp, &lowTemp);
#pragma udata

/*...........................................................................*/
void ShtFuzzy_init() {
    Sht11_init();
}

#endif
