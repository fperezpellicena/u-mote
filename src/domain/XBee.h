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
// TODO Should ProfileId be stored? It's used in xbee api frames

struct XBee {
    /* Basic variables */
    uint64_t serial; /* Serial number 64 bit integer */
    uint8_t sourceEndpoint; /* Application layer source endpoint */
    uint8_t destinationEndpoint; /* Application layer destination endpoint */
    uint16_t cluster; /* Cluster identifier */
    uint16_t profile; /* Profile used un explicit addressing */
    uint16_t maxRFPayload; /* Maximun RF payload bytes */
    uint8_t channel; /* Channel */
    uint8_t role; /* Role: Router or End device */
    uint8_t apiMode; /* API mode: API off, API on, API on escaped */
    uint8_t apiFormat; /* API output format: Standard or explicit */
    uint8_t baudrate; /* Baudrate */
    uint8_t packetTimeout; /* Packetization timeout */
    uint8_t parity; /* UART parity setting */

    /* IO variables*/
    /* NOT IMPLEMENTED */

    /* Diagnosis variables */
    uint16_t rfErrors; /* RF errors */
    uint16_t goodPackets; /* Good frames with valid MAC received by RF */
    uint16_t temperature; /* Module temperature */
    uint8_t rssi; /* Signal strength received for last RF packet */

    /* Node identification variables */
    uint16_t networkId; /* Network identifier */
    uint32_t nodeId; /* String identifier up to 20 characters */
    uint8_t networkDiscoverOptions; /* Network discover options */

    /* Security variables */
    uint8_t aesEnabled; /* Security AES 128 enabled */

    /* Mac level variables */
    uint8_t broadcastRetransmissions; /* Number of rebroadcasts */
    uint8_t unicastRetries; /* Unicast retries if no ACK received */
    uint8_t rfPower; /* RF power level */

    /* DigiMesh variables */
    uint8_t hops; /* Maximum hops expected */
    uint8_t delay; /* Network delay slots before rebroadcast */
    uint8_t retries; /* Maximum delivery attempts if no ACK reveived */
    uint8_t radious; /* Broadcast radius */
    uint8_t nodeType; /* Role: Router or End device */

    /* Sleep variables */
    uint8_t sleepMode; /* Sleep mode of the module */
    uint8_t sleepOptions; /* Sleep options bitmask */
    uint16_t wakeTime; /* Amount of time awake in cyclic mode */
    uint16_t sleepPeriod; /* Amount of time sleep per cycle */
    uint8_t missedSyncs; /* Wake cycles elapsed since last sync message */
    uint8_t missedSyncCnt; /* Number of sync messages missed */
    uint8_t sleepStatus; /* Sleep status mask */
    uint16_t operationalSleepPeriod; /* Sleep period currently used */
    uint16_t operationalWakeTime; /* Wake time currently used */
    uint16_t wakeHost; /* Wake up time before sending data */


    /* Basic methods */
    uint64_t(*getSerialNumber)(XBee * const xbee);
    void (*setSourceEndpoint)(XBee * const xbee, uint8_t sourceEndpoint);
    uint8_t(*getSourceEndpoint)(XBee * const xbee);
    void (*setDestinationEndpoint)(XBee * const xbee, uint8_t destinationEndpoint);
    uint8_t(*getDestinationEndpoint)(XBee * const xbee);
    uint16_t(*getCluster)(XBee * const xbee);
    void (*setCluster)(XBee * const xbee, uint16_t cluster);
    uint16_t(*getProfile)(XBee * const xbee);
    void (*setProfile)(XBee * const xbee, uint16_t profile);
    uint16_t(*getMaxRFPayload)(XBee * const xbee);
    void (*setMaxRFPayload)(XBee * const xbee, uint16_t maxRFPayload);
    uint8_t(*getChannel)(XBee * const xbee);
    void (*setChannel)(XBee * const xbee, uint8_t channel);
    uint8_t(*getRole)(XBee * const xbee);
    void (*setRole)(XBee * const xbee, uint8_t role);
    uint8_t(*getApiMode)(XBee * const xbee);
    void (*setApiMode)(XBee * const xbee, uint8_t apiMode);
    uint8_t(*getApiFormat)(XBee * const xbee);
    void (*setApiFormat)(XBee * const xbee, uint8_t apiFormat);
    uint8_t(*getBaudrate)(XBee * const xbee);
    void (*setBaudrate)(XBee * const xbee, uint8_t baudrate);
    uint8_t(*getPacketTimeot)(XBee * const xbee);
    void (*setPacketTimeout)(XBee * const xbee, uint8_t packetTimeout);
    uint8_t(*getParity)(XBee * const xbee);
    void (*setParity)(XBee * const xbee, uint8_t parity);

    /* IO methods*/
    /* NOT IMPLEMENTED */

    /* Diagnosis methods (read only)*/
    uint16_t(*getRfErrors)(XBee * const xbee); /* RF errors */
    uint16_t(*getGoodPackets)(XBee * const xbee); /* Good frames with valid MAC received by RF */
    uint16_t(*getTemperature)(XBee * const xbee); /* Module temperature */
    uint8_t(*getRssi)(XBee * const xbee); /* Signal strength received for last RF packet */

    /* Node identification methods */
    void (*setNetworkId)(XBee * const xbee, uint16_t networkId);
    uint16_t(*getNetworkId)(XBee * const xbee); /* Network identifier */
    void (*setNodeId)(XBee * const xbee, uint32_t nodeId);
    uint32_t(*getNodeId)(XBee * const xbee); /* String identifier up to 20 characters */
    void (*setNetworkDiscoverOptions)(XBee * const xbee, uint8_t networkDiscoverOptions);
    uint8_t(*getNetworkDiscoverOptions)(XBee * const xbee); /* Network discover options */

    /* Security methods */
    uint8_t (*isAesEnabled)(XBee * const xbee);
    uint8_t(*enableAes)(XBee * const xbee); /* Security AES 128 enabled */
    uint8_t(*disableAes)(XBee * const xbee); /* Security AES 128 enabled */

    /* Mac level methods */
    void (*setBroadcastRetransmissions)(XBee * const xbee, uint8_t broadcastRetransmissions);
    uint8_t(*getBroadcastRetransmissions)(XBee * const xbee); /* Number of rebroadcasts */
    void (*setUnicastRetries)(XBee * const xbee, uint8_t unicastRetries);
    uint8_t(*getUnicastRetries)(XBee * const xbee); /* Unicast retries if no ACK received */
    void (*setRfPower)(XBee * const xbee, uint8_t rfPower);
    uint8_t(*getRfPower)(XBee * const xbee); /* RF power level */

    /* DigiMesh variables */
    void (*setHops)(XBee * const xbee, uint8_t hops);
    uint8_t(*getHops)(XBee * const xbee); /* Maximum hops expected */
    void (*setDelay)(XBee * const xbee, uint8_t delay);
    uint8_t(*getDelay)(XBee * const xbee); /* Network delay slots before rebroadcast */
    void (*setRetries)(XBee * const xbee, uint8_t retries);
    uint8_t(*getRetries)(XBee * const xbee); /* Maximum delivery attempts if no ACK reveived */
    void (*setRadious)(XBee * const xbee, uint8_t radious);
    uint8_t(*getRadious)(XBee * const xbee); /* Broadcast radius */
    void (*setNodeType)(XBee * const xbee, uint8_t nodeType);
    uint8_t(*getNodeType)(XBee * const xbee); /* Role: Router or End device */

