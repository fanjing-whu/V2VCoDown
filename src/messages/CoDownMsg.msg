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

cplusplus {{
#include "ApplPkt_m.h"
}}
packet ApplPkt;

//
// TODO generated message class
//
enum CoDownMsgType{
    
    // Base message types
	CDMT_RequestAP = 0;	//S ask AP for download
	CDMT_Sensor = 1;	//B send the sensor message
	CDMT_Hello = 2;		//S hello message
	CDMT_IsBusy = 3;	//B reply for hello msg
	CDMT_IsIdle = 4;	//B reply for hello msg
	CDMT_Send = 5;		//S send content to target
	CDMT_Reply = 6;		//S replay for the sensor message
	CDMT_APSend = 7;	//S send content to co-car from AP
	CDMT_Scan = 8;		//B use for making back-up
	CDMT_Negative = 9;	//B reply for scan msg
	CDMT_Acative = 10;	//B reply for scan msg
	CDMT_Quit = 11;		//B leave AP range
	CDMT_Request = 12;	//S request content from car
	
	// Control message types
	CDCMT_ConnectToGCU = 100;
	CDCMT_ConnectToAP = 101;
	CDCMT_DisconnectFromGCU = 102;
	CDCMT_DisconnectFromAP = 103;
	CDCMT_DisconnectAll = 104;
	CDCMT_UpdatePostion = 105;
}
packet CoDownBaseMsg extends ApplPkt {
    int msgType @enum(CoDownMsgType);
}
packet CoDownRequestAPMsg extends CoDownBaseMsg{
    double speed;
    double angle;
    double startPos;
    double endPos;
    double position;
}
packet CoDownHelloMsg extends CoDownBaseMsg{
    double speed;
    double angle;
    double position;
}
packet CoDownReplyMsg extends CoDownBaseMsg{
    double speed;
    double angle;
    double startPos;
    double endPos;
    double position;
} 
packet CoDownRequestMsg extends CoDownBaseMsg{
    double startPos;
    double endPos;
}
packet CoDownAPContentMsg extends CoDownBaseMsg{
    double startTime;
    double endTime;
    double startPos;
    double endPos;
    int targetId;
}
packet CoDownContentMsg extends CoDownBaseMsg{
    double startPos;
    double endPos;
    bool lastMsg;
}
packet CoDownCtrlMsg extends CoDownBaseMsg{
    int targetId;
}