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
        // initialize the gcu module
        gcu->setAddr(getParentModule()->getId());
        gcu->isAp(false);
        gcu->apid(-1);
        // initialize other parameters
        coreDebug = par("coreDebug").boolValue();
        debug = par("debug").boolValue();
        isTargetCar = par("isTargetCar").boolValue();
        taskSize = par("taskSize").doubleValue();

        status = CAR_IDEL;
    }else if(stage == 1){

    }
}

void CooperativeDownload::handleSelfMsg(cMessage* msg) {
}

void CooperativeDownload::handleLowerControl(cMessage* msg) {
    CoDownCtrlMsg* cdmsg =check_and_cast<CoDownCtrlMsg*>(msg);
    if(cdmsg == NULL){
        debugEV <<"Error: CooperativeDownload::get a wrong type message."
                << endl;
    }
    switch(cdmsg->getMsgType()){
    case CDCMT_ConnectToAP:
        debugEV <<"CooperativeDownload::handleLowerMsg::CDCMT_ConnectToAP: "
                << cdmsg->getTargetId()
                <<endl;
        // TODO
        break;
    case CDCMT_ConnectToGCU:
        debugEV <<"CooperativeDownload::handleLowerMsg::CDCMT_ConnectToGCU: "
                << cdmsg->getTargetId()
                <<endl;
        // TODO
        break;
    case CDCMT_DisconnectFromAP:
        debugEV <<"CooperativeDownload::handleLowerMsg::CDCMT_DisconnectFromAP: "
                << cdmsg->getTargetId()
                <<endl;
        // TODO
        break;
    case CDCMT_DisconnectFromGCU:
        debugEV <<"CooperativeDownload::handleLowerMsg::CDCMT_DisconnectFromGCU: "
                << cdmsg->getTargetId()
                <<endl;
        // TODO
        break;
    case CDCMT_DisconnectAll:
        debugEV <<"CooperativeDownload::handleLowerMsg::CDCMT_DisconnectAll"
                <<endl;
        selfReset();
        delete msg;
        msg = NULL;
        break;
    case CDCMT_UpdatePostion:
        debugEV <<"CooperativeDownload::handleLowerMsg::CDCMT_UpdatePostion"
                <<endl;
        break;
    default:
        delete msg;
        msg = NULL;
        break;
    }
}

void CooperativeDownload::selfReset() {
    contentQueueMap.clear();
}

void CooperativeDownload::handleLowerMsg(cMessage* msg) {
}
