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

SegmentQueue::~SegmentQueue() {
}

void SegmentQueue::remove(SegmentQueue sq) {
}

void SegmentQueue::add(SegmentQueue sq) {
    SQueue::iterator itThis = this->sq.begin();
    SQueue::iterator itSq = sq.sq.begin();
    bool flagThis = false;
    bool flagSq = false;
    while(itSq!=sq.sq.end()){
        if(!flagThis){
            //--|--
            if(itThis == this->sq.end()||(*itThis)->value > (*itSq)->value){
                if(flagSq){
                    // --|++
                    // +|-xx
                    // +||++
                    addNode(itThis,(*itSq)->value,(*itSq)->flag);
                    flagSq = (*itSq)->flag;
                    itSq++;
                }else{
                    // --|++
                    // -|+xx
                    // +||++
                    addNode(itThis, (*itSq)->value, (*itSq)->flag);
                    flagSq = (*itSq)->flag;
                    itSq++;
                }
            }else if((*itThis)->value == (*itSq)->value){
                if(flagSq){
                    flagThis = (*itThis)->flag;
                    removeNode(itThis);
                }else{
                    flagSq = (*itSq)->flag;
                    if(!flagSq){
                        std::cout<<"Error: the flagSq must be true here."<<std::endl;
                    }
                    itSq++;
                }
            }else if((*itThis)->value < (*itSq)->value){
                if(flagSq){
                    flagThis = (*itThis)->flag;
                    removeNode(itThis);
                }else{

                }
            }
        }
    }
}

void SegmentQueue::add(double start, double end) {

}

void SegmentQueue::removeNode(SQueue::iterator it) {
    delete(*it);
    // (*it) = NULL;
    this->sq.erase(it);
}

void SegmentQueue::addNode(SQueue::iterator it, double value, bool flag) {
    _Container* e = new _Container();
    e->value = value;
    e->flag = flag;
    this->sq.insert(itThis,e);
}
