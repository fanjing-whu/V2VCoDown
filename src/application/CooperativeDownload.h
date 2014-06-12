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
 * TODO - Generated class
 */
#define CD_SQUEUE_MAP std::map<int,SegmentQueue*>
class CooperativeDownload: public BaseGlobalAppLayer {
public:
    CooperativeDownload();
    virtual ~CooperativeDownload();
public:
    virtual void initialize(int stage);
public:
    void handleSelfMsg(cMessage *msg);
    void handleLowerControl(cMessage *msg);
    void handleLowerMsg(cMessage *msg);
private:
    void sayHelloToAp(int apid);
    void sayByeToAp(int apid);
    void selfReset();
    CD_SQUEUE_MAP contentQueueMap;

    bool coreDebug;
    bool debug;

    bool isTargetCar;
    double taskSize;

    enum CarStatus{
        CAR_IDEL = 0,
        CAR_BUSY = 1
    };

    CarStatus status;
};

#endif
