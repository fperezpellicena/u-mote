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

#ifndef XBEE_H_
#define XBEE_H_

/*..........................................................................*/
                                                              /* XBee class */
typedef struct XBee XBee;

// TODO Are sourceEndpoint and destinationEndpoint registers used in API mode?
// TODO Check for I/O options if necessary
struct XBee{
    /* Basic variables */
    uint64_t serial;                        /* Serial number 64 bit integer */
    uint8_t sourceEndpoint;            /* Application layer source endpoint */
    uint8_t destinationEndpoint;  /* Application layer destination endpoint */
    uint16_t cluster;                                 /* Cluster identifier */
    uint16_t maxRFPayload;                      /* Maximun RF payload bytes */
    uint8_t channel;                                             /* Channel */
    uint8_t role;                             /* Role: Router or End device */
    uint8_t apiMode;           /* API mode: API off, API on, API on escaped */
    uint8_t apiFormat;           /* API output format: Standard or explicit */
    uint8_t baudrate;                                           /* Baudrate */
    uint8_t packetTimeout;                         /* Packetization timeout */
    uint8_t parity;                                  /* UART parity setting */

    /* IO variables*/
    /* NOT IMPLEMENTED */

    /* Diagnosis variables */
    uint16_t rfErrors;                                         /* RF errors */
    uint16_t goodPackets;      /* Good frames with valid MAC received by RF */
    uint16_t temperature;                             /* Module temperature */
    uint8_t rssi;            /* Signal strength received for last RF packet */

    /* Node identification variables */
    uint16_t networkId;                               /* Network identifier */
    uint32_t nodeId;               /* String identifier up to 20 characters */
    uint8_t networkDiscoverOptions;             /* Network discover options */

    /* Security variables */
    uint8_t aesEnabled;                         /* Security AES 128 enabled */

    /* Mac level variables */
    uint8_t broadcastRetransmissions;             /* Number of rebroadcasts */
    uint8_t unicastRetries;           /* Unicast retries if no ACK received */
    uint8_t rfPower;                                      /* RF power level */

    /* DigiMesh variables */
    uint8_t hops;                                  /* Maximum hops expected */
    uint8_t delay;                /* Network delay slots before rebroadcast */
    uint8_t retries;        /* Maximum delivery attempts if no ACK reveived */
    uint8_t radious;                                    /* Broadcast radius */
    uint8_t nodeType;                         /* Role: Router or End device */

    /* Sleep variables */
    uint8_t sleepMode;                          /* Sleep mode of the module */
    uint8_t sleepOptions;                          /* Sleep options bitmask */
    uint16_t wakeTime;               /* Amount of time awake in cyclic mode */
    uint16_t sleepPeriod;                 /* Amount of time sleep per cycle */
    uint8_t missedSyncs;     /* Wake cycles elapsed since last sync message */
    uint8_t missedSyncCnt;                /* Number of sync messages missed */
    uint8_t sleepStatus;                               /* Sleep status mask */
    uint16_t operationalSleepPeriod;         /* Sleep period currently used */
    uint16_t operationalWakeTime;               /* Wake time currently used */
    uint16_t wakeHost;                  /* Wake up time before sending data */


    /* Methods */
    uint64_t (*getSerialNumber)(XBee* xbee);
    void (*setSourceEndpoint)(XBee* xbee, uint8_t sourceEndpoint);
    uint8_t (*getSourceEndpoint)(XBee* xbee);
    void (*setDestinationEndpoint)(XBee* xbee, uint8_t destinationEndpoint);
    uint8_t (*getDestinationEndpoint)(XBee* xbee);
};

/*..........................................................................*/
                                                          /* XBee interface */

/* Default constructor */
XBee* createXBee();

/*
 * Return source serial numbre
 *
 * @param xbee      xbee instance
 * @return          serial number
 */
uint64_t getSerialNumber(const XBee* xbee);

/*
 * Set source endpoint value
 *
 * @param xbee              xbee instance
 * @param sourceEndpoint    source endopoint value
 */
void setSourceEndpoint(const XBee* xbee, uint8_t sourceEndpoint);

/*
 * Return source endpoint value
 *
 * @param xbee      xbee instance
 * @return source   source endpoint value
 */
uint8_t getSourceEndpoint(const XBee* xbee);

/*
 * Set destination endpoint value
 *
 * @param xbee                  XBee instance
 * @param destinationEndpoint   destination endopoint value
 */
void setDestinationEndpoint(const XBee* xbee, uint8_t destinationEndpoint);

/*
 * Return source endpoint value
 *
 * @param xbee      xbee instance
 * @return source   destination endpoint value
 */
uint8_t getDestinationEndpoint(const XBee* xbee);


#endif /* XBEE_H_ */
