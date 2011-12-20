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

#ifndef usb_parser_h
#define usb_parser_h

// Parseador del contenido USB
typedef struct USBParser USBParser;

// Función estereotipo para los manejadores de contenido USB
typedef void (*ParseUSBData)(USBParser* const parser, char* const usbOutBuffer, char* const usbInBuffer);

struct USBParser {
    ParseUSBData parse;
    void* object;
};

void USBParser_parse(USBParser* const parser, char* const usbOutBuffer, char* const usbInBuffer);

void USBParser_create(USBParser* const parser, ParseUSBData parseStrategy, void* object);

#endif  /* usb_parser_h */
