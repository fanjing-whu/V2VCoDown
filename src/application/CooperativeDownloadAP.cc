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

#include "CooperativeDownloadAP.h"

Define_Module(CooperativeDownloadAP);

CooperativeDownloadAP::CooperativeDownloadAP() {
    frameInterval = 0;
    APframeSize = 0;
    maxDALength = 0;
    maxAPRadius = 0;
    minCarSpeed = 0;
    APDLSpeed = 0;
    CarDLSpeed = 0;
    carComRadius = 0;
    DATime0 = 0;
    DATime1 = 0;
    maxDLSizePerAP = 0;
    maxDLSizePerDA = 0;
}

CooperativeDownloadAP::~CooperativeDownloadAP() {
}

void CooperativeDownloadAP::initialize(int stage)
{
    BaseGlobalAppLayer::initialize(stage);
    if(stage == 0){
        gcu->setAddr(getParentModule()->getIndex());
        gcu->isAp(true);
        gcu->setApid(getParentModule()->getIndex());
        Coord pos = Coord(par("x").doubleValue(),par("y").doubleValue(),par("z").doubleValue());
        gcu->setCurrentPostion(pos);

        frameInterval = par("frameInterval").doubleValue();
        APframeSize = par("APframeSize").doubleValue();
        maxDALength = par("maxDALength").doubleValue();
        maxAPRadius = par("maxAPRadius").doubleValue();
        minCarSpeed = par("minCarSpeed").doubleValue();
        APDLSpeed = par("APDLSpeed").doubleValue();
        CarDLSpeed = par("CarDLSpeed").doubleValue();
        carComRadius = par("carComRadius").doubleValue();

        maxDLSizePerAP = (2 * maxAPRadius / minCarSpeed) * APDLSpeed;
        maxDLSizePerDA = (maxDALength / minCarSpeed) * CarDLSpeed;

        getParentModule()->getDisplayString().setTagArg("b", 2, "rect");
        getParentModule()->getDisplayString().setTagArg("b", 3, "red");
        getParentModule()->getDisplayString().setTagArg("b", 4, "red");
        updatePosition();
    }else if(stage == 1){

    }
}

void CooperativeDownloadAP::handleSelfMsg(cMessage* msg) {
    // TODO ap does not handle any self-messages for now.
}

void CooperativeDownloadAP::handleLowerMsg(cMessage* msg) {
    CoDownBaseMsg* cdmsg = check_and_cast<CoDownBaseMsg*>(msg);
    if(cdmsg == NULL){
        delete msg;
    }else{
        switch(cdmsg->getMsgType()){
        case CDMT_AskAP:
            handleAskAPMsg(cdmsg);
            break;
        case CDMT_Sensor:
            debugEV<<"Error: do not send SENSOR message to AP."<<endl;
            delete cdmsg;
            break;
        case CDMT_Hello:
            handleHelloMsg(cdmsg);
            break;
        case CDMT_IsBusy:
            debugEV<<"Error: do not send ISBUSY message to AP."<<endl;
            delete cdmsg;
            break;
        case CDMT_IsIdle:
            debugEV<<"Error: do not send REQUESTAP message to AP."<<endl;
            delete cdmsg;
            break;
        case CDMT_Send:
            debugEV<<"Error: do not send SEND message to AP."<<endl;
            delete cdmsg;
            break;
        case CDMT_Reply:
            debugEV<<"Error: do not send REPLY message to AP."<<endl;
            delete cdmsg;
            break;
        case CDMT_APSend:
            debugEV<<"Error: do not send APSEND message to AP."<<endl;
            delete cdmsg;
            break;
        case CDMT_Scan:
            debugEV<<"Error: do not send REQUESTAP message to AP."<<endl;
            delete cdmsg;
            break;
        case CDMT_Negative:
            debugEV<<"Error: do not send NEGATIVE message to AP."<<endl;
            delete cdmsg;
            break;
        case CDMT_Acative:
            debugEV<<"Error: do not send REQUESTAP message to AP."<<endl;
            delete cdmsg;
            break;
        case CDMT_Quit:
            handleQuitMsg(cdmsg);
            break;
        case CDMT_Request:
            debugEV<<"Error: do not send REQUEST message to AP."<<endl;
            delete cdmsg;
            break;
        case CDMT_RequestAP:
            handleRequestAP(cdmsg);
            break;
        default:
            debugEV<<"Error: get an unknown message from lower layer."<<endl;
            delete cdmsg;
            break;
        }
    }
}

