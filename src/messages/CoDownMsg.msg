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
	CDMT_Request = 0;	// ask AP for download
	CDMT_Hello = 1;	// hello message
	CDMT_IsBusy = 2;	// reply for hello msg
	CDMT_IsIdle = 3;	//
	CDMT_Send = 4;	// send content to target
	CDMT_APSend = 5;	// send the sensor message
	CDMT_Reply = 6;	// replay for the sensor message
	CDMT_Scan = 7;	// use for making back-up
	CDMT_Negative = 8;	// 
	CDMT_Acative = 9;	// 
	CDMT_Quit = 10;	// leave AP range
	CDCMT_ConnectToGCU = 11;
	CDCMT_ConnectToAP = 12;
	CDCMT_DisconnectFromGCU = 13;
	CDCMT_DisconnectFromAP = 14;
	CDCMT_DisconnectAll = 15;
	CDCMT_UpdatePostion = 16;
}
packet CoDownBaseMsg extends ApplPkt {
    int msgType @enum(CoDownMsgType);
}
packet CoDownRequestMsg extends CoDownBaseMsg{
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