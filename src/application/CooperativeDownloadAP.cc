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

#include "CooperativeDownloadAP.h"

Define_Module(CooperativeDownloadAP);

CooperativeDownloadAP::CooperativeDownloadAP() {
}

CooperativeDownloadAP::~CooperativeDownloadAP() {
}

void CooperativeDownloadAP::initialize(int stage)
{
    BaseGlobalAppLayer::initialize(stage);
    if(stage == 0){
        gcu->setAddr(getId());
        gcu->isAp(true);
        gcu->apid(getIndex());
    }else if(stage == 1){
    }
}

void CooperativeDownloadAP::handleSelfMsg(cMessage* msg) {
}

void CooperativeDownloadAP::handleLowerMsg(cMessage* msg) {
}

void CooperativeDownloadAP::handleLowerControl(cMessage* msg) {
}
