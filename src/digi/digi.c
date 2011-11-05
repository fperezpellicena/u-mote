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

#include "digi.h"


/**
 * Default constructor.
 * Set default parameters:
 * ID=7FFF CH=E RR=2 MT=3 BH=0 MR=1 NH=7 NN=3 CE=0 DH=0 DL=FFFF NI=ED5 NT=82
 * NO=0 DD=40000 EE=0 BD=3 NB=0 RO=3 D7=1 D6=0 FT=BE AP=1 AO=0 D0=0 D1=0 D2=0
 * D3=0 D4=0 D5=1 D8=0 D9=1 P0=0 P1=0 P2=0 M0=0 M1=0 LT=19 RP=28 PR=1FFF IC=0
 * IR=0 IF=1 CT=64 GT=3E8 CC=2B PL=4 WH=0 SO=3E SM=7 SP=7530 ST=51A
 */
void XBee_create(XBee* xbee) {
    xbee->networkId = 0x7FFF;
    xbee->channel = 0x0E;
    xbee->retries = 0x02;
    xbee->mtBroadcasts = 0x03;
    xbee->radious = 0x00;
    xbee->hops = 0x07;
    xbee->delay = 0x03;
    xbee->nodeType = 0x00;
    xbee->nodeId = 0xED5;
    xbee->nodeDiscoverTimeout = 0x82;
    xbee->networkDiscoverOptions = 0x00;
    xbee->deviceTypeId = 0x40000;
    xbee->aesEnabled = 0x00;
    xbee->baudrate = 0x03;
    xbee->parity = 0x00;
    xbee->packetTimeout = 0x03;
    xbee->flowCtrlThr = 0xBE;
    xbee->apiMode = 0x01;
    xbee->apiFormat = 0x00;
    xbee->assocBlinkTime = 0x19;
    xbee->rssiPwmPower = 0x28;
    xbee->pullUpRes = 0x1FFF;
    xbee->ioChangeDetect = 0x00;
    xbee->ioSampleRate = 0x00;
    xbee->sleepSampleRate = 0x00;
    xbee->sleepMode = 0x07;
    xbee->sleepOptions = 0x3E;
    xbee->wakeTime = 0x51A;
    xbee->sleepPeriod = 7530;
    xbee->rfPower = 0x04;
}