    /* Sleep variables */
    void (*setSleepMode)(XBee * const xbee, uint8_t sleepMode);
    uint8_t(*getSleepMode)(XBee * const xbee); /* Sleep mode of the module */
    void (*setSleepOptions)(XBee * const xbee, uint8_t options);
    uint8_t(*getSleepOptions)(XBee * const xbee); /* Sleep options bitmask */
    void (*setWakeTime)(XBee * const xbee, uint16_t wakeTime);
    uint16_t(*getWakeTime)(XBee * const xbee); /* Amount of time awake in cyclic mode */
    void (*setSleepPeriod)(XBee * const xbee, uint16_t sleepPeriod);
    uint16_t(*getSleepPeriod)(XBee * const xbee); /* Amount of time sleep per cycle */
    uint8_t(*getMissedSyncs)(XBee * const xbee); /* Wake cycles elapsed since last sync message */
    uint8_t(*getMissedSyncCnt)(XBee * const xbee); /* Number of sync messages missed */
    void (*setSleepStatus)(XBee * const xbee, uint8_t status);
    uint8_t(*getSleepStatus)(XBee * const xbee); /* Sleep status mask */
    uint16_t(*getOperationalSleepPeriod)(XBee * const xbee); /* Sleep period currently used */
    uint16_t(*getOperationalWakeTime)(XBee * const xbee); /* Wake time currently used */
    void (*setWakeHost)(XBee * const xbee, uint16_t wakeHost);
    uint16_t(*getWakeHost)(XBee * const xbee); /* Wake up time before sending data */

};

/*..........................................................................*/
/* XBee interface */

/* Default constructor */
XBee* createXBee();

/**
 * Return source serial number
 *
 * @param xbee      xbee instance
 * @return          serial number
 */
uint64_t getSerialNumber(XBee * const xbee);

/**
 * Set source endpoint value
 *
 * @param xbee              xbee instance
 * @param sourceEndpoint    source endopoint value
 */
void setSourceEndpoint(XBee * const xbee, uint8_t sourceEndpoint);

/**
 * Return source endpoint value
 *
 * @param xbee      xbee instance
 * @return source endpoint value
 */
uint8_t getSourceEndpoint(XBee * const xbee);

/**
 * Set destination endpoint value
 *
 * @param xbee                  XBee instance
 * @param destinationEndpoint   destination endopoint value
 */
void setDestinationEndpoint(XBee * const xbee, uint8_t destinationEndpoint);

/**
 * Return source endpoint value
 *
 * @param xbee      xbee instance
 * @return source destination endpoint value
 */
uint8_t getDestinationEndpoint(XBee * const xbee);

/**
 * Return cluster id
 *
 * @param xbee      xbee instance
 * @return cluster identificator
 */
uint16_t getCluster(XBee * const xbee);

/**
 * Set cluster id
 *
 * @param xbee      xbee instance
 * @param cluster   cluster identificator
 */
void setCluster(XBee * const xbee, uint16_t cluster);

/**
 * Return profile id
 *
 * @param xbee  xbee instance
 * @return profile used
 */
uint16_t getProfile(XBee * const xbee);

/**
 * Set profile id
 *
 * @param xbee      xbee instance
 * @param profile   profile id
 */
void setProfile(XBee * const xbee, uint16_t profile);

/**
 * Return maximum RF payload bytes
 *
 * @param xbee      xbee instance
 * @return maximum RF payload in bytes
 */
uint16_t getMaxRFPayload(XBee * const xbee);

/**
 * Set maximum RF payload bytes
 * @param xbee          xbee instance
 * @param maxRFPayload  maximum RF payload in bytes
 */
void setMaxRFPayload(XBee * const xbee, uint16_t maxRFPayload);

/**
 * Return channel used in the network
 *
 * @param xbee      xbee instance
 * @return channel used in the network
 */
uint8_t getChannel(XBee * const xbee);

/**
 * Set channel used in the network
 *
 * @param xbee      xbee instance
 * @param channel   channel
 */
void setChannel(XBee * const xbee, uint8_t channel);

/**
 * Return the role of the mote in the network
 *
 * @param xbee      xbee instance
 * @return role of the mote in the network
 */
uint8_t getRole(XBee * const xbee);

/**
 * Set the role of the mote in the network
 *
 * @param xbee      xbee instance
 * @param role      role
 */
void setRole(XBee * const xbee, uint8_t role);

/**
 * Return API mode used
 *
 * @param xbee      xbee instance
 * @return api mode used
 */
uint8_t getApiMode(XBee * const xbee);

/**
 * Set API mode used
 *
 * @param xbee      xbee instance
 * @param apiMode   api mode
 */
void setApiMode(XBee * const xbee, uint8_t apiMode);

/**
 * Return API format
 *
 * @param xbee      xbee instance
 * @return api format used in api mode
 */
uint8_t getApiFormat(XBee * const xbee);

