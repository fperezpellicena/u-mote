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

#include "usb_digi_handler.h"
#include "digi_proxy.h"
#include "util.h"

#define USB_JOINED_MSG "Mote joined\0"
#define USB_JOINED_ERROR_MSG "Mote not joined\0"

void USBXBeeHandler_join(char usbBuffer[]) {
    if(XBeeProxy_join())
        Util_str2ram((UINT8 rom*)USB_JOINED_MSG, (UINT8*)usbBuffer);
    Util_str2ram((UINT8 rom*)USB_JOINED_MSG, (UINT8*)usbBuffer);
}