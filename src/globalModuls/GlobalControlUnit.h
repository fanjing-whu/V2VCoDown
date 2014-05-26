//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __SIMPLEVEINS_GLOBALCONTROLUNIT_H_
#define __SIMPLEVEINS_GLOBALCONTROLUNIT_H_

#include <omnetpp.h>
#include "IGlobalNetworkLayer.h"
#include "IGlobalControlUnit.h"
#include "FindModule.h"
#include "Coord.h"

/**
 * TODO - Generated class
 */
class GlobalControlUnit: public cSimpleModule, public IGlobalControlUnit {
public:
    GlobalControlUnit();
    virtual ~GlobalControlUnit();
protected:
    void initialize(int stage);
    void handleMessage(cMessage *msg);
    void finish();

public:
    int getAddr();
    Coord getCurrentPostion();
    void setCurrentPostion(Coord pos);
    void sendMsgToAP(int apid, cMessage *msg);

    /** @brief Handle self messages such as timer... */
    void handleSelfMsg(cMessage* msg);
    void handleUpperMsg(cMessage *msg);
    void handleUpperControl(cMessage *msg);

    /** @brief Sends a message to the upper layer
     *
     * Short hand for send(msg, upperLayerOut);
     * You have to take care of decapsulation and deletion of
     * superflous frames. We recommend that you use a pair of
     * functions decapsMsg/encapsMsg.
     */
    void sendUp(cMessage *msg);

    /** @brief Sends a control message to an upper layer */
    void sendControlUp(cMessage *msg);
protected:

    /* gate ids */
    int upperLayerIn;
    int upperLayerOut;
    int upperControlIn;
    int upperControlOut;
private:
    IGlobalNetworkLayer *gnl;

};

#endif
