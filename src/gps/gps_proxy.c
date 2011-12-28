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

#include "gps_proxy.h"

void GpsProxy_create(GpsProxy * const proxy, Serial * const serial) {
    proxy->serial = serial;
}

void GpsProxy_readOutput(GpsProxy * const proxy, NMEAOutput* packet) {

}

void GpsProxy_sendCommand(GpsProxy * const proxy, NMEACommandPacket* packet) {

}
