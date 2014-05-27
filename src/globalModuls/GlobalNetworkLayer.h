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

#ifndef __SIMPLEVEINS_GLOBALNETWORKLAYER_H_
#define __SIMPLEVEINS_GLOBALNETWORKLAYER_H_

#include <omnetpp.h>
#include <map>
#include <list>
#include "IGlobalControlUnit.h"
#include "IGlobalNetworkLayer.h"
/**
 * TODO - Generated class
 */
#define GNL_IGCU_MAP std::map<int, IGlobalControlUnit*>
#define GNL_IGCU_LIST std::list<IGlobalControlUnit*>


class GlobalNetworkLayer : public cSimpleModule,
                           public IGlobalNetworkLayer
{

public:
    GlobalNetworkLayer();
    virtual ~GlobalNetworkLayer();
  protected:
    void initialize(int stage);
    void handleMessage(cMessage *msg);

  public:
    void registerGCU(IGlobalControlUnit* gcu);
    void unregisterGCU(IGlobalControlUnit* gcu);
    void refreshGCU(IGlobalControlUnit* gcu);

  private:
    void sortGCUList(){
        gcuSortedList.sort();
    }
    GNL_IGCU_MAP gcuMap;
    GNL_IGCU_LIST gcuSortedList;

};

#endif
