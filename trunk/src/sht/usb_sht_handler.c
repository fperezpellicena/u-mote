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

#include "usb_sht_handler.h"
#include "sht.h"
#include <stdio.h>
#include <math.h>

/**
 * No es compatible con USB.
 *
 * USB interrumpe cada cierto tiempo y el sensor bloquea las interrupciones
 * al realizar una espera ocupada, por lo que el módulo USB deja de funcionar
 * correctamente.
 * Se podría partir la funcionalidad en interrupt driven evitando la espera
 * ocupada, lo que significa que el pin asociado a datos debe poder generar
 * interrupción.
 * Esta interrupción está sólo habilitada mientras dura la espera hasta que
 * el sensor termina la conversión.
 * Una vez iniciada la rutina de interrupción, esta se deshabilita hasta que
 * termina, para evitar generar interrupciones constantemente.
 * Cuando la rutina termina se habilida de nuevo para la próxima medición.
 *
 * El sistema de interrupciones, permite realizar esto implementando la función
 * de interrupción y añadiéndola a la lista de handlers.
 */

// Measure sensor
unsigned char USBShtHandler_sht11Test(char* usbBuffer) {
    Sht shtData;
    Sht11_init();
    // Perform measures and calculations
    Sht11_measure(&shtData);
    // Put info into buffer
    sprintf(usbBuffer,(const rom far char*)"Relative temperature:%d.%2u % \n\r Relative humidity:%d.%2u % \n\r",
            (int) shtData.data.temperature.f / 100,
            fabs(((shtData.data.temperature.f - (int) shtData.data.temperature.f)*100)),
            (int) shtData.data.humidity.f / 100,
            fabs(((shtData.data.humidity.f - (int) shtData.data.humidity.f)*100)));
    // Return length
    return 60;
}