void CooperativeDownloadAP::handleLowerControl(cMessage* msg) {
    // ap does not handle any control messages.
}

void CooperativeDownloadAP::handleAskAPMsg(cMessage* msg) {
    CoDownAskAPMsg* cdmsg = check_and_cast<CoDownAskAPMsg*>(msg);
    if(cdmsg == NULL){
        delete msg;
        return;
    }
    if(cdmsg->getDestAddr()<0){
        handleAskAPMsgFromAP(cdmsg);
    }else{
        handleAskAPMsgFromCar(cdmsg);
    }
}

void CooperativeDownloadAP::handleHelloMsg(cMessage* msg) {
    CoDownHelloMsg* cdmsg = check_and_cast<CoDownHelloMsg*>(msg);
    if(cdmsg == NULL){
        delete msg;
        return;
    }
    _Car_Info car;
    car.angle = cdmsg->getAngle();
    car.postion = cdmsg->getPosition();
    car.speed = cdmsg->getSpeed();
    car.time = simTime().dbl();
    carInfoList[cdmsg->getSrcAddr()] = car;
    debugEV << " car[" << cdmsg->getSrcAddr() << "] is connected to ap["
                   << gcu->getApid() << "];" << endl;
    debugEV << " carInfo[id:" << cdmsg->getSrcAddr() << ",speed:"
                   << carInfoList[cdmsg->getSrcAddr()].speed << ",position:"
                   << carInfoList[cdmsg->getSrcAddr()].postion << "]" << endl;
    untappedCarList.push_back(cdmsg->getSrcAddr());
    choseAssistedCAr();
}

void CooperativeDownloadAP::handleQuitMsg(cMessage* msg) {
    CoDownBaseMsg* cdmsg = check_and_cast<CoDownBaseMsg*>(msg);
    if(cdmsg == NULL){
        delete msg;
        return;
    }
    carInfoList.erase(cdmsg->getSrcAddr());
    untappedCarList.remove(cdmsg->getSrcAddr());
}

void CooperativeDownloadAP::handleRequestAP(cMessage* msg) {
    CoDownRequestAPMsg* cdmsg = check_and_cast<CoDownRequestAPMsg*>(msg);
    if(cdmsg == NULL){
        delete msg;
        return;
    }
    if(cdmsg->getDestAddr()<0){
        handleRequestAPFromCar(cdmsg);
    }else{
        handleRequestAPFromCar(cdmsg);
    }
}

void CooperativeDownloadAP::clearContentMap() {
    for(CDAP_SQUEUE_MAP::iterator it = contentQueueMap.begin();
            it!=contentQueueMap.end();it++){
        it->second->clear();
        delete(it->second);
    }
    contentQueueMap.clear();
}

void CooperativeDownloadAP::finish() {
    clearContentMap();
    clearTimeMap();
    carInfoList.clear();
}

