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

#ifndef __SIMPLEVEINS_COOPERATIVEDOWNLOAD_H_
#define __SIMPLEVEINS_COOPERATIVEDOWNLOAD_H_

#include <omnetpp.h>
#include "BaseGlobalAppLayer.h"
#include "SegmentQueue.h"
#include "CoDownMsg_m.h"

/**
 *
 */
#define CD_SQUEUE_MAP std::map<int,SegmentQueue*>
class CooperativeDownload: public BaseGlobalAppLayer {
public:
    CooperativeDownload();
    virtual ~CooperativeDownload();
public:
    virtual void initialize(int stage);
    virtual void finish();
public:
    void handleSelfMsg(cMessage *msg);
    void handleLowerControl(cMessage *msg);
    void handleLowerMsg(cMessage *msg);

private:
    // handle control messages.
    void connectToAP(int apid); // when connect to an AP
    void disconnectFromAP(int apid);    // an AP disconnected
    void connectToCar(int carid);   // when connect to a car
    void disconnectFromCar(int carid);  // a car disconnected

    // handle self messages.
    void handleFrameTimer();

    // handle lower message.
    void handleSensorMsg(CoDownBaseMsg* msg);
    void handleReplyMsg(CoDownBaseMsg* msg);
    void handleSendMsg(CoDownBaseMsg* msg);
    void handleAPSendMsg(CoDownBaseMsg* msg);
    void handleRequestMsg(CoDownBaseMsg* msg);
    void handleIsBusyMsg(CoDownBaseMsg* msg);
    void handleNagativeMsg(CoDownBaseMsg* msg);

    // private functional method
    void askForDownload(int apid);  // start the task
    void sayHelloToAp(int apid);    // start when a CO-CAR gets into AP range
    void sayByeToAp(int apid);  // CO-CAR leave AP range
    void disconnectFromCurrentCar();    // target car disconnect
    void selfReset();   // when this car run out of the broad
    void startSensingProcess(); // start sensing process if target CAR
    void startScanProcess(); // start scaning process if CO-CAR
    void sendContentToCar();    // send content to target car
    void requestContentFromAP();    // request content from an AP
    void sendSensorMsgToCar();  // target CAR sends sensor message to a car
    void sendReplyMsgToCar();   // send reply message after recived the sensor message.
    void sendScanMsgToCar();    // CO-CAR scan other car
    void requestContentFromCar();   //request content from a car.
    void sendIsBusyTo(int carid);
    void sendNagativeTo(int carid);
    void changeToIdel();
    void makeRecord();

    void clearContentMap();
    void clearTimeMap();
private:
    CD_SQUEUE_MAP contentQueueMap;
    CD_SQUEUE_MAP timeQueueMap;
    std::list<int> untappedCarList;

    bool coreDebug;
    bool debug;

    bool isTargetCar;
    double taskSize;
    double frameSize;
    double frameInterval;
    double APframeSize;
    cMessage* frameTimer;

    enum CarStatus{
        CAR_INIT = 0,
        CAR_IDEL,
        CAR_BUSY,
        CAR_SENSING,
        CAR_SCANING,
        CAR_PRESENDING,
        CAR_SENDING,
        CAR_RECEIVING,
        CAR_AP
    };
    int targetID;
    CarStatus car_Status;
};

#endif
