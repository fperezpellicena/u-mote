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

#ifndef XBEEMOCK_H_
#define XBEEMOCK_H_

#include "../../domain/XBee.h"

class XBeeMock : public XBee {
public:
	XBeeMock();
	virtual ~XBeeMock();
	/** Dummy set serial number for testing purposes */
	void setSerialNumber(uint64_t serial) {_serial = serial;}
	/** Mock set source endpoint for testing purposes */
	void setSourceEndpoint(uint8_t sourceEndpoint) {_sourceEndpoint = sourceEndpoint;}
	/** Mock set destination endpoint for testing purposes */
	void setDestinationEndpoint(uint8_t destinationEndpoint) {_destinationEndpoint= destinationEndpoint;}
};

#endif /* XBEEMOCK_H_ */
