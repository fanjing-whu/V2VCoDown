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

#include "CooperativeDownload.h"

Define_Module(CooperativeDownload);

CooperativeDownload::CooperativeDownload() {
    // TODO generate()
}

CooperativeDownload::~CooperativeDownload() {
    clearContentMap();
    clearTimeMap();
    cancelAndDelete(frameTimer);
}

void CooperativeDownload::finish() {
    // TODO finish()
}
void CooperativeDownload::initialize(int stage)
{
    BaseGlobalAppLayer::initialize(stage);
    if(stage == 0){
        // initialize the gcu module
        gcu->setAddr(getParentModule()->getId());
        gcu->isAp(false);
        gcu->apid(-1);
        // initialize other parameters
        coreDebug = par("coreDebug").boolValue();
        debug = par("debug").boolValue();
        isTargetCar = par("isTargetCar").boolValue();
        taskSize = par("taskSize").doubleValue();
        frameSize = par("frameSize").doubleValue();
        frameInterval = par("frameInterval").doubleValue();
        frameTimer = new cMessage("frameTimer");

        car_Status = CAR_IDEL;
        if(isTargetCar){
            contentQueueMap[gcu->getAddr()] = new SegmentQueue(0,taskSize);
        }
    }else if(stage == 1){

    }
}

void CooperativeDownload::handleSelfMsg(cMessage* msg) {
    if(msg == frameTimer){
        handleFrameTimer();
        delete msg;
    }else{
        debugEV<<"Error: Unknown self message."<<endl;
        delete msg;
    }
}

void CooperativeDownload::handleLowerControl(cMessage* msg) {
    CoDownCtrlMsg* cdmsg =check_and_cast<CoDownCtrlMsg*>(msg);
    if(cdmsg == NULL){
        debugEV <<"Error: CooperativeDownload::get a wrong type message."
                << endl;
        delete msg;
    } else {
        switch (cdmsg->getMsgType()) {
        case CDCMT_ConnectToAP:
            debugEV
                    << "CooperativeDownload::handleLowerMsg::CDCMT_ConnectToAP: "
                    << cdmsg->getTargetId() << endl;
            connectToAP(cdmsg->getTargetId());
            break;
        case CDCMT_ConnectToGCU:
            debugEV
                    << "CooperativeDownload::handleLowerMsg::CDCMT_ConnectToGCU: "
                    << cdmsg->getTargetId() << endl;
            connectToCar(cdmsg->getTargetId());
            break;
        case CDCMT_DisconnectFromAP:
            debugEV
                    << "CooperativeDownload::handleLowerMsg::CDCMT_DisconnectFromAP: "
                    << cdmsg->getTargetId() << endl;
            disconnectFromAP(cdmsg->getTargetId());
            break;
        case CDCMT_DisconnectFromGCU:
            debugEV
                    << "CooperativeDownload::handleLowerMsg::CDCMT_DisconnectFromGCU: "
                    << cdmsg->getTargetId() << endl;
            disconnectFromCar(cdmsg->getTargetId());
            break;
        case CDCMT_DisconnectAll:
            debugEV
                    << "CooperativeDownload::handleLowerMsg::CDCMT_DisconnectAll"
                    << endl;
            selfReset();
            break;
        case CDCMT_UpdatePostion:
            debugEV
                    << "CooperativeDownload::handleLowerMsg::CDCMT_UpdatePostion"
                    << endl;
            break;
        default:
            break;
        }
        delete cdmsg;
    }
}

void CooperativeDownload::sayHelloToAp(int apid) {
    {
        CoDownHelloMsg* cdmsg = new CoDownHelloMsg();
        cdmsg->setMsgType(CDMT_Hello);
        cdmsg->setSrcAddr(gcu->getAddr());
        cdmsg->setDestAddr(gcu->apid());
        cdmsg->setSpeed(fabs(gcu->getSpeed().x));
        cdmsg->setAngle(gcu->getSpeed().x > 0 ? 0 : 180);
        cdmsg->setPosition(gcu->getCurrentPostion().x);
        gcu->sendMsgToAP(gcu->apid(), cdmsg);
    }
}

void CooperativeDownload::sayByeToAp(int apid) {
    {
        CoDownBaseMsg* cdmsg = new CoDownBaseMsg();
        cdmsg->setMsgType(CDMT_Quit);
        cdmsg->setSrcAddr(gcu->getAddr());
        cdmsg->setDestAddr(gcu->apid());
        gcu->sendMsgToAP(gcu->apid(), cdmsg);
    }
}

void CooperativeDownload::disconnectFromCurrentCar() {
    targetID = -1;
    car_Status = CAR_IDEL;
    if (!isTargetCar) {
        cancelEvent(frameTimer);
    }else{
        startSensingProcess();
    }
}

void CooperativeDownload::connectToCar(int carid) {
    untappedCarList.push_back(carid);
    if (isTargetCar) {
        startSensingProcess();
    } else {
        startScanProcess();
    }
}

void CooperativeDownload::disconnectFromCar(int carid) {
    untappedCarList.remove(carid);
    if (carid == targetID) {
        disconnectFromCurrentCar();
    }
}


void CooperativeDownload::askForDownload(int apid) {
    // send CDMT_AskAP message
    {
        CoDownAskAPMsg* cdmsg = new CoDownAskAPMsg();
        cdmsg->setMsgType(CDMT_AskAP);
        cdmsg->setSrcAddr(gcu->getAddr());
        cdmsg->setDestAddr(gcu->apid());
        cdmsg->setStartPos(contentQueueMap[gcu->getAddr()]->getFirstStart());
        cdmsg->setEndPos(contentQueueMap[gcu->getAddr()]->getFirstEnd());
        gcu->sendMsgToAP(gcu->apid(),cdmsg);
    }
}

void CooperativeDownload::selfReset() {
    clearContentMap();
    clearTimeMap();
    untappedCarList.clear();
    disconnectFromCurrentCar();
}

void CooperativeDownload::handleLowerMsg(cMessage* msg) {
    // FIXME add handle functions
    CoDownBaseMsg* cdmsg = check_and_cast<CoDownBaseMsg*>(msg);
    if(cdmsg == NULL){
        delete msg;
    }else{
        switch(cdmsg->getMsgType()){
        case CDMT_AskAP:
            debugEV<<"Error: do not send ASKAP message to a car."<<endl;
            delete cdmsg;
            break;
        case CDMT_Sensor:
            handleSensorMsg(cdmsg);
            break;
        case CDMT_Hello:
            debugEV<<"Error: do not send HELLO message to a car."<<endl;
            break;
        case CDMT_IsBusy:
            handleIsBusyMsg(cdmsg);
            break;
        case CDMT_IsIdle:
            // not handle for now.
            debugEV<<"Error: not handle for now."<<endl;
            delete cdmsg;
            break;
        case CDMT_Send:
            handleSendMsg(cdmsg);
            break;
        case CDMT_Reply:
            handleReplyMsg(cdmsg);
            break;
        case CDMT_APSend:
            handleAPSendMsg(cdmsg);
            break;
        case CDMT_Scan:
            // use to make backup, not handle for now.
            debugEV<<"Error: use to make backup, not handle for now."<<endl;
            delete cdmsg;
            break;
        case CDMT_Negative:
            handleNagativeMsg(cdmsg);
            break;
        case CDMT_Acative:
            // use to make backup, not handle for now.
            debugEV<<"Error: use to make backup, not handle for now."<<endl;
            delete cdmsg;
            break;
        case CDMT_Quit:
            debugEV<<"Error: do not send QUIT message to a car."<<endl;
            delete cdmsg;
            break;
        case CDMT_Request:
            handleRequestMsg(cdmsg);
            break;
        case CDMT_RequestAP:
            debugEV<<"Error: do not send REQUESTAP message to a car."<<endl;
            delete cdmsg;
            break;
        default:
            debugEV<<"Error: get an unknown message from lower layer."<<endl;
            delete cdmsg;
            break;
        }
    }
}

void CooperativeDownload::clearContentMap() {
    for(CD_SQUEUE_MAP::iterator it = contentQueueMap.begin();
            it!=contentQueueMap.end();it++){
        it->second->clear();
    }
    contentQueueMap.clear();
}

void CooperativeDownload::connectToAP(int apid) {
    car_Status = CAR_AP;
    if(isTargetCar){
        askForDownload(apid);
    }else{
        sayHelloToAp(apid);
    }
}

void CooperativeDownload::disconnectFromAP(int apid) {
    if(isTargetCar){
        // sayByeToAp(apid);
    }else{
        sayByeToAp(apid);
    }
}

void CooperativeDownload::handleFrameTimer() {
    // TODO handle the frametimer message
    if (!isTargetCar) {
        switch(car_Status){
        case CAR_SENDING:
            sendContentToCar();
            break;
        case CAR_IDEL:
            break;
        default:
            break;
        }
    }else{
        switch(car_Status){
        case CAR_AP:
            requestContentFromAP();
            break;
        case CAR_IDEL:
            break;
        default:
            break;
        }
    }
}

void CooperativeDownload::sendContentToCar() {
    if (car_Status == CAR_SENDING||car_Status == CAR_PRESENDING) {
        // start sending data to target car
        car_Status = CAR_SENDING;
        SegmentQueue* content = contentQueueMap[targetID]->getFirstSegment(frameSize);
        contentQueueMap[targetID]->remove(*content);
        {
            CoDownContentMsg* cdmsg = new CoDownContentMsg();
            cdmsg->setMsgType(CDMT_Send);
            cdmsg->setSrcAddr(gcu->getAddr());
            cdmsg->setDestAddr(targetID);
            cdmsg->setStartPos(content->getFirstStart());
            cdmsg->setEndPos(content->getFirstEnd());
            cdmsg->setLastMsg(contentQueueMap[targetID]->empty());
            sendDown(cdmsg);
        }
        delete content;
        if(!contentQueueMap[targetID]->empty()){
            scheduleAt(simTime()+frameInterval,frameTimer);
        }
    }else{
        debugEV<<"Error: Start sending process from an unknown status."<<endl;
    }
}

void CooperativeDownload::requestContentFromAP() {
    if(car_Status == CAR_AP){
        SegmentQueue* content = contentQueueMap[gcu->getAddr()]->getFirstSegment(
                APframeSize);
        {
            CoDownRequestMsg* cdmsg = new CoDownRequestMsg();
            cdmsg->setMsgType(CDMT_RequestAP);
            cdmsg->setSrcAddr(gcu->getAddr());
            cdmsg->setStartPos(content->getFirstStart());
            cdmsg->setEndPos(content->getFirstEnd());
            gcu->sendMsgToAP(gcu->apid(),cdmsg);
        }
        delete content;
    }else{
        debugEV<<"Error: Start requestContentFromAP process from a wrong status."<<endl;
    }
}

void CooperativeDownload::requestContentFromCar() {
    car_Status = CAR_SCANING;
    {
        CoDownBaseMsg* cdmsg = new CoDownBaseMsg();
        cdmsg->setMsgType(CDMT_Scan);
        cdmsg->setSrcAddr(gcu->getAddr());
        cdmsg->setDestAddr(*untappedCarList.begin());
        sendDown(cdmsg);
    }
}

void CooperativeDownload::sendSensorMsgToCar() {
    car_Status = CAR_SENSING;
    targetID = *untappedCarList.begin();
    {
        CoDownBaseMsg* cdmsg = new CoDownBaseMsg();
        cdmsg->setMsgType(CDMT_Sensor);
        cdmsg->setSrcAddr(gcu->getAddr());
        cdmsg->setDestAddr(*untappedCarList.begin());
        sendDown(cdmsg);
    }
}

