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
    coreDebug = false;
    debug = false;
    isTargetCar = false;
    taskSize = 0;
    frameSize = 0;
    frameInterval = 0;
    APframeSize = 0;
    frameTimer = 0;
    frameTimer = NULL;
    targetID = -1;
    car_Status = CAR_INIT;
    unfinishedTask = 0;
}

CooperativeDownload::~CooperativeDownload() {
}

void CooperativeDownload::finish() {
    if (isTargetCar) {
        recordScalar("test", unfinishedTask);
    }
    clearContentMap();
    clearTimeMap();
    cancelAndDelete(frameTimer);
}
void CooperativeDownload::initialize(int stage)
{
    BaseGlobalAppLayer::initialize(stage);
    if(stage == 0){
        // initialize the gcu module
        gcu->setAddr(getParentModule()->getId());
        gcu->isAp(false);
        gcu->setApid(-1);
        // initialize other parameters
        coreDebug = par("coreDebug").boolValue();
        debug = par("debug").boolValue();
        isTargetCar = par("isTargetCar").boolValue();
        taskSize = par("taskSize").doubleValue();
        frameSize = par("frameSize").doubleValue();
        frameInterval = par("frameInterval").doubleValue();
        APframeSize = par("APframeSize").doubleValue();
        frameTimer = new cMessage("frameTimer");

        car_Status = CAR_IDEL;
        if(isTargetCar){
            car_Status = CAR_INIT;
            unfinishedTask = 0;
            debugEV<<"the id of the target car is "<<gcu->getAddr()<<endl;
            contentQueueMap[gcu->getAddr()] = new SegmentQueue(0,taskSize);
        }
        WATCH(unfinishedTask);
    }else if(stage == 1){

    }
}

void CooperativeDownload::handleSelfMsg(cMessage* msg) {
    if(msg == frameTimer){
        handleFrameTimer();
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
                    << "CooperativeDownload::handleLowerControlMsg::CDCMT_ConnectToAP: "
                    << cdmsg->getTargetId() << endl;
            connectToAP(cdmsg->getTargetId());
            break;
        case CDCMT_ConnectToGCU:
            debugEV
                    << "CooperativeDownload::handleLowerControlMsg::CDCMT_ConnectToGCU: "
                    << cdmsg->getTargetId() << endl;
            connectToCar(cdmsg->getTargetId());
            break;
        case CDCMT_DisconnectFromAP:
            debugEV
                    << "CooperativeDownload::handleLowerControlMsg::CDCMT_DisconnectFromAP: "
                    << cdmsg->getTargetId() << endl;
            disconnectFromAP(cdmsg->getTargetId());
            break;
        case CDCMT_DisconnectFromGCU:
            debugEV
                    << "CooperativeDownload::handleLowerControlMsg::CDCMT_DisconnectFromGCU: "
                    << cdmsg->getTargetId() << endl;
            disconnectFromCar(cdmsg->getTargetId());
            break;
        case CDCMT_DisconnectAll:
            debugEV
                    << "CooperativeDownload::handleLowerControlMsg::CDCMT_DisconnectAll"
                    << endl;
            selfReset();
            break;
        case CDCMT_UpdatePostion:
            debugEV
                    << "CooperativeDownload::handleLowerControlMsg::CDCMT_UpdatePostion"
                    << endl;
            makeRecord();
            break;
        default:
            break;
        }
        delete cdmsg;
    }
}

void CooperativeDownload::sayHelloToAp(int apid) {
    car_Status = CAR_AP;
    {
        CoDownHelloMsg* cdmsg = new CoDownHelloMsg();
        cdmsg->setMsgType(CDMT_Hello);
        cdmsg->setSrcAddr(gcu->getAddr());
        cdmsg->setDestAddr(gcu->getApid());
        cdmsg->setSpeed(fabs(gcu->getSpeed().x));
        cdmsg->setAngle(gcu->getSpeed().x > 0 ? 0 : 180);
        cdmsg->setPosition(gcu->getCurrentPostion().x);
        gcu->sendMsgToAP(gcu->getApid(), cdmsg);
    }
}

void CooperativeDownload::sayByeToAp(int apid) {
    {
        CoDownBaseMsg* cdmsg = new CoDownBaseMsg();
        cdmsg->setMsgType(CDMT_Quit);
        cdmsg->setSrcAddr(gcu->getAddr());
        cdmsg->setDestAddr(gcu->getApid());
        gcu->sendMsgToAP(gcu->getApid(), cdmsg);
    }
}