void CooperativeDownloadAP::handleAskAPMsgFromCar(CoDownAskAPMsg* msg) {
    int carid = msg->getSrcAddr();
    double speed = msg->getSpeed();
    double angle = msg->getAngle();
    double startPos = msg->getStartPos();
    double endPos = msg->getEndPos();
    double position = msg->getPosition();
    double taskSize = endPos - startPos;
    delete msg;
    // 1st. add car infomation into carInfoList
    _Car_Info carInfo;
    carInfo.time = simTime().dbl();
    carInfo.angle = angle;
    carInfo.postion = position;
    carInfo.speed = speed;
    if(carInfoList.find(carid)!=carInfoList.end()){
        carInfoList.erase(carid);
        carInfoList[carid] = carInfo;
        debugEV<<"Error: this car has already registered at this ap."<<endl;
    }else{
        carInfoList[carid] = carInfo;
    }
    targetCarList[carid] = TS_INRANGE;
    // 2nd. allocate the task to next ap
    SegmentQueue* contentTotal = new SegmentQueue(startPos,endPos);
    debugEV<<"maxDLSizePerAP"<<maxDLSizePerAP<<endl;
    if(taskSize>maxDLSizePerAP){
        SegmentQueue* content = contentTotal->getFirstSegment(maxDLSizePerAP);
        contentTotal->remove(*content);
        if(contentQueueMap.find(carid)!=contentQueueMap.end()){
            contentQueueMap[carid]->add(*content);
            delete(content);
        }else{
            contentQueueMap[carid] = content;
        }
        {
            // send CDMT_AskAP message
            {
                CoDownAskAPMsg* cdmsg = new CoDownAskAPMsg();
                cdmsg->setMsgType(CDMT_AskAP);
                cdmsg->setSrcAddr(carid);
                cdmsg->setDestAddr(-1);
                cdmsg->setSpeed(speed);
                cdmsg->setAngle(angle);
                cdmsg->setStartPos(contentTotal->getFirstStart());
                cdmsg->setEndPos(contentTotal->getFirstEnd());
                cdmsg->setPosition(position);
                gcu->sendMsgToAP(gcu->getAddr()+1,cdmsg);
            }
        }
        delete contentTotal;
    }else{
        if (contentQueueMap.find(carid)!=contentQueueMap.end()) {
            contentQueueMap[carid]->add(*contentTotal);
            delete (contentTotal);
        } else {
            contentQueueMap[carid] = contentTotal;
        }
    }
    // 3rd. send content to this car.
    sendContentToCar(carid);
}

void CooperativeDownloadAP::handleAskAPMsgFromAP(CoDownAskAPMsg* msg) {
    int carid = msg->getSrcAddr();
    int apnum = msg->getDestAddr();
    double speed = msg->getSpeed();
    double angle = msg->getAngle();
    double startPos = msg->getStartPos();
    double endPos = msg->getEndPos();
    double position = msg->getPosition();
    double taskSize = endPos - startPos;
    delete msg;
    // 1st. add car infomation into carInfoList
    _Car_Info carInfo;
    carInfo.time = simTime().dbl();
    carInfo.angle = angle;
    carInfo.postion = position;
    carInfo.speed = speed;
    if(carInfoList.find(carid)!=carInfoList.end()){
        carInfoList.erase(carid);
        carInfoList[carid] = carInfo;
        debugEV<<"Error: this car has already registered at this ap."<<endl;
    }else{
        carInfoList[carid] = carInfo;
    }
    if(apnum == -1){
        activatedTargetList.insert(carid);
        targetCarList[carid] = TS_PREAP0;
        DATime0 = simTime().dbl()+(maxAPRadius*2 + maxDALength/2)/speed;
        DATime1 = simTime().dbl()+(maxAPRadius*2 + maxDALength)/speed;
    }else if(apnum == -2){
        activatedTargetList.insert(carid);
        targetCarList[carid] = TS_PREAP1;
        DATime0 = simTime().dbl()+(maxAPRadius*4 + maxDALength)/speed;
        DATime1 = simTime().dbl()+(maxAPRadius*4 + 3*maxDALength/2)/speed;
    }else{
        targetCarList[carid] = TS_OUTRANGE;
    }
    // 2nd. allocate the task to next ap
    SegmentQueue* contentTotal = new SegmentQueue(startPos,endPos);
    double mineTaskSize = apnum == -1?maxDLSizePerAP+maxDLSizePerDA/2:maxDLSizePerAP+maxDLSizePerDA;
    debugEV<<"mineTaskSize"<<mineTaskSize<<endl;
    if(taskSize>mineTaskSize){
        SegmentQueue* content = contentTotal->getFirstSegment(mineTaskSize);
        contentTotal->remove(*content);
        if(contentQueueMap.find(carid)!=contentQueueMap.end()){
            contentQueueMap[carid]->add(*content);
            delete(content);
        }else{
            contentQueueMap[carid] = content;
            timeQueueMap[carid] = new SegmentQueue();
        }
        {
            // send CDMT_AskAP message
            {
                CoDownAskAPMsg* cdmsg = new CoDownAskAPMsg();
                cdmsg->setMsgType(CDMT_AskAP);
                cdmsg->setSrcAddr(carid);
                cdmsg->setDestAddr(apnum-1);
                cdmsg->setSpeed(speed);
                cdmsg->setAngle(angle);
                cdmsg->setStartPos(contentTotal->getFirstStart());
                cdmsg->setEndPos(contentTotal->getFirstEnd());
                cdmsg->setPosition(position);
                gcu->sendMsgToAP(gcu->getAddr()+1,cdmsg);
            }
        }
        delete contentTotal;
    }else{
        if (contentQueueMap.find(carid)!=contentQueueMap.end()) {
            contentQueueMap[carid]->add(*contentTotal);
            delete (contentTotal);
        } else {
            contentQueueMap[carid] = contentTotal;
            timeQueueMap[carid] = new SegmentQueue();
        }
    }
    // 3rd. the car is still out range.
}