void CooperativeDownload::startSensingProcess() {
    if(car_Status == CAR_IDEL){
        if(!untappedCarList.empty()){
            sendSensorMsgToCar();
        }
    }
}

void CooperativeDownload::startScanProcess() {
    // FIXME do not use for now
    if(car_Status == CAR_IDEL){
        if(!untappedCarList.empty()){
            sendScanMsgToCar();
        }
    }
}

void CooperativeDownload::sendScanMsgToCar() {
    // FIXME do nothing for now
}

void CooperativeDownload::handleSensorMsg(CoDownBaseMsg* msg) {
    // TODO handleSensorMsg();
    if(isTargetCar){
        {
            sendNagativeTo(msg->getSrcAddr());
        }
    }else{
        if (car_Status == CAR_IDEL) {
            if(contentQueueMap.find(msg->getSrcAddr())!= contentQueueMap.end()){
                {
                    CoDownReplyMsg* cdmsg = CoDownReplyMsg();
                    cdmsg->setMsgType(CDMT_Reply);
                    cdmsg->setSrcAddr(gcu->getAddr());
                    cdmsg->setDestAddr(msg->getSrcAddr());
                    cdmsg->setSpeed(fabs(gcu->getSpeed().x));
                    cdmsg->setAngle(gcu->getSpeed().x > 0 ? 0 : 180);
                    cdmsg->setPosition(gcu->getCurrentPostion().x);
                    cdmsg->setStartPos(contentQueueMap[msg->getSrcAddr()]->getFirstStart());
                    cdmsg->setEndPos(contentQueueMap[msg->getSrcAddr()]->getFirstEnd());
                    sendDown(cdmsg);
                }
            } else {
                sendNagativeTo(msg->getSrcAddr());
            }
        }else{
            sendIsBusyTo(msg->getSrcAddr());
        }
    }
    delete msg;
}

void CooperativeDownload::handleReplyMsg(CoDownBaseMsg* msg) {
    // TODO handleReplyMsg();
}

void CooperativeDownload::handleSendMsg(CoDownBaseMsg* msg) {
    // TODO handleSendMsg();
}

void CooperativeDownload::handleAPSendMsg(CoDownBaseMsg* msg) {
    // TODO handleAPSendMsg();

}

void CooperativeDownload::handleRequestMsg(CoDownBaseMsg* msg) {
    // TODO handleRequestMsg();
}

void CooperativeDownload::sendIsBusyTo(int carid) {
    CoDownBaseMsg* cdmsg = new CoDownBaseMsg();
    cdmsg->setMsgType(CDMT_IsBusy);
    cdmsg->setSrcAddr(gcu->getAddr());
    cdmsg->setDestAddr(carid);
    sendDown(cdmsg);
}

void CooperativeDownload::sendNagativeTo(int carid) {
    CoDownBaseMsg* cdmsg = new CoDownBaseMsg();
    cdmsg->setMsgType(CDMT_Negative);
    cdmsg->setSrcAddr(gcu->getAddr());
    cdmsg->setDestAddr(carid);
    sendDown(cdmsg);
}

void CooperativeDownload::handleIsBusyMsg(CoDownBaseMsg* msg) {
    if(msg->getSrcAddr()==targetID){
        untappedCarList.remove(targetID);
        untappedCarList.push_back(targetID);
        scheduleAt(simTime()+frameInterval,frameTimer);
    }else{
        // FIXME a msg from a wrong car.
    }
}

void CooperativeDownload::handleNagativeMsg(CoDownBaseMsg* msg) {
    if(msg->getSrcAddr()==targetID){
        untappedCarList.remove(targetID);
        scheduleAt(simTime()+frameInterval,frameTimer);
    }else{
        // FIXME a msg from a wrong car.
    }
}

void CooperativeDownload::clearTimeMap() {
    for(CD_SQUEUE_MAP::iterator it = timeQueueMap.begin();
            it!=timeQueueMap.end();it++){
        it->second->clear();
    }
    timeQueueMap.clear();
}
