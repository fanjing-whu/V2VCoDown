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
    this->clear();
}

void SegmentQueue::remove(SegmentQueue& sq) {
    SQueue::iterator itThis = this->sq.begin();
    SQueue::iterator itSq = sq.sq.begin();
    bool flagThis = false;
    bool flagSq = false;
    while (itSq != sq.sq.end() && itThis != this->sq.end()) {
        if (!flagThis) {
            //0:--|
            if (itThis == this->sq.end()) {
                std::cout
                        << "The loop should have been terminated"
                        << " in this situation."
                        << std::endl;
            }
            if ((*itThis)->value > (*itSq)->value) {
                if (flagSq) {
                    //0:-----|+
                    //1:+|-????
                    //r:-----|+
                    flagSq = (*itSq)->flag;
                    itSq++;
                } else {
                    //0:-----|+
                    //1:-|+????
                    //r:-----|+
                    flagSq = (*itSq)->flag;
                    itSq++;
                }
            } else if ((*itThis)->value == (*itSq)->value) {
                if (flagSq) {
                    //0:-|+
                    //1:+|-
                    //r:-|+
                    flagThis = (*itThis)->flag;
                    itThis++;
                    flagSq = (*itSq)->flag;
                    itSq++;
                } else {
                    //0:-|+
                    //1:-|+
                    //r:-|+
                    flagThis = (*itThis)->flag;
                    removeNode(itThis);
                    flagSq = (*itSq)->flag;
                    itSq++;
                }
            } else if ((*itThis)->value < (*itSq)->value) {
                if (flagSq) {
                    //0:-|+????
                    //1:+++++|-
                    //r:---????
                    flagThis = (*itThis)->flag;
                    removeNode(itThis);
                } else {
                    //0:-|+????
                    //1:-----|+
                    //r:-|+????
                    flagThis = (*itThis)->flag;
                    itThis++;
                }
            } else {
                std::cout
                        << "Error: This situation cannot happen."
                        << std::endl;
            }
        } else {
            //0:++|
            if (itThis == this->sq.end()) {
                //0:++;
                std::cout
                        << "Error: The flag cannot be true "
                        << "while itThis is the end of this segment queue "
                        << "and the loop should have been terminated"
                        << " in this situation."
                        << std::endl;
            } else if ((*itThis)->value > (*itSq)->value) {
                if (flagSq) {
                    //0:+++++|-
                    //1:+|-????
                    //r:-|+++|-
                    addNode(itThis, (*itSq)->value, true);
                    flagSq = (*itSq)->flag;
                    itSq++;
                } else {
                    //0:+++++|-
                    //1:-|+????
                    //r:+|-?+|-
                    addNode(itThis, (*itSq)->value, false);
                    flagSq = (*itSq)->flag;
                    itSq++;
                }
            } else if ((*itThis)->value == (*itSq)->value) {
                if (flagSq) {
                    //0:+|-
                    //1:+|-
                    //r:---
                    flagThis = (*itThis)->flag;
                    removeNode(itThis);
                    flagSq = (*itSq)->flag;
                    itSq++;
                } else {
                    //0:+|-
                    //1:-|+
                    //r:+|-
                    flagThis = (*itThis)->flag;
                    itThis++;
                    flagSq = (*itSq)->flag;
                    itSq++;
                }
            } else if ((*itThis)->value < (*itSq)->value) {
                if (flagSq) {
                    //0:+|-????
                    //1:+++++|-
                    //r:---????
                    flagThis = (*itThis)->flag;
                    removeNode(itThis);
                } else {
                    //0:+|-????
                    //1:-----|+
                    //r:+|-????
                    flagThis = (*itThis)->flag;
                    itThis++;
                }
            } else {
                std::cout
                        << "Error: This situation cannot happen."
                        << std::endl;
            }
        }
    }
}

void SegmentQueue::add(SegmentQueue &sq) {
    SQueue::iterator itThis = this->sq.begin();
    SQueue::iterator itSq = sq.sq.begin();
    bool flagThis = false;
    bool flagSq = false;
    while (itSq != sq.sq.end()) {
        if (!flagThis) {
            //0:--|
            if (itThis == this->sq.end()) {
                if (flagSq) {
                    //0:-----;
                    //1:+|-???
                    //r:+|-?-;
                    addNode(itThis, (*itSq)->value, (*itSq)->flag);
                    flagSq = (*itSq)->flag;
                    itSq++;
                } else {
                    //0:-----;
                    //1:-|+???
                    //r:-|+?-;
                    addNode(itThis, (*itSq)->value, (*itSq)->flag);
                    flagSq = (*itSq)->flag;
                    itSq++;
                }
            } else if ((*itThis)->value > (*itSq)->value) {
                if (flagSq) {
                    //0:-----|+
                    //1:+|-????
                    //r:+|-?-|+
                    addNode(itThis, (*itSq)->value, (*itSq)->flag);
                    flagSq = (*itSq)->flag;
                    itSq++;
                } else {
                    //0:-----|+
                    //1:-|+????
                    //r:-|+?-|+
                    addNode(itThis, (*itSq)->value, (*itSq)->flag);
                    flagSq = (*itSq)->flag;
                    itSq++;
                }
            } else if ((*itThis)->value == (*itSq)->value) {
                if (flagSq) {
                    //0:-|+
                    //1:+|-
                    //r:+++
                    flagThis = (*itThis)->flag;
                    removeNode(itThis);
                    flagSq = (*itSq)->flag;
                    itSq++;
                } else {
                    //0:-|+
                    //1:-|+
                    //r:-|+
                    flagThis = (*itThis)->flag;
                    itThis++;
                    flagSq = (*itSq)->flag;
                    itSq++;
                }
            } else if ((*itThis)->value < (*itSq)->value) {
                if (flagSq) {
                    //0:-|+????
                    //1:+++++|-
                    //r:+++????
                    flagThis = (*itThis)->flag;
                    removeNode(itThis);
                } else {
                    //0:-|+????
                    //1:-----|+
                    //r:-|+????
                    flagThis = (*itThis)->flag;
                    itThis++;
                }
            } else {
                std::cout
                        << "Error: This situation cannot happen."
                        << std::endl;
            }
        } else {
            //0:++|
            if (itThis == this->sq.end()) {
                //0:++;
                std::cout
                        << "Error: The flag cannot be true"
                        << " while itThis is the end of this segment queue."
                        << std::endl;
            } else if ((*itThis)->value > (*itSq)->value) {
                if (flagSq) {
                    //0:+++++|-
                    //1:+|-????
                    //r:+++++|-
                    flagSq = (*itSq)->flag;
                    itSq++;
                } else {
                    //0:+++++|-
                    //1:-|+????
                    //r:+++++|-
                    flagSq = (*itSq)->flag;
                    itSq++;
                }
            } else if ((*itThis)->value == (*itSq)->value) {
                if (flagSq) {
                    //0:+|-
                    //1:+|-
                    //r:+|-
                    flagThis = (*itThis)->flag;
                    itThis++;
                    flagSq = (*itSq)->flag;
                    itSq++;
                } else {
                    //0:+|-?
                    //1:-|+?
                    //r:+++?
                    flagThis = (*itThis)->flag;
                    removeNode(itThis);
                    flagSq = (*itSq)->flag;
                    itSq++;
                }
            } else if ((*itThis)->value < (*itSq)->value) {
                if (flagSq) {
                    //0:+|-????
                    //1:+++++|-
                    //r:+++????
                    flagThis = (*itThis)->flag;
                    removeNode(itThis);
                } else {
                    //0:+|-????
                    //1:-----|+
                    //r:+|-????
                    flagThis = (*itThis)->flag;
                    itThis++;
                }
            } else {
                std::cout
                        << "Error: This situation cannot happen."
                        << std::endl;
            }
        }
    }
}

void SegmentQueue::output() {
    std::cout << ":";
    for (SQueue::iterator it = sq.begin(); it != sq.end(); it++) {
        std::cout << (*it)->value;
        if ((*it)->flag) {
            std::cout << "+++";
        } else {
            std::cout << "---";
        }
    }
    std::cout << std::endl;
}

SegmentQueue* SegmentQueue::copy() {
    SegmentQueue * copy = new SegmentQueue();
    copy->add(*this);
    return copy;
}

void SegmentQueue::clear() {
    for (SQueue::iterator it = sq.begin(); it != sq.end();) {
        removeNode(it);
    }
}

bool SegmentQueue::empty() {
    return sq.begin()==sq.end();
}

double SegmentQueue::length() {
    double result = 0;
    double start = 0;

    for (SQueue::iterator it = sq.begin(); it != sq.end();it++) {
        if((*it)->flag){
            start = (*it)->value;
        }else{
            result+=(*it)->value - start;
        }
    }
    return result;
}

SegmentQueue* SegmentQueue::getFirstSegment() {
    double start = 0;
    double end = 0;
    for(SQueue::iterator it = sq.begin(); it!=sq.end(); it++){
        if ((*it)->flag) {
            start = (*it)->value;
        } else {
            end = (*it)->value;
            break;
        }
    }
    if(end>start){
        return new SegmentQueue(start,end);
    }else{
        return new SegmentQueue();
    }
}

double SegmentQueue::getStartValue() {
    if(sq.begin() == sq.end()){
        return -1;
    }
    return (*sq.begin())->value;
}

SegmentQueue* SegmentQueue::getFirstSegment(double length) {
    SegmentQueue* first = this->getFirstSegment();
    if(first->length()<=length){
        return first;
    }else{
        SegmentQueue* subfirst = new SegmentQueue(first->getStartValue(),first->getStartValue()+length);
        delete(first);
        return subfirst;
    }
}

double SegmentQueue::getFirstStart() {
    return getStartValue();
}

double SegmentQueue::getFirstEnd() {
    SQueue::iterator it = sq.begin();
    if(it == sq.end()){
        return -1;
    }
    it++;
    if((*it)->flag){
        return -1;
    }
    return (*it)->value;
}

bool SegmentQueue::isCollide(SegmentQueue& sq) {
    SegmentQueue* origin = sq.copy();
    SegmentQueue* copy = sq.copy();
    copy->remove(*this);
    origin->remove(*copy);
    bool flag = !origin->empty();
    delete copy;
    delete origin;
    return flag;
}

void SegmentQueue::init(double start, double end) {
    SQueue::iterator it = sq.begin();
    if (it != sq.end()) {
        std::cout
                << "Error: You must call the INIT function"
                << " before insert any other nodes."
                << std::endl;
        return;
    }
    addNode(it, start, true);
    addNode(it, end, false);
}

void SegmentQueue::removeNode(SQueue::iterator &it) {
    delete (*it);
    // (*it) = NULL;
    it = sq.erase(it);
}

void SegmentQueue::addNode(SQueue::iterator &it, double value, bool flag) {
    _Container* e = new _Container();
    e->value = value;
    e->flag = flag;
    sq.insert(it, e);
}
