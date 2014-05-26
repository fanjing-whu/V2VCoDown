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

class IGlobalControlUnit {
public:
    IGlobalControlUnit() :
            neighbors(), hasAp(false), apid(0), lastPos(Coord::ZERO) {
    }
    virtual ~IGlobalControlUnit() {
    }
protected:
    typedef std::map<int, IGlobalControlUnit*> IGlobalControlUnitMap;
public:
    virtual int getAddr() = 0;
    virtual Coord getCurrentPostion() = 0;
    virtual void setCurrentPostion(Coord pos) = 0;
    virtual void sendMsgToAP(int apid, cMessage *msg) = 0;
    virtual void connectToGCU(IGlobalControlUnit* gcu) {
        neighbors[gcu->getAddr()] = gcu;
    }
    virtual void disconnectFromGCU(IGlobalControlUnit* gcu) {
        if (neighbors.erase(gcu->getAddr()) > 0) {
            gcu->disconnectFromGCU(this);
        }
    }
    virtual void disconnectAll() {
        for (IGlobalControlUnitMap::iterator it = neighbors.begin();
                it != neighbors.end(); it++) {
            this->disconnectFromGCU(it->second);
        }
    }
    virtual void connectToAP(int apid) {
        hasAp = true;
        this->apid = apid;
    }
    virtual void disconnectFromAP(int apid) {
        if (this->apid == apid) {
            hasAp = false;
            this->apid = 0;
        }
    }
    virtual IGlobalControlUnitMap* getNeighbors() {
        return &neighbors;
    }
protected:
    IGlobalControlUnitMap neighbors;
    bool hasAp;
    int apid;
    Coord lastPos;
};

#endif /* IGLOBALCONTROLUNIT_H_ */
