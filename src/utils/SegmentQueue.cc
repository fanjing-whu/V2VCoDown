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

#include "SegmentQueue.h"
#include <iostream>

SegmentQueue::SegmentQueue() {
}

SegmentQueue::SegmentQueue(double start, double end) {
	init(start, end);
}

SegmentQueue::~SegmentQueue() {
    for(SQueue::iterator it = sq.begin();it!=sq.end();){
        removeNode(it);
    }
}

void SegmentQueue::remove(SegmentQueue& sq) {
    SQueue::iterator itThis = this->sq.begin();
    SQueue::iterator itSq = sq.sq.begin();
    bool flagThis = false;
    bool flagSq = false;
    while(itSq!=sq.sq.end()&&itThis!=this->sq.end()){
        if(!flagThis){
            //0:--|
            if(itThis == this->sq.end()){
                std::cout<<"The loop should have been terminated in this situation."<<std::endl;
            }
            if((*itThis)->value > (*itSq)->value){
                if(flagSq){
                    //0:-----|+
                    //1:+|-????
                    //r:-----|+
                    flagSq = (*itSq)->flag;
                    itSq++;
                }else{
                    //0:-----|+
                    //1:-|+????
                    //r:-----|+
                    flagSq = (*itSq)->flag;
                    itSq++;
                }
            }else if((*itThis)->value == (*itSq)->value){
                if(flagSq){
                    //0:-|+
                    //1:+|-
                    //r:-|+
                    flagThis = (*itThis)->flag;
                    itThis++;
                    flagSq = (*itSq)->flag;
                    itSq++;
                }else{
                    //0:-|+
                    //1:-|+
                    //r:-|+
                    flagThis = (*itThis)->flag;
                    removeNode(itThis);
                    flagSq = (*itSq)->flag;
                    itSq++;
                }
            }else if((*itThis)->value < (*itSq)->value){
                if(flagSq){
                    //0:-|+????
                    //1:+++++|-
                    //r:---????
                    flagThis = (*itThis)->flag;
                    removeNode(itThis);
                }else{
                    //0:-|+????
                    //1:-----|+
                    //r:-|+????
                    flagThis = (*itThis)->flag;
                    itThis++;
                }
            }else{
                std::cout<<"Error: This situation cannot happen."<<std::endl;
            }
        }else{
            //0:++|
            if(itThis == this->sq.end()){
                //0:++;
                std::cout<<"Error: The flag cannot be true while itThis is the end of this segment queue ";
                std::cout<<"and the loop should have been terminated in this situation."<<std::endl;
            }else if((*itThis)->value > (*itSq)->value){
                if(flagSq){
                    //0:+++++|-
                    //1:+|-????
                    //r:-|+++|-
                    addNode(itThis,(*itSq)->value,true);
                    flagSq = (*itSq)->flag;
                    itSq++;
                }else{
                    //0:+++++|-
                    //1:-|+????
                    //r:+|-?+|-
                    addNode(itThis,(*itSq)->value,false);
                    flagSq = (*itSq)->flag;
                    itSq++;
                }
            }else if((*itThis)->value == (*itSq)->value){
                if(flagSq){
                    //0:+|-
                    //1:+|-
                    //r:---
                    flagThis = (*itThis)->flag;
                    removeNode(itThis);
                    flagSq = (*itSq)->flag;
                    itSq++;
                }else{
                    //0:+|-
                    //1:-|+
                    //r:+|-
                    flagThis = (*itThis)->flag;
                    itThis++;
                    flagSq = (*itSq)->flag;
                    itSq++;
                }
            }else if((*itThis)->value < (*itSq)->value){
                if(flagSq){
                    //0:+|-????
                    //1:+++++|-
                    //r:---????
                    flagThis = (*itThis)->flag;
                    removeNode(itThis);
                }else{
                    //0:+|-????
                    //1:-----|+
                    //r:+|-????
                    flagThis = (*itThis)->flag;
                    itThis++;
                }
            }else{
                std::cout<<"Error: This situation cannot happen."<<std::endl;
            }
        }
    }
}

