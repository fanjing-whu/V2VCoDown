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
	CDMT_Base = 0;	// do not use this msg type
	CDMT_Hello = 1;	// 
	CDMT_IsBusy = 2;	// reply for hello msg
	CDMT_IsIdle = 3;	//
	CDMT_Send = 4;	// 
	CDMT_Request = 5;	// from target to coorperative car
	CDMT_Scan = 6;	// use for making back-up
	CDMT_Negative = 7;	// 
	CDMT_Acative = 8;	// 
}
packet CoDownBaseMsg extends ApplPkt {
    int msgType @enum(CoDownMsgType);
}
packet CoDownContentMsg extends CoDownBaseMsg{
    int startPos;
    int endPos;
    bool lastMsg;
}