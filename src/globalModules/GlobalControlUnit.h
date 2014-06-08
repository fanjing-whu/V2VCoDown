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
#include "GlobalNetworkLayer.h"
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
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

public:
    virtual int getAddr();
    virtual void setAddr(int addr);
    virtual Coord getCurrentPostion();
    virtual void setCurrentPostion(Coord pos);
    virtual void handleMsgFromNetwLayer(cMessage* msg);
    virtual void sendMsgToAP(int apid, cMessage *msg);

    /** @brief Handle self messages such as timer... */
    virtual void handleSelfMsg(cMessage* msg);
    virtual void handleUpperMsg(cMessage *msg);
    virtual void handleUpperControl(cMessage *msg);

    /** @brief Sends a message to the upper layer
     *
     * Short hand for send(msg, upperLayerOut);
     * You have to take care of decapsulation and deletion of
     * superflous frames. We recommend that you use a pair of
     * functions decapsMsg/encapsMsg.
     */
    virtual void sendUp(cMessage *msg);

    /** @brief Sends a control message to an upper layer */
    virtual void sendControlUp(cMessage *msg);
protected:

    /* gate ids */
    int upperLayerIn;
    int upperLayerOut;
    int upperControlIn;
    int upperControlOut;
private:
    GlobalNetworkLayer *gnl;
    int myAddress;

};

#endif