/**
 * Set API format
 *
 * @param xbee      xbee instance
 * @param apiFormat api format used in api mode
 */
void setApiFormat(XBee * const xbee, uint8_t apiFormat);

/**
 * Return UART baudrate
 *
 * @param xbee      xbee instance
 * @return UART baudrate
 */
uint8_t getBaudrate(XBee * const xbee);

/**
 * Set UART baudrate
 *
 * @param xbee      xbee instance
 * @param baudrate  baudrate
 */
void setBaudrate(XBee * const xbee, uint8_t baudrate);

/**
 * Return packet timeout
 *
 * @param xbee      xbee instance
 * @return packet timeout configured
 */
uint8_t getPacketTimeot(XBee * const xbee);

/**
 * Set packet timeout
 *
 * @param xbee          xbee instance
 * @param packetTimeout packet timeout configured
 */
void setPacketTimeout(XBee * const xbee, uint8_t packetTimeout);

/**
 * Return UART parity
 *
 * @param xbee      xbee instance
 * @return UART parity
 */
uint8_t getParity(XBee * const xbee);

/**
 * Set UART parity
 *
 * @param xbee      xbee instance
 * @param parity    UART parity
 */
void setParity(XBee * const xbee, uint8_t parity);

/**
 * Return RF errors
 *
 * @param xbee      xbee instance
 * @return Rf errors
 */
uint16_t getRfErrors(XBee * const xbee);

/**
 * Return good packets
 * Good frames with valid MAC received by RF
 *
 * @param xbee      xbee instance
 * @return Good packets
 */
uint16_t getGoodPackets(XBee * const xbee);

/**
 * Return module temperature
 *
 * @param xbee      xbee instance
 * @return module temperature
 */
uint16_t getTemperature(XBee * const xbee);

/**
 * Return signal strength received for last RF packet
 *
 * @param xbee      xbee instance
 * @return rssi
 */
uint8_t getRssi(XBee * const xbee);

/**
 *  Return network identifier
 *
 * @param xbee      xbee instance
 * @return network id
 */
uint16_t(*getNetworkId)(XBee * const xbee);

/**
 *  Set network identifier
 *
 * @param xbee  xbee instance
 * @param id    network id
 */
void (*setNetworkId)(XBee * const xbee, uint16_t id);

/**
 * Return string identifier
 *
 * @param xbee  xbee instance
 * @return node identifier up to 20 characters
 */
uint32_t(*getNodeId)(XBee * const xbee);

/**
 *  Set string identifier up to 20 characters
 *
 * @param xbee  xbee instance
 * @param id    node id
 */
void (*setNodeId)(XBee * const xbee, uint32_t id);

/**
 *  Return network discover options
 *
 * @param xbee  xbee instance
 * @return network discover options
 */
uint8_t(*getNetworkDiscoverOptions)(XBee * const xbee);

/**
 *  Set network discover options
 *
 * @param xbee      xbee instance
 * @param ioptions  discover options
 */
void (*setNetworkDiscoverOptions)(XBee * const xbee, uint8_t options); 

/**
 *  Return if security AES 128 is enabled
 *
 * @param xbee  xbee instance
 * @return 0xFF if true
 */
uint8_t(*isAesEnabled)(XBee * const xbee);

/**
 * Set Security AES 128 enabled
 *
 * @param xbee  xbee instance
 */
void (*enableAes)(XBee * const xbee);

/**
 * Set Security AES 128 disabled
 *
 * @param xbee  xbee instance
 */
void (*disableAes)(XBee * const xbee);

// TODO Falta establecer la clave AES


/**
 * Return number of rebroadcasts
 *
 * @param xbee  xbee instance
 * @return rebroadcasts MT
 */
uint8_t(*getBroadcastRetransmissions)(XBee * const xbee);

/**
 * Set number of rebroadcasts
 *
 * @param xbee  xbee instance
 * @param mt    rebroadcasts
 */
void (*setBroadcastRetransmissions)(XBee * const xbee, uint8_t mt);

/**
 *  Return unicast retries if no ACK received
 *
 * @param xbee  xbee instance
 * @return unicast retries RR
 */
uint8_t(*getUnicastRetries)(XBee * const xbee);

/**
 * Set number of unicast retries
 *
 * @param xbee  xbee instance
 * @param rr    unicast retries
 */
void (*setUnicastRetries)(XBee * const xbee, uint8_t rr);

/**
 * Return RF power level
 *
 * @param xbee  xbee instance
 * @return RF power configured
 */
uint8_t(*getRfPower)(XBee * const xbee);

/**
 * Set RF power level
 *
 * @param xbee  xbee instance
 * @param power rf power
 */
void (*setRfPower)(XBee * const xbee, uint8_t power); 


/**
 *  Return Maximum hops expected
 *
 * @param xbee  xbee instance
 * @return hops
 */
uint8_t(*getHops)(XBee * const xbee);

/**
 *  Set Maximum hops expected
 *
 * @param xbee  xbee instance
 * @param hops  hops
 */
void (*setHops)(XBee * const xbee, uint8_t hops);

/**
 *  Return network delay slots before rebroadcast
 *
 * @param xbee  xbee instance
 * @return delay on rebroadcast
 */
uint8_t(*getDelay)(XBee * const xbee);

/**
 *  Set network delay slots before rebroadcast
 *
 * @param xbee  xbee instance
 * @param delay delay on rebroadcast
 */
void (*setDelay)(XBee * const xbee, uint8_t delay);

/**
 * Return Maximum delivery attempts if no ACK reveived
 *
 * @param xbee  xbee instance
 * @return mesh network retries
 */
uint8_t(*getRetries)(XBee * const xbee);

/**
 * Set Maximum delivery attempts if no ACK reveived
 *
 * @param xbee      xbee instance
 * @param retries   mesh netework retries
 */
void (*setRetries)(XBee * const xbee, uint8_t retries);

/**
 * Return Broadcast radious
 *
 * @param xbee  xbee instance
 * @return Broadcast radious
 */
uint8_t(*getRadious)(XBee * const xbee);

/**
 * Set Broadcast radious
 *
 * @param xbee      xbee instance
 * @param radious   Broadcast radious
 */
void (*setRadious)(XBee * const xbee, uint8_t radious);

/**
 *  Return Role: Router or End device
 *
 * @param xbee  xbee instance
 * @return node type
 */
uint8_t(*getNodeType)(XBee * const xbee);

/**
 *  Set Role: Router or End device
 *
 * @param xbee  xbee instance
 * @param type  node type
 */
void (*setNodeType)(XBee * const xbee, uint8_t type); 

/* Sleep variables */
uint8_t(*getSleepMode)(XBee * const xbee); /* Sleep mode of the module */
void (*setSleepMode)(XBee * const xbee, uint8_t mode); /* Sleep mode of the module */
uint8_t(*getSleepOptions)(XBee * const xbee); /* Sleep options bitmask */
void (*setSleepOptions)(XBee * const xbee, uint8_t options); /* Sleep options bitmask */
uint16_t(*getWakeTime)(XBee * const xbee); /* Amount of time awake in cyclic mode */
void (*setWakeTime)(XBee * const xbee, uint16_t time); /* Amount of time awake in cyclic mode */
uint16_t(*getSleepPeriod)(XBee * const xbee); /* Amount of time sleep per cycle */
void (*setSleepPeriod)(XBee * const xbee, uint16_t period); /* Amount of time sleep per cycle */
uint8_t(*getMissedSyncs)(XBee * const xbee); /* Wake cycles elapsed since last sync message */
uint8_t(*getMissedSyncCnt)(XBee * const xbee); /* Number of sync messages missed */
uint8_t(*getSleepStatus)(XBee * const xbee); /* Sleep status mask */
void (*setSleepStatus)(XBee * const xbee, uint8_t status); /* Sleep status mask */
uint16_t(*getOperationalSleepPeriod)(XBee * const xbee); /* Sleep period currently used */
uint16_t(*getOperationalWakeTime)(XBee * const xbee); /* Wake time currently used */
uint16_t(*getWakeHost)(XBee * const xbee); /* Wake up time before sending data */
void (*setWakeHost)(XBee * const xbee, uint16_t wakeHost); /* Wake up time before sending data */


#endif /* XBEE_H_ */
