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

#ifndef SEGMENTQUEUE_H_
#define SEGMENTQUEUE_H_
#include <list>

class SegmentQueue {
public:
    SegmentQueue();
    virtual ~SegmentQueue();
private:
    typedef struct {
        double value;
        bool flag;
    } _Container;
    typedef std::list<_Container*> SQueue;
    SQueue sq;
public:
    void remove(SegmentQueue sq);
    void add(SegmentQueue sq);
private:
    void removeNode(SQueue::iterator it);
    void addNode(SQueue::iterator it, double value, bool flag);

    void init(double start, double end);

    inline bool operator_remove(bool lhs, bool rhs) {
        return lhs && !rhs;
    }
    inline bool operator_add(bool lhs, bool rhs) {
        return lhs || rhs;
    }
};



#endif /* SEGMENTQUEUE_H_ */
