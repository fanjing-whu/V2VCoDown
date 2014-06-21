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

#ifndef __SIMPLEVEINS_COOPERATIVEDOWNLOADAP_H_
#define __SIMPLEVEINS_COOPERATIVEDOWNLOADAP_H_

#include <omnetpp.h>
#include <map>
#include "BaseGlobalAppLayer.h"
#include "SegmentQueue.h"
#include "CoDownMsg_m.h"


/**
 *
 */
#define CDAP_SQUEUE_MAP std::map<int,SegmentQueue*>
class CooperativeDownloadAP : public BaseGlobalAppLayer
{
public:
    CooperativeDownloadAP();
    virtual ~CooperativeDownloadAP();
protected:
    virtual void initialize(int stage);
    virtual void finish();

public:
    void handleSelfMsg(cMessage *msg);
    void handleLowerMsg(cMessage *msg);
    void handleLowerControl(cMessage* msg);
private:

    // handle lower message.
    void handleAskAPMsg(cMessage* msg);
    void handleHelloMsg(cMessage* msg);
    void handleQuitMsg(cMessage* msg);
    void handleRequestAP(cMessage* msg);

    // private functional method
    void handleAskAPMsgFromCar(CoDownAskAPMsg* msg);
    void handleAskAPMsgFromAP(CoDownAskAPMsg* msg);
    void handleRequestAPFromCar(CoDownRequestAPMsg* msg);
    void handleRequestAPFromAP(CoDownRequestAPMsg* msg);

    void sendContentToCar(int carid);
    void sendAPContentToCar(int target, int assistor ,SegmentQueue* time);
    void choseAssistedCAr();
    void clearContentMap();
    void clearTimeMap();

private:
    CDAP_SQUEUE_MAP timeQueueMap;
    CDAP_SQUEUE_MAP contentQueueMap;
    typedef struct {
        double speed;
        double angle;
        double postion;
        double time;
    } _Car_Info;
    std::map<int,_Car_Info> carInfoList;
    std::list<int> untappedCarList;
    std::set<int> activatedTargetList;
    enum _Target_Status{
        TS_INRANGE = 0,
        TS_PREAP0,
        TS_PREAP1,
        TS_OUTRANGE
    };
    std::map<int,_Target_Status> targetCarList;
    std::list<int> assistedCarList;
    double frameInterval;
    double APframeSize;
    double maxDALength;
    double maxAPRadius; // double of the ap radius
    double minCarSpeed;
    double APDLSpeed;
    double CarDLSpeed;
    double carComRadius;

    double DATime0; // the min time can be used in chose assisted car.
    double DATime1; // the max time can be used in chose assisted car.

    double maxDLSizePerAP;
    double maxDLSizePerDA;
};

#endif
