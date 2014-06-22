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
    IGlobalControlUnit(){
    }
    virtual ~IGlobalControlUnit() {
    }
public:
    inline bool operator< (IGlobalControlUnit& rhs){
        return this->getCurrentPostion().x<rhs.getCurrentPostion().x;
    }

public:
    virtual int getAddr() = 0;
    virtual void setCurrentPostion(Coord pos) = 0;
    virtual Coord getCurrentPostion() = 0;
    virtual void setCurrentSpeed(Coord speed) = 0;
    virtual Coord getCurrentSpeed() = 0;
    virtual void setSpeed(Coord speed) = 0;
    virtual Coord getSpeed() = 0;
    virtual void handleMsgFromNetwLayer(cMessage* msg) = 0;
    virtual void connectToGCU(IGlobalControlUnit* gcu) = 0;
    virtual void disconnectFromGCU(IGlobalControlUnit* gcu)  = 0;
    virtual void disconnectAll()  = 0;
    virtual void connectToAP(int apid) = 0;
    virtual void disconnectFromAP(int apid)  = 0;
    virtual bool isAp() = 0;
    virtual void isAp(bool isAp) = 0;
    virtual bool isConnectedTo(IGlobalControlUnit* gcu) = 0;
    virtual bool isInRange(IGlobalControlUnit* gcu) = 0;
    virtual double getDistFrom(IGlobalControlUnit* gcu) = 0;
    virtual bool hasAp() = 0;
    virtual int getApid() = 0;
    virtual void setApid(int apid) = 0;
    virtual GCU_IGCU_MAP* getNeighbors() = 0;

    virtual double getReceivePower() const = 0;

    virtual void setReceivePower(double receivePower) = 0;

    virtual double getSendPower() const = 0;

    virtual void setSendPower(double sendPower) = 0;

};

#endif /* IGLOBALCONTROLUNIT_H_ */
