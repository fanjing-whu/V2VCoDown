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

#ifndef IGLOBALCONTROLUNIT_H_
#define IGLOBALCONTROLUNIT_H_

#include "Coord.h"
#include <map>

#define GCU_IGCU_MAP std::map<int, IGlobalControlUnit*>
class IGlobalControlUnit {
public:
    IGlobalControlUnit() :
            neighbors(), hasAp(false), apid(0), lastPos(Coord::ZERO),sendPower(0),receivePower(0){
    }
    virtual ~IGlobalControlUnit() {
    }
public:
    inline bool operator< (const IGlobalControlUnit& rhs){
        return this->getCurrentPostion().x<rhs.getCurrentPostion().x;
    }

public:
    virtual int getAddr() = 0;
    virtual void setCurrentPostion(Coord pos) = 0;
    virtual Coord getCurrentPostion() = 0;
    virtual void handleMsgFromNetwLayer(cMessage* msg) = 0;
public:
    bool isAp(){
        return isAp;
    }
    bool isConnectedTo(IGlobalControlUnit* gcu){
        return neighbors.find(gcu->getAddr())!=neighbors.end();
    }
    bool isInRange(IGlobalControlUnit* gcu){
        return (getDistFrom(gcu)<=gcu->receivePower+sendPower)&&(getDistFrom(gcu)<=receivePower+gcu->sendPower);
    }
    double getDistFrom(IGlobalControlUnit* gcu){
        return fabs(this->getCurrentPostion().x-gcu->getCurrentPostion().x);
    }
    void connectToGCU(IGlobalControlUnit* gcu) {
        neighbors[gcu->getAddr()] = gcu;
    }
    void disconnectFromGCU(IGlobalControlUnit* gcu) {
        if (neighbors.erase(gcu->getAddr()) > 0) {
            gcu->disconnectFromGCU(this);
        }
    }
    void disconnectAll() {
        for (GCU_IGCU_MAP::iterator it = neighbors.begin();
                it != neighbors.end(); it++) {
            this->disconnectFromGCU(it->second);
        }
    }
    void connectToAP(int apid) {
        hasAp = true;
        this->apid = apid;
    }
    void disconnectFromAP(int apid) {
        if (this->apid == apid) {
            hasAp = false;
            this->apid = 0;
        }
    }
    bool hasAp(){
        return hasAp;
    }
    int apid(){
        return hasAp?apid:-1;
    }
    GCU_IGCU_MAP* getNeighbors() {
        return &neighbors;
    }
protected:
    GCU_IGCU_MAP neighbors;
    bool isAp;
    bool hasAp;
    int apid;
    Coord lastPos;
    double sendPower;
    double receivePower;
};

#endif /* IGLOBALCONTROLUNIT_H_ */
