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
    :upperLayerIn(0),
     upperLayerOut(0),
     upperControlIn(0),
     upperControlOut(0),
     gnl(NULL),
     myAddress(0),neighbors(),
     m_isAp(false),
     m_hasAp(false),
     m_apid(0),
     lastPos(Coord::ZERO),
     lastSpeed(Coord::ZERO),
     sendPower(0),
     receivePower(0)
{
}

GlobalControlUnit::~GlobalControlUnit() {
}

void GlobalControlUnit::initialize(int stage)
{

    EV<<"GlobalControlUnit::initialize(stage = "<< stage <<")"<<endl;
    if(stage==0){
        gnl=FindModule<GlobalNetworkLayer*>::findGlobalModule();
        ASSERT2(gnl!=NULL,"Can not find the GlobalNetworkLayer module.");

        upperLayerIn  = findGate("upperLayerIn");
        upperLayerOut = findGate("upperLayerOut");
        upperControlIn  = findGate("upperControlIn");
        upperControlOut = findGate("upperControlOut");

        receivePower = par("receivePower");
        sendPower = par("sendPower");
    }else if( stage == 1){
        gnl->registerGCU(this);
    }
}

void GlobalControlUnit::handleMessage(cMessage *msg)
{
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


void GlobalControlUnit::finish() {
    gnl->unregisterGCU(this);
}

Coord GlobalControlUnit::getCurrentPostion(){
    Enter_Method_Silent();
    return lastPos;
}

void GlobalControlUnit::setCurrentPostion(Coord pos) {
    Enter_Method_Silent();
    lastPos = pos;
    {
        CoDownCtrlMsg* msg = new CoDownCtrlMsg();
        msg->setMsgType(CDCMT_UpdatePostion);
        msg->setTargetId(getAddr());
        sendControlUp(msg);
    }
    if (!isAp()) {
        gnl->refreshGCU(this);
    }
}

void GlobalControlUnit::sendMsgToAP(int apid, cMessage* msg) {
    Enter_Method_Silent();
    cMessage* dup = msg->dup();
    gnl->sendMsgToAP(apid, dup);
    delete msg;
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
    Enter_Method_Silent();
    if(isAp()){
        return getApid();
    }else{
        return myAddress;
    }
}

void GlobalControlUnit::handleMsgFromNetwLayer(cMessage* msg) {
    Enter_Method_Silent();
    cMessage* dup = msg->dup();
    sendUp(dup);
    delete(msg);
}

void GlobalControlUnit::setAddr(int addr) {
    Enter_Method_Silent();
    this->myAddress = addr;
}

void GlobalControlUnit::connectToGCU(IGlobalControlUnit* gcu) {
    Enter_Method_Silent();
    neighbors[gcu->getAddr()] = gcu;
    {
        CoDownCtrlMsg* msg = new CoDownCtrlMsg();
        msg->setMsgType(CDCMT_ConnectToGCU);
        msg->setTargetId(getAddr());
        sendControlUp(msg);
    }
}

void GlobalControlUnit::disconnectFromGCU(IGlobalControlUnit* gcu) {
    Enter_Method_Silent();
    if (neighbors.erase(gcu->getAddr()) > 0) {
        gcu->disconnectFromGCU(this);
        {
            CoDownCtrlMsg* msg = new CoDownCtrlMsg();
            msg->setMsgType(CDCMT_DisconnectFromGCU);
            msg->setTargetId(getAddr());
            sendControlUp(msg);
        }
    }
}

void GlobalControlUnit::disconnectAll() {
    Enter_Method_Silent();
    EV<<"neighbors.size():"<<neighbors.size()<<endl;
    for (GCU_IGCU_MAP::iterator it = neighbors.begin();it!= neighbors.end(); ) {
        this->disconnectFromGCU(it->second);
        it = neighbors.begin();
    }
    {
        m_hasAp = false;
        this->m_apid = -1;
    }
    {
        CoDownCtrlMsg* msg = new CoDownCtrlMsg();
        msg->setMsgType(CDCMT_DisconnectAll);
        msg->setTargetId(-1);
        sendControlUp(msg);
    }
}

void GlobalControlUnit::connectToAP(int apid) {
    Enter_Method_Silent();
    ASSERT2(m_hasAp == false,
            "Error: Cannot disconnect form this AP, since this GCU has already connect to an AP.");
    m_hasAp = true;
    this->m_apid = apid;
    {
        CoDownCtrlMsg* msg = new CoDownCtrlMsg();
        msg->setMsgType(CDCMT_ConnectToAP);
        msg->setTargetId(apid);
        sendControlUp(msg);
    }
}

void GlobalControlUnit::disconnectFromAP(int apid) {
    Enter_Method_Silent();
    ASSERT2(m_hasAp == true && this->m_apid == apid,
            "Error: Cannot disconnect form this AP, since this GCU dose not connect to it.");
    m_hasAp = false;
    this->m_apid = -1;
    {
        CoDownCtrlMsg* msg = new CoDownCtrlMsg();
        msg->setMsgType(CDCMT_DisconnectFromAP);
        msg->setTargetId(apid);
        sendControlUp(msg);
    }
}

void GlobalControlUnit::sendUp(cMessage *msg) {
    ASSERT2(gate(upperLayerOut)->isPathOK(),
            "GlobalControlUnit: upperLayerOut is not connected");
    send(msg, upperLayerOut);
}

Coord GlobalControlUnit::getCurrentSpeed() {
    return lastSpeed;
}

void GlobalControlUnit::setCurrentSpeed(Coord speed) {
    Enter_Method_Silent();
    this->lastSpeed = speed;
}

void GlobalControlUnit::setSpeed(Coord speed) {
    Enter_Method_Silent();
    this->averageSpeed = speed;
}

Coord GlobalControlUnit::getSpeed() {
    Enter_Method_Silent();
    return averageSpeed;
}

void GlobalControlUnit::sendControlUp(cMessage *msg) {
    if (gate(upperControlOut)->isPathOK())
        send(msg, upperControlOut);
    else {
        EV << "GlobalControlUnit: upperControlOut is not connected; dropping message" << std::endl;
        delete msg;
    }
}

