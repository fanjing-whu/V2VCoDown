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

#include "CooperativeDownload.h"

Define_Module(CooperativeDownload);

CooperativeDownload::CooperativeDownload() {
}

CooperativeDownload::~CooperativeDownload() {
}

void CooperativeDownload::initialize(int stage)
{
    BaseGlobalAppLayer::initialize(stage);
    if(stage == 0){
        gcu->setAddr(getParentModule()->getId());
        gcu->isAp(false);
        gcu->apid(-1);
    }else if(stage == 1){
    }
}

void CooperativeDownload::handleSelfMsg(cMessage* msg) {
}

void CooperativeDownload::handleLowerControl(cMessage* msg) {
    CoDownBaseMsg* cdmsg =check_and_cast<CoDownBaseMsg*>(msg);
    if(cdmsg == NULL){
        EV<<"Error: CooperativeDownload::get a wrong type message."<< endl;
    }
    switch(cdmsg->getMsgType()){
    case CDCMT_ConnectToAP:
        EV<<"CooperativeDownload::handleLowerMsg::CDCMT_ConnectToAP"<<endl;
        break;
    case CDCMT_ConnectToGCU:
        EV<<"CooperativeDownload::handleLowerMsg::CDCMT_ConnectToGCU"<<endl;
        break;
    case CDCMT_DisconnectFromAP:
        EV<<"CooperativeDownload::handleLowerMsg::CDCMT_DisconnectFromAP"<<endl;
        break;
    case CDCMT_DisconnectFromGCU:
        EV<<"CooperativeDownload::handleLowerMsg::CDCMT_DisconnectFromGCU"<<endl;
        break;
    case CDCMT_DisconnectAll:
        EV<<"CooperativeDownload::handleLowerMsg::CDCMT_DisconnectAll"<<endl;
        selfReset();
        delete msg;
        msg = NULL;
        break;
    case CDCMT_UpdatePostion:
        EV<<"CooperativeDownload::handleLowerMsg::CDCMT_UpdatePostion"<<endl;
        break;
    default:
        delete msg;
        msg = NULL;
        break;
    }
}

void CooperativeDownload::selfReset() {

}

void CooperativeDownload::handleLowerMsg(cMessage* msg) {
}
