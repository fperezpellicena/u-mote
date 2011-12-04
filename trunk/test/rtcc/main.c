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

// RTCC test

#include <stdlib.h>
#include <rtcc.h>
#include <usart.h>

rtccDate date;
rtccTime time;
rtccTimeDate dateTime;

rtccDate alrmDate;
rtccTime alrmTime;
rtccTimeDate alrmDateTime;

const far rom char eol[] = "\r\n";

const far rom char start[] = "Start test\r\n";
const far rom char end[] = "End test\r\n";
const far rom char dateSet[] = "Date set\r\n";
const far rom char dateRead[] = "Date read\r\n";
const far rom char timeSet[] = "Time set\r\n";
const far rom char timeRead[] = "Time read\r\n";

const far rom char wday[] = "wday:";
const far rom char mon[] = "mon:";
const far rom char mday[] = "mday:";
const far rom char year[] = "year:";

const far rom char hour[] = "hour:";
const far rom char min[] = "min:";
const far rom char sec[] = "sec:";

char str[2];

void main() {
	Open1USART(USART_TX_INT_OFF	& USART_RX_INT_ON & USART_ASYNCH_MODE &
			USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 25);
	putrs1USART(start);
	
	// Init clk
	RtccInitClock();

	// Set date
	date.f.wday = 6;	// Domingo
	date.f.mon = 12;	// Diciembre
	date.f.mday = 4;	// Día del mes: 4
	date.f.year = 11;	// Año 2011
	RtccWriteDate(&date, TRUE);

	// Set time
	time.f.hour = 12;	// Hora del día
	time.f.min = 0;		// Minuto
	time.f.sec = 0;		// Segundo
	RtccWriteTime(&time, TRUE);

	// Set datetime
	dateTime.f.wday = 6;	// Domingo
	dateTime.f.mon = 12;	// Diciembre
	dateTime.f.mday = 4;	// Día del mes: 4
	dateTime.f.year = 11;	// Año 2011
	dateTime.f.hour = 12;	// Hora del día
	dateTime.f.min = 0;		// Minuto
	dateTime.f.sec = 0;		// Segundo
	RtccWriteTimeDate(&dateTime, TRUE);

	putrs1USART(dateSet);
	putrs1USART(wday);
	puts1USART(itoa(date.f.wday, str));
	putrs1USART(eol);
	putrs1USART(mon);
	puts1USART(itoa(date.f.mon, str));
	putrs1USART(eol);
	putrs1USART(mday);
	puts1USART(itoa(date.f.mday, str));
	putrs1USART(eol);
	putrs1USART(year);
	puts1USART(itoa(date.f.year, str));
	putrs1USART(eol);

	// Read date
	RtccReadDate(&date);
	putrs1USART(dateRead);
	putrs1USART(wday);
	puts1USART(itoa(date.f.wday, str));
	putrs1USART(eol);
	putrs1USART(mon);
	puts1USART(itoa(date.f.mon, str));
	putrs1USART(eol);
	putrs1USART(mday);
	puts1USART(itoa(date.f.mday, str));
	putrs1USART(eol);
	putrs1USART(year);
	puts1USART(itoa(date.f.year, str));
	putrs1USART(eol);

	putrs1USART(timeSet);
	putrs1USART(hour);
	puts1USART(itoa(time.f.hour, str));
	putrs1USART(eol);
	putrs1USART(min);
	puts1USART(itoa(time.f.min, str));
	putrs1USART(eol);
	putrs1USART(sec);
	puts1USART(itoa(time.f.sec, str));
	putrs1USART(eol);

	// Read time
	RtccReadTime(&time);
	putrs1USART(timeRead);
	putrs1USART(hour);
	puts1USART(itoa(time.f.hour, str));
	putrs1USART(eol);
	putrs1USART(min);
	puts1USART(itoa(time.f.min, str));
	putrs1USART(eol);
	putrs1USART(sec);
	puts1USART(itoa(time.f.sec, str));
	putrs1USART(eol);

	// Read datetime
	RtccReadTimeDate(&dateTime);

	// Set alarm date
	RtccWriteAlrmDate(&alrmDate);

	// Set alarm time
	RtccWriteAlrmTime(&alrmTime);

	// Set alarm datetime
	RtccWriteAlrmTimeDate(&alrmDateTime);

	// Read alarm date
	RtccReadAlrmDate(&alrmDate);

	// Read alarm time
	RtccReadAlrmTime(&alrmTime);

	// Read alarm datetime
	RtccReadAlrmTimeDate(&alrmDateTime);

	puts1USART(end);
	while(1);
}