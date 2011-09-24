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

#ifndef XBEE_H_
#define XBEE_H_

#include <stdint.h>

class XBee {
protected:
	uint64_t _serial;
	uint8_t _sourceEndpoint;
	uint8_t _destinationEndpoint;
public:
	XBee();
	virtual ~XBee();

	/**
	 * Return serial number
	 *
	 * @return serial number
	 */
	uint64_t getSerialNumber() const;

	/**
	 * Return source endpoint
	 *
	 * @return source endpoint
	 */
	uint8_t getSourceEndpoint() const;

	/**
	 * Return destination endpoint
	 *
	 * @return destination endpoint
	 */
	uint8_t getDestinationEndpoint() const;
};

#endif /* XBEE_H_ */