void SegmentQueue::add(SegmentQueue &sq) {
    SQueue::iterator itThis = this->sq.begin();
    SQueue::iterator itSq = sq.sq.begin();
    bool flagThis = false;
    bool flagSq = false;
    while(itSq!=sq.sq.end()){
        if(!flagThis){
            //0:--|
            if(itThis == this->sq.end()){
                if(flagSq){
                    //0:-----;
                    //1:+|-???
                    //r:+|-?-;
                    addNode(itThis,(*itSq)->value,(*itSq)->flag);
                    flagSq = (*itSq)->flag;
                    itSq++;
                }else{
                    //0:-----;
                    //1:-|+???
                    //r:-|+?-;
                    addNode(itThis, (*itSq)->value, (*itSq)->flag);
                    flagSq = (*itSq)->flag;
                    itSq++;
                }
            }else if((*itThis)->value > (*itSq)->value){
                if(flagSq){
                    //0:-----|+
                    //1:+|-????
                    //r:+|-?-|+
                    addNode(itThis,(*itSq)->value,(*itSq)->flag);
                    flagSq = (*itSq)->flag;
                    itSq++;
                }else{
                    //0:-----|+
                    //1:-|+????
                    //r:-|+?-|+
                    addNode(itThis, (*itSq)->value, (*itSq)->flag);
                    flagSq = (*itSq)->flag;
                    itSq++;
                }
            }else if((*itThis)->value == (*itSq)->value){
                if(flagSq){
                    //0:-|+
                    //1:+|-
                    //r:+++
                    flagThis = (*itThis)->flag;
                    removeNode(itThis);
                    flagSq = (*itSq)->flag;
                    itSq++;
                }else{
                    //0:-|+
                    //1:-|+
                    //r:-|+
                    flagThis = (*itThis)->flag;
                    itThis++;
                    flagSq = (*itSq)->flag;
                    itSq++;
                }
            }else if((*itThis)->value < (*itSq)->value){
                if(flagSq){
                    //0:-|+????
                    //1:+++++|-
                    //r:+++????
                    flagThis = (*itThis)->flag;
                    removeNode(itThis);
                }else{
                    //0:-|+????
                    //1:-----|+
                    //r:-|+????
                    flagThis = (*itThis)->flag;
                    itThis++;
                }
            }else{
                std::cout<<"Error: This situation cannot happen."<<std::endl;
            }
        }else{
            //0:++|
            if(itThis == this->sq.end()){
                //0:++;
                std::cout<<"Error: The flag cannot be true while itThis is the end of this segment queue."<<std::endl;
            }else if((*itThis)->value > (*itSq)->value){
                if(flagSq){
                    //0:+++++|-
                    //1:+|-????
                    //r:+++++|-
                    flagSq = (*itSq)->flag;
                    itSq++;
                }else{
                    //0:+++++|-
                    //1:-|+????
                    //r:+++++|-
                    flagSq = (*itSq)->flag;
                    itSq++;
                }
            }else if((*itThis)->value == (*itSq)->value){
                if(flagSq){
                    //0:+|-
                    //1:+|-
                    //r:+|-
                    flagThis = (*itThis)->flag;
                    itThis++;
                    flagSq = (*itSq)->flag;
                    itSq++;
                }else{
                    //0:+|-?
                    //1:-|+?
                    //r:+++?
                    flagThis = (*itThis)->flag;
                    removeNode(itThis);
                    flagSq = (*itSq)->flag;
                    itSq++;
                }
            }else if((*itThis)->value < (*itSq)->value){
                if(flagSq){
                    //0:+|-????
                    //1:+++++|-
                    //r:+++????
                    flagThis = (*itThis)->flag;
                    removeNode(itThis);
                }else{
                    //0:+|-????
                    //1:-----|+
                    //r:+|-????
                    flagThis = (*itThis)->flag;
                    itThis++;
                }
            }else{
                std::cout<<"Error: This situation cannot happen."<<std::endl;
            }
        }
    }
}

void SegmentQueue::output() {
    std::cout<<":";
    for(SQueue::iterator it = sq.begin();it!=sq.end();it++){
        std::cout<<(*it)->value;
        if((*it)->flag){
        	std::cout<<"+++";
        }else{
        	std::cout<<"---";
        }
    }
    std::cout<<std::endl;
}

SegmentQueue* SegmentQueue::copy() {
}

void SegmentQueue::clean() {
}

void SegmentQueue::init(double start, double end) {
    SQueue::iterator it = sq.begin();
    if(it!=sq.end()){
        std::cout<<"Error: You must call the INIT function before insert any other nodes."<<std::endl;
        return;
    }
    addNode(it,start,true);
    addNode(it,end,false);
}

void SegmentQueue::removeNode(SQueue::iterator &it) {
    delete((*it));
    // (*it) = NULL;
    it = sq.erase(it);
}

void SegmentQueue::addNode(SQueue::iterator &it, double value, bool flag) {
    _Container* e = new _Container();
    e->value = value;
    e->flag = flag;
    sq.insert(it,e);
}
