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

#include "GlobalControlUnit.h"

Define_Module(GlobalControlUnit);

GlobalControlUnit::GlobalControlUnit()
    :gnl(NULL),myAddress(0)
{
}

GlobalControlUnit::~GlobalControlUnit() {
}

void GlobalControlUnit::initialize(int stage)
{
    // TODO - Generated method body

    EV<<"GlobalControlUnit::initialize(stage = "<< stage <<")"<<endl;
    if(stage==0){
        gnl=FindModule<GlobalNetworkLayer*>::findGlobalModule();
        ASSERT2(gnl!=NULL,"Can not find the GlobalNetworkLayer module.");

        upperLayerIn  = findGate("upperLayerIn");
        upperLayerOut = findGate("upperLayerOut");
        upperControlIn  = findGate("upperControlIn");
        upperControlOut = findGate("upperControlOut");
    }
}

void GlobalControlUnit::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    if (msg->isSelfMessage()){
        handleSelfMsg(msg);
    } else if(msg->getArrivalGateId()==upperLayerIn) {
        handleUpperMsg(msg);
    } else if(msg->getArrivalGateId()==upperControlIn) {
        handleUpperControl(msg);
    }
    else if(msg->getArrivalGateId()==-1) {
        /* Classes extending this class may not use all the gates, f.e.
         * BaseApplLayer has no upper gates. In this case all upper gate-
         * handles are initialized to -1. When getArrivalGateId() equals -1,
         * it would be wrong to forward the message to one of these gates,
         * as they actually don't exist, so raise an error instead.
         */
        opp_error("No self message and no gateID?? Check configuration.");
    } else {
        /* msg->getArrivalGateId() should be valid, but it isn't recognized
         * here. This could signal the case that this class is extended
         * with extra gates, but handleMessage() isn't overridden to
         * check for the new gate(s).
         */
        opp_error("Unknown gateID?? Check configuration or override handleMessage().");
    }
}

void GlobalControlUnit::sendUp(cMessage *msg) {
    send(msg, upperLayerOut);
}

void GlobalControlUnit::finish() {
}

Coord GlobalControlUnit::getCurrentPostion(){
    return lastPos;
}

void GlobalControlUnit::setCurrentPostion(Coord pos) {
    lastPos = pos;
    gnl->refreshGCU(this);
}

void GlobalControlUnit::sendMsgToAP(int apid, cMessage* msg) {
    gnl->sendMsgToAP(apid, msg);
}

void GlobalControlUnit::handleSelfMsg(cMessage* msg) {
    std::cout<<"The GlobalControlUnit does not handle any self message."<<std::endl;
    delete(msg);
}

void GlobalControlUnit::handleUpperMsg(cMessage* msg) {
    gnl->sendMsg(msg);
}

void GlobalControlUnit::handleUpperControl(cMessage* msg) {
    std::cout<<"The GlobalControlUnit does not handle any control message."<<std::endl;
    delete(msg);
}

int GlobalControlUnit::getAddr() {
    return myAddress;
}

void GlobalControlUnit::handleMsgFromNetwLayer(cMessage* msg) {
    sendUp(msg);
}

void GlobalControlUnit::setAddr(int addr) {
    this->myAddress = addr;
}

void GlobalControlUnit::connectToGCU(IGlobalControlUnit* gcu) {
    IGlobalControlUnit::connectToGCU(gcu);

}

void GlobalControlUnit::disconnectFromGCU(IGlobalControlUnit* gcu) {
}

void GlobalControlUnit::disconnectAll() {
}

void GlobalControlUnit::connectToAP(int apid) {
}

void GlobalControlUnit::disconnectFromAP(int apid) {
}

void GlobalControlUnit::sendControlUp(cMessage *msg) {
    if (gate(upperControlOut)->isPathOK())
        send(msg, upperControlOut);
    else {
        EV << "BaseLayer: upperControlOut is not connected; dropping message" << std::endl;
        delete msg;
    }
}