void CooperativeDownload::disconnectFromCurrentCar() {
    targetID = -1;
    if(car_Status != CAR_AP){
        changeToIdel();
    }
}

void CooperativeDownload::connectToCar(int carid) {
    untappedCarList.push_back(carid);
    if (isTargetCar) {
        debugEV<<"startSensingProcess:"<<car_Status<<endl;
        startSensingProcess();
    } else {
        startScanProcess();
    }
}

void CooperativeDownload::disconnectFromCar(int carid) {
    untappedCarList.remove(carid);
    CD_SQUEUE_MAP::iterator it = contentQueueMap.find(carid);
    if(it!=contentQueueMap.end()){
        it->second->clear();
        delete(it->second);
        contentQueueMap.erase(it);
    }
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
        cdmsg->setDestAddr(gcu->getApid());
        cdmsg->setSpeed(gcu->getSpeed().x);
        cdmsg->setAngle(gcu->getSpeed().x>0?0:180);
        cdmsg->setStartPos(contentQueueMap[gcu->getAddr()]->getFirstStart());
        cdmsg->setEndPos(contentQueueMap[gcu->getAddr()]->getFirstEnd());
        cdmsg->setPosition(gcu->getCurrentPostion().x);
        gcu->sendMsgToAP(gcu->getApid(),cdmsg);
    }
}

void CooperativeDownload::selfReset() {
    clearContentMap();
    clearTimeMap();
    untappedCarList.clear();
    changeToIdel();
    disconnectFromCurrentCar();
}

void CooperativeDownload::handleLowerMsg(cMessage* msg) {
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
            delete cdmsg;
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
        delete(it->second);
    }
    contentQueueMap.clear();
}

void CooperativeDownload::connectToAP(int apid) {
    if(isTargetCar){
        if(car_Status == CAR_INIT){
            car_Status = CAR_AP;
            askForDownload(apid);
        }else{
            car_Status = CAR_AP;
            requestContentFromAP();
        }
    }else{
        sayHelloToAp(apid);
    }
}

void CooperativeDownload::disconnectFromAP(int apid) {
    if(isTargetCar){
        // sayByeToAp(setApid);
        changeToIdel();
    }else{
        sayByeToAp(apid);
        changeToIdel();
    }
}

void CooperativeDownload::handleFrameTimer() {
    if (!isTargetCar) {
        switch(car_Status){
        case CAR_SENDING:
            sendContentToCar();
            break;
        case CAR_IDEL:
            startScanProcess();
            break;
        default:
            break;
        }
    }else{
        switch(car_Status){
        case CAR_AP:
            debugEV<<"handleFrameTimer:AP"<<endl;
            requestContentFromAP();
            break;
        case CAR_IDEL:
            debugEV<<"handleFrameTimer:IDEL"<<endl;
            startSensingProcess();
            break;
        case CAR_SENDING:
            debugEV<<"handleFrameTimer:SENDING"<<endl;
            requestContentFromCar();
        default:
            debugEV<<"handleFrameTimer:default"<<endl;
            break;
        }
    }
}

void CooperativeDownload::sendContentToCar() {
    if (car_Status == CAR_SENDING) {
        // start sending data to target car
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
            sendDelayedDown(cdmsg,frameInterval);
        }
        delete content;
        if(contentQueueMap[targetID]->empty()){
            disconnectFromCurrentCar();
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
            CoDownRequestAPMsg* cdmsg = new CoDownRequestAPMsg();
            cdmsg->setMsgType(CDMT_RequestAP);
            cdmsg->setSrcAddr(gcu->getAddr());
            cdmsg->setDestAddr(gcu->getApid());
            cdmsg->setSpeed(gcu->getSpeed().x);
            cdmsg->setAngle(gcu->getSpeed().x>0?0:180);
            cdmsg->setStartPos(content->getFirstStart());
            cdmsg->setEndPos(content->getFirstEnd());
            cdmsg->setPosition(gcu->getCurrentPostion().x);
            gcu->sendMsgToAP(gcu->getApid(),cdmsg);
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
        cdmsg->setMsgType(CDMT_Request);
        cdmsg->setSrcAddr(gcu->getAddr());
        cdmsg->setDestAddr(targetID);
        sendDown(cdmsg);
    }
}

