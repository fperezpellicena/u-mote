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
#include "usb_parser.h"

// Create custom stategy parser

void USBParser_create(USBParser * const parser, ParseUSBData parseStrategy, void* object) {
    parser->parse = parseStrategy;
    parser->object = object;
}

// Parse USB content

void USBParser_parse(USBParser * const parser,char* const usbOutBuffer, char* const usbInBuffer) {
    parser->parse(parser, usbOutBuffer, usbInBuffer);
}
