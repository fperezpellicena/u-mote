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
 *  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <CppUTest/TestHarness.h>
#include "domain/XBeeMock.h"

TEST_GROUP(XBee) {
	XBeeMock xbee;
};

TEST(XBee, testSerialNumber) {
	uint64_t _serial = 0xFAAAAAAA;
	xbee.setSerialNumber(_serial);
	uint64_t serial = xbee.getSerialNumber();
	CHECK_EQUAL_C_INT(_serial, serial);
};

TEST(XBee, testSourceEndPoint) {
	uint8_t _sourceEndpoint = 0xFA;
	xbee.setSourceEndpoint(_sourceEndpoint);
	uint8_t souceEndPoint = xbee.getSourceEndpoint();
	CHECK_EQUAL_C_INT(_sourceEndpoint, souceEndPoint);
};

TEST(XBee, testDestinationEndPoint) {
	uint8_t _destinationEndpoint = 0xBF;
	xbee.setDestinationEndpoint(_destinationEndpoint);
	uint8_t destinationEndpoint = xbee.getDestinationEndpoint();
	CHECK_EQUAL_C_INT(_destinationEndpoint, destinationEndpoint);
};

