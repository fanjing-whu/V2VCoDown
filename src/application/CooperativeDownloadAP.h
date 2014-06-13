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

public:
    void handleSelfMsg(cMessage *msg);
    void handleLowerMsg(cMessage *msg);
    void handleLowerControl(cMessage* msg);
private:
    CDAP_SQUEUE_MAP timeQueueMap;
    CDAP_SQUEUE_MAP contentQueueMap;
};

#endif