void CooperativeDownload::sendSensorMsgToCar() {
    if(car_Status == CAR_AP||untappedCarList.empty()){
        return;
    }
    car_Status = CAR_PRESENDING;
    targetID = *(untappedCarList.begin());
    debugEV<<"sendSensorMsgToCar: "<<targetID<<endl;
    {
        CoDownBaseMsg* cdmsg = new CoDownBaseMsg();
        cdmsg->setMsgType(CDMT_Sensor);
        cdmsg->setSrcAddr(gcu->getAddr());
        cdmsg->setDestAddr(targetID);
        sendDown(cdmsg);
    }
}

void CooperativeDownload::startSensingProcess() {
    if(car_Status == CAR_IDEL){
        if(!untappedCarList.empty()){
            sendSensorMsgToCar();
        }else{
            //changeToIdel();
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
    debugEV<<"handleSensorMsgFrom: "<<msg->getSrcAddr()<<endl;
    if(isTargetCar){
        untappedCarList.remove(msg->getSrcAddr());
        sendNagativeTo(msg->getSrcAddr());
    }else{
        if (car_Status == CAR_IDEL) {
            targetID = msg->getSrcAddr();
            sendReplyMsgToCar();
        }else{
            sendIsBusyTo(msg->getSrcAddr());
        }
    }
    delete msg;
}

void CooperativeDownload::handleReplyMsg(CoDownBaseMsg* msg) {
    CoDownReplyMsg* cdmsg = check_and_cast<CoDownReplyMsg*>(msg);
    if(cdmsg == NULL){
        debugEV<<"Error: wrong message type!"<<endl;
        delete msg;
        return;
    }
    if(cdmsg->getSrcAddr() == targetID&& car_Status == CAR_PRESENDING){
        // decide to download data or not
        // caculate the contact time
        double time = (gcu->getCurrentSpeed().x
                - (cdmsg->getSpeed() * cos(PI * cdmsg->getAngle() / 180)))
                / (cdmsg->getPosition() - gcu->getCurrentPostion().x
                        + 2 * gcu->getSendPower());
        debugEV<<"collide time:"<<time<<endl;
        if(time >= 1.0){
            SegmentQueue* content = new SegmentQueue(cdmsg->getStartPos(),cdmsg->getEndPos());
            if(contentQueueMap[gcu->getAddr()]->isCollide(*content)){
                car_Status = CAR_SENDING;
                // after 1.0 second start request message from this car.
                scheduleAt(simTime()+1.0-frameInterval,frameTimer);
            }else{
                untappedCarList.remove(targetID);
                sendNagativeTo(targetID);
                changeToIdel();
            }
            delete content;
        }else{
            untappedCarList.remove(targetID);
            sendNagativeTo(targetID);
            changeToIdel();
        }
    }
    delete cdmsg;
}

void CooperativeDownload::handleSendMsg(CoDownBaseMsg* msg) {
    CoDownContentMsg* cdmsg = check_and_cast<CoDownContentMsg*>(msg);
    if(cdmsg == NULL){
        debugEV<<"Error: wrong message type!"<<endl;
        delete msg;
        return;
    }
    SegmentQueue* content = new SegmentQueue(cdmsg->getStartPos(),cdmsg->getEndPos());
    contentQueueMap[gcu->getAddr()]->remove(*content);
    debugEV<<"contentQueueMap->Length():"<<contentQueueMap[gcu->getAddr()]->length()<<endl;
    if(cdmsg->getLastMsg()){
        if (car_Status != CAR_AP) {
            changeToIdel();
            if (cdmsg->getSrcAddr() > 0) {
                untappedCarList.remove(cdmsg->getSrcAddr());
            }
        }else{
            // FIXME add the process in ap-range
        }
    }else{
        if(car_Status == CAR_AP){
            requestContentFromAP();
        }else if(car_Status == CAR_SENDING){
            requestContentFromCar();
        }
    }
    delete content;
    delete cdmsg;
}

void CooperativeDownload::handleAPSendMsg(CoDownBaseMsg* msg) {
    CoDownAPContentMsg* cdmsg = check_and_cast<CoDownAPContentMsg*>(msg);
    if(cdmsg==NULL){
        delete msg;
        return;
    }
    if(isTargetCar){
        // do nothing
    }else{
        if(contentQueueMap.find(cdmsg->getTargetId()) != contentQueueMap.end()){
            contentQueueMap[cdmsg->getTargetId()]
                            = new SegmentQueue(cdmsg->getStartPos(),cdmsg->getEndPos());
            timeQueueMap[cdmsg->getTargetId()]
                         = new SegmentQueue(cdmsg->getStartTime(),cdmsg->getEndTime());
        }
    }
    delete cdmsg;
}

void CooperativeDownload::handleRequestMsg(CoDownBaseMsg* msg) {
    CoDownRequestMsg* cdmsg = check_and_cast<CoDownRequestMsg*>(msg);
    if(cdmsg == NULL){
        delete msg;
        return;
    }
    if(!isTargetCar){
        if(targetID == cdmsg->getSrcAddr()){
            car_Status = CAR_SENDING;
            sendContentToCar();
        }else{
            // get request from a wrong car.
            debugEV<<"Error: get request from a wrong car"<<endl;
        }
    }else{
        // target Car does not handle request message.
        debugEV<<"Error: target Car does not handle request message."<<endl;
    }
    delete cdmsg;
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
        if(car_Status != CAR_AP){
            untappedCarList.remove(targetID);
            untappedCarList.push_back(targetID);
            changeToIdel();
        }
    }else{
        // FIXME a msg from a wrong car.
        debugEV<<"Error: a msg from a wrong car."<<endl;
    }
    delete msg;
}

void CooperativeDownload::handleNagativeMsg(CoDownBaseMsg* msg) {
    if(msg->getSrcAddr()==targetID){
        if(car_Status != CAR_AP){
            untappedCarList.remove(targetID);
            changeToIdel();
        }
    }else{
        // FIXME a msg from a wrong car.
        debugEV<<"Error: a msg from a wrong car."<<endl;
    }
    delete msg;
}

void CooperativeDownload::sendReplyMsgToCar() {
    if(contentQueueMap.find(targetID)!= contentQueueMap.end()){
        car_Status = CAR_PRESENDING;
        {
            CoDownReplyMsg* cdmsg = new CoDownReplyMsg();
            cdmsg->setMsgType(CDMT_Reply);
            cdmsg->setSrcAddr(gcu->getAddr());
            cdmsg->setDestAddr(targetID);
            cdmsg->setSpeed(fabs(gcu->getSpeed().x));
            cdmsg->setAngle(gcu->getSpeed().x > 0 ? 0 : 180);
            cdmsg->setPosition(gcu->getCurrentPostion().x);
            cdmsg->setStartPos(contentQueueMap[targetID]->getFirstStart());
            cdmsg->setEndPos(contentQueueMap[targetID]->getFirstEnd());
            sendDown(cdmsg);
        }
    } else {
        untappedCarList.remove(targetID);
        sendNagativeTo(targetID);
        changeToIdel();
    }
}

void CooperativeDownload::changeToIdel() {
    if(isTargetCar){
        if(car_Status!=CAR_INIT){
            car_Status = CAR_IDEL;
            targetID = -1;
            cancelEvent(frameTimer);
            scheduleAt(simTime()+frameInterval,frameTimer);
        }
    }else{
        if(contentQueueMap.empty()){
            car_Status = CAR_IDEL;
            targetID = -1;
            cancelEvent(frameTimer);
        }else{
            // start making backup, do nothing now
            car_Status = CAR_IDEL;
            targetID = -1;
            cancelEvent(frameTimer);
            //scheduleAt(simTime()+frameInterval,frameTimer);
        }
    }
}

void CooperativeDownload::makeRecord() {
    if (isTargetCar) {
        ASSERT(contentQueueMap.find(gcu->getAddr())!=contentQueueMap.end());
        ASSERT(contentQueueMap[gcu->getAddr()]!=NULL);
        debugEV<<"makeRecord()"<<endl;
        unfinishedTask = contentQueueMap[gcu->getAddr()]->length();
        if(gcu->getCurrentPostion().x>19900.0){
            endSimulation();
        }
    }
}

void CooperativeDownload::clearTimeMap() {
    for(CD_SQUEUE_MAP::iterator it = timeQueueMap.begin();
            it!=timeQueueMap.end();it++){
        it->second->clear();
        delete(it->second);
    }
    timeQueueMap.clear();
}