void CooperativeDownloadAP::handleRequestAPFromCar(CoDownRequestAPMsg* msg) {
    int carid = msg->getSrcAddr();
    double speed = msg->getSpeed();
    double angle = msg->getAngle();
    // double startPos = msg->getStartPos();
    // double endPos = msg->getEndPos();
    double position = msg->getPosition();
    // double taskSize = endPos - startPos;
    delete msg;
    debugEV<<"get request AP message from car "<<carid<<endl;
    // 1st. update the carlist.
    if(targetCarList[carid]!=TS_INRANGE){
        // send CDMT_AskAP message
        targetCarList[carid] = TS_INRANGE;
        {
            CoDownRequestAPMsg* cdmsg = new CoDownRequestAPMsg();
            cdmsg->setMsgType(CDMT_RequestAP);
            cdmsg->setSrcAddr(carid);
            cdmsg->setDestAddr(-1);
            cdmsg->setSpeed(speed);
            cdmsg->setAngle(angle);
            // cdmsg->setStartPos(startPos);
            // cdmsg->setEndPos(endPos);
            cdmsg->setPosition(position);
            gcu->sendMsgToAP(gcu->getAddr() + 1, cdmsg);
        }
    }
    // 2nd. send content to this car.
    sendContentToCar(carid);
}

void CooperativeDownloadAP::handleRequestAPFromAP(CoDownRequestAPMsg* msg) {
    int carid = msg->getSrcAddr();
    int apnum = msg->getDestAddr();
    double speed = msg->getSpeed();
    double angle = msg->getAngle();
    // double startPos = msg->getStartPos();
    // double endPos = msg->getEndPos();
    double position = msg->getPosition();
    // double taskSize = endPos - startPos;
    delete msg;
    // 1st. update the carlist.
    if (apnum == -1) {
        targetCarList[carid] = TS_PREAP0;
        DATime0 = simTime().dbl()+(maxAPRadius*2 + maxDALength/2)/speed;
        DATime1 = simTime().dbl()+(maxAPRadius*2 + maxDALength)/speed;
        if(activatedTargetList.find(carid)==activatedTargetList.end()){
            debugEV<<"Error: the car must be activated before here."<<endl;
        }
        // send CDMT_AskAP message
        {
            CoDownRequestAPMsg* cdmsg = new CoDownRequestAPMsg();
            cdmsg->setMsgType(CDMT_RequestAP);
            cdmsg->setSrcAddr(carid);
            cdmsg->setDestAddr(apnum - 1);
            cdmsg->setSpeed(speed);
            cdmsg->setAngle(angle);
            // cdmsg->setStartPos(startPos);
            // cdmsg->setEndPos(endPos);
            cdmsg->setPosition(position);
            gcu->sendMsgToAP(gcu->getAddr() + 1, cdmsg);
        }
    }else if(apnum == -2){
        targetCarList[carid] = TS_PREAP1;
        DATime0 = simTime().dbl()+(maxAPRadius*4 + maxDALength)/speed;
        DATime1 = simTime().dbl()+(maxAPRadius*4 + 3*maxDALength/2)/speed;
        activatedTargetList.insert(carid);
    }
    // 2nd. do nothing
}

void CooperativeDownloadAP::sendContentToCar(int carid) {
    debugEV<<"start send content to car "<<carid<<endl;
    SegmentQueue* content = contentQueueMap[carid]->getFirstSegment(
            APframeSize);
    contentQueueMap[carid]->remove(*content);
    {
        CoDownContentMsg* cdmsg = new CoDownContentMsg();
        cdmsg->setMsgType(CDMT_Send);
        cdmsg->setSrcAddr(-1);
        cdmsg->setDestAddr(carid);
        cdmsg->setStartPos(content->getFirstStart());
        cdmsg->setEndPos(content->getFirstEnd());
        cdmsg->setLastMsg(contentQueueMap[carid]->empty());
        sendDelayedDown(cdmsg, frameInterval);
    }
    delete content;
}

void CooperativeDownloadAP::choseAssistedCAr() {
    debugEV<<"CooperativeDownloadAP::choseAssistedCAr:start"<<endl;
    int target = 0;
    int assistor = 0;
    for(std::set<int>::iterator it_target = activatedTargetList.begin();it_target!=activatedTargetList.end();it_target++){
        target = *it_target;
        int delta = 0;
        if(targetCarList[*it_target] == TS_PREAP0){
            delta = 1;
        }else if(targetCarList[*it_target] == TS_PREAP1){
            delta = 2;
        }else{
            debugEV<<"Error: this car must not be activated car."<<endl;
        }

        debugEV<<"untappedCarList.size(): "<<untappedCarList.size()<<endl;
        for(std::list<int>::iterator it_car = untappedCarList.begin();it_car!=untappedCarList.end();){
            assistor = *it_car;
            bool flag = true;
            double bTime = carInfoList[assistor].time
                    + ((delta + 1) * (2 * maxAPRadius) + delta * maxDALength
                            - (carInfoList[assistor].time
                                    - carInfoList[target].time)
                                    * carInfoList[target].speed
                            - carComRadius)
                            / (carInfoList[assistor].speed
                                    + carInfoList[target].speed);
            double eTime = bTime
                    + 2 * carComRadius
                            / (carInfoList[assistor].speed
                                    + carInfoList[target].speed);
            if(bTime>DATime0&&eTime<DATime1){
                SegmentQueue* duration = new SegmentQueue(bTime,eTime);
                duration->remove(*timeQueueMap[target]);
                SegmentQueue* time = duration->getFirstSegment();
                if(time->length()>2.0){
                    sendAPContentToCar(target,assistor,time);
                    assistedCarList.push_back(assistor);
                    it_car = untappedCarList.erase(it_car);
                    flag = false;
                }
                delete duration;
                delete time;
            }
            if(it_car!=untappedCarList.end()){
                if(flag){
                    it_car++;
                }else{
                    flag = true;
                }
            }
        }
    }
}

void CooperativeDownloadAP::sendAPContentToCar(int target, int assistor , SegmentQueue* time) {
    SegmentQueue* content = contentQueueMap[target]->getFirstSegment(APDLSpeed * time->length());
    contentQueueMap[target]->remove(*content);
    timeQueueMap[target]->add(*time);
    {
        CoDownAPContentMsg* cdmsg = new CoDownAPContentMsg();
        cdmsg->setMsgType(CDMT_APSend);
        cdmsg->setSrcAddr(gcu->getAddr());
        cdmsg->setDestAddr(assistor);
        cdmsg->setStartPos(content->getFirstStart());
        cdmsg->setEndPos(content->getFirstEnd());
        cdmsg->setStartTime(time->getFirstStart());
        cdmsg->setEndTime(time->getFirstEnd());
        cdmsg->setTargetId(target);
        sendDelayedDown(cdmsg, frameInterval);
    }
    delete content;
}

void CooperativeDownloadAP::updatePosition() {
    EV<< "updatePosition: " << gcu->getCurrentPostion().info() << endl;

    if(ev.isGUI())
    {
        std::ostringstream osDisplayTag;
        cDisplayString& disp = findHost()->getDisplayString();

        // setup output stream
        osDisplayTag << std::fixed; osDisplayTag.precision(5);
        osDisplayTag << ( gcu->getCurrentPostion().x);
        disp.setTagArg("p", 0, osDisplayTag.str().data());

        osDisplayTag.str("");// reset
        osDisplayTag << ( gcu->getCurrentPostion().y - 50);
        disp.setTagArg("p", 1, osDisplayTag.str().data());
    }
}

void CooperativeDownloadAP::clearTimeMap() {
    for(CDAP_SQUEUE_MAP::iterator it = timeQueueMap.begin();
            it!=timeQueueMap.end();it++){
        it->second->clear();
        delete(it->second);
    }
    timeQueueMap.clear();
}
