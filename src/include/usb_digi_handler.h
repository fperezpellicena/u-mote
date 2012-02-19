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

#ifndef usb_digi_handler
#define usb_digi_handler

/**
 * Realiza la operación de unión de un nodo a la red mientras
 * está conectado al terminal USB.
 *  1.- Envía el comando ATCB1
 *  2.- Espera a recibir una trama de sincronismo.
 *  3.- Envía el comando ATSM8
 *
 * @param usbBuffer
 */
void USBXBeeHandler_join(char usbBuffer[]);

#endif /* usb_digi_handler */