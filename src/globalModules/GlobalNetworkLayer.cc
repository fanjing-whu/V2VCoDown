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
    EV<<"GlobalNetworkLayer::initialize(stage = "<< stage <<")"<<endl;
}

void GlobalNetworkLayer::handleMessage(cMessage *msg)
{
    std::cout<<"The GlobalNetworkLayer does not handle any message."<<std::endl;
    delete(msg);
}

void GlobalNetworkLayer::registerGCU(IGlobalControlUnit* gcu) {
    Enter_Method_Silent();
    if(gcu->isAp()){
        apMap[gcu->getAddr()] = gcu;
    }else{
        gcuMap[gcu->getAddr()] = gcu;
        gcuSortedList.push_back(gcu);
        sortGCUList();
    }
}

GlobalNetworkLayer::GlobalNetworkLayer() {
}

GlobalNetworkLayer::~GlobalNetworkLayer() {
}

void GlobalNetworkLayer::unregisterGCU(IGlobalControlUnit* gcu) {
    Enter_Method_Silent();
    if(gcu->isAp()){
        gcu->disconnectAll();
        apMap.erase(gcu->getAddr());
    }else{
        gcu->disconnectAll();
        gcuMap.erase(gcu->getAddr());
        gcuSortedList.remove(gcu);
    }
}

void GlobalNetworkLayer::refreshGCU(IGlobalControlUnit* gcu) {
    Enter_Method_Silent();
    //disconnect the out-range GCU
    {
        GNL_IGCU_MAP oldNeighbors = GNL_IGCU_MAP(*(gcu->getNeighbors()));
        for(GNL_IGCU_MAP::iterator it = oldNeighbors.begin();it!=oldNeighbors.end();it++){
            if(!gcu->isInRange(it->second)){
                gcu->disconnectFromGCU(it->second);
            }
        }
    }
    //connect the in-range GCU
    {
        sortGCUList();
        // find this GCU
        GNL_IGCU_LIST::iterator thisGCU;
        for (thisGCU = gcuSortedList.begin(); gcu != *thisGCU; thisGCU++) {
            ASSERT2(thisGCU != gcuSortedList.end(), "Error: this GCU is unregistered.");
        }
        //connect to the front GCU
        GNL_IGCU_LIST::iterator it = thisGCU;
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
    // refresh the connect with AP
    {
        GNL_IGCU_MAP::iterator it;
        for (it = apMap.begin();; it++) {
            if( it==apMap.end()) {
                gcu->disconnectFromAP(gcu->apid());
                break;
            }else if(gcu->isInRange(it->second)){
                ASSERT2(!gcu->hasAp()||gcu->apid()==it->second->apid(), "Error: this GCU already has an other AP connected.");
                gcu->connectToAP(it->second->apid());
            }
        }
    }
}

void GlobalNetworkLayer::sendMsg(cMessage* msg) {
    Enter_Method_Silent();
    ApplPkt* pkt = check_and_cast<ApplPkt*>(msg);
    gcuMap[pkt->getDestAddr()]->handleMsgFromNetwLayer(msg);
}

void GlobalNetworkLayer::sendMsgToAP(int apid, cMessage* msg) {
    Enter_Method_Silent();
    apMap[apid]->handleMsgFromNetwLayer(msg);
}

void GlobalNetworkLayer::sortGCUList() {
    gcuSortedList.sort();
}

int GlobalNetworkLayer::convertIPtoIndex(LAddress::L3Type ip) {
    return ip;
}

LAddress::L3Type GlobalNetworkLayer::convertIndextoIP(int index) {
    return index;
}
