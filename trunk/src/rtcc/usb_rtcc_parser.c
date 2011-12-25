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

#include <string.h>
#include <stdlib.h>
#include <rtcc.h>
#include "usb_rtcc_parser.h"

rom const char* SEPARATOR = "#";

/**
 * Implementación del prototipo de función para parsear el contenido USB
 * de configuración del RTCC
 */
void parseRTCCData(char* usbOutBuffer) {
    rtccTimeDate timestamp;
    char* result = NULL;
    result = strtokpgmram(usbOutBuffer, (rom const char far*)SEPARATOR);
    result = strtokpgmram(NULL, (rom const char far*)SEPARATOR);
    timestamp.f.hour = atoi(result);
    result = strtokpgmram(NULL, (rom const char far*)SEPARATOR);
    timestamp.f.min = atoi(result);

    RtccWriteTimeDate(&timestamp,TRUE);
}
