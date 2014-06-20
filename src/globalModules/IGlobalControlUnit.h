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
            neighbors(),
            m_isAp(false),
            m_hasAp(false),
            m_apid(0),
            lastPos(Coord::ZERO),
            lastSpeed(Coord::ZERO),
            sendPower(0),
            receivePower(0){
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
    virtual void connectToGCU(IGlobalControlUnit* gcu) {
        neighbors[gcu->getAddr()] = gcu;
    }
    virtual void disconnectFromGCU(IGlobalControlUnit* gcu) {
        if (neighbors.erase(gcu->getAddr()) > 0) {
            gcu->disconnectFromGCU(this);
        }
    }
    virtual void disconnectAll() {
        for (GCU_IGCU_MAP::iterator it = neighbors.begin();it != neighbors.end(); it++) {
            this->disconnectFromGCU(it->second);
        }
    }
    virtual void connectToAP(int apid) {
        ASSERT2(m_hasAp == false, "Error: Cannot disconnect form this AP, since this GCU has already connect to an AP.");
        m_hasAp = true;
        this->m_apid = apid;
    }
    virtual void disconnectFromAP(int apid) {
        ASSERT2(m_hasAp == true && this->m_apid == apid, "Error: Cannot disconnect form this AP, since this GCU dose not connect to it.");
        m_hasAp = false;
        this->m_apid = 0;
    }
public:
    bool isAp(){
        return m_isAp;
    }
    void isAp(bool isAp){
        this->m_isAp = isAp;
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
    bool hasAp(){
        return m_hasAp;
    }
    int getApid(){
        return m_hasAp?m_apid:-1;
    }
    void setApid(int apid){
        this->m_apid = apid;
    }
    GCU_IGCU_MAP* getNeighbors() {
        return &neighbors;
    }

    double getReceivePower() const
    {
        return receivePower;
    }

    void setReceivePower(double receivePower)
    {
        this->receivePower = receivePower;
    }

    double getSendPower() const
    {
        return sendPower;
    }

    void setSendPower(double sendPower)
    {
        this->sendPower = sendPower;
    }

protected:
    GCU_IGCU_MAP neighbors;
    bool m_isAp;
    bool m_hasAp;
    int m_apid;
    Coord lastPos;
    Coord lastSpeed;
    Coord averageSpeed;
    double sendPower;
    double receivePower;
};

#endif /* IGLOBALCONTROLUNIT_H_ */
