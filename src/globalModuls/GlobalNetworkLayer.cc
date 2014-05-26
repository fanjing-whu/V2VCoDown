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

#include "GlobalNetworkLayer.h"

Define_Module(GlobalNetworkLayer);

void GlobalNetworkLayer::initialize(int stage)
{
    // TODO - Generated method body
}

void GlobalNetworkLayer::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}

void GlobalNetworkLayer::registerGCU(IGlobalControlUnit* gcu) {
    gcuMap[gcu->getAddr()] = gcu;
    gcuSortedList.insert(gcu);
    sortGCUList();
}

GlobalNetworkLayer::GlobalNetworkLayer() {
}

GlobalNetworkLayer::~GlobalNetworkLayer() {
}

void GlobalNetworkLayer::unregisterGCU(IGlobalControlUnit* gcu) {
    gcu->disconnectAll();
    gcuMap.erase(gcu->getAddr());
    gcuSortedList.remove(gcu);
}

void GlobalNetworkLayer::refreshGCU(IGlobalControlUnit* gcu) {
    //disconnect the out-range GCU
    {
        GlobalControlUnitMap oldNeighbors = GlobalControlUnitMap(gcu->getNeighbors());
        for(GlobalControlUnitMap::iterator it = oldNeighbors.begin();it!=oldNeighbors.end();it++){
            if(!gcu->isInRange(it->second)){
                gcu->disconnectFromGCU(it->second);
            }
        }
    }
    //connect the in-range GCU
    {
        sortGCUList();
        // find this GCU
        GlobalControlUnitList::iterator thisGCU;
        for (thisGCU = gcuSortedList.begin(); gcu != *thisGCU; thisGCU++) {
            ASSERT2(*thisGCU != gcuSortedList.end(), "Error: this GCU is unregistered.");
        }
        //connect to the front GCU
        GlobalControlUnitList::iterator it = thisGCU;
        for(it++;it!=gcuSortedList.end()&&gcu->isInRange(*it);it++){
            if(!gcu->isConnectedTo(*it)){
                gcu->connectToGCU(*it);
            }
        }
        //connect to the behind GCU
        it = thisGCU;
        for(it--;it!=gcuSortedList.begin()&&gcu->isInRange(*it);it++){
            if(!gcu->isConnectedTo(*it)){
                gcu->connectToGCU(*it);
            }
        }
    }
}
