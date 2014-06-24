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

#ifndef __SIMPLEVEINS_GLOBALSTATISTICS_H_
#define __SIMPLEVEINS_GLOBALSTATISTICS_H_

#include <omnetpp.h>
#include<map>
#include<list>
#include<string>
#include<stdarg.h>
#include"GlobalStatisticsUnit.h"


typedef std::list<GlobalStatisticsUnit*> GlobalStatisticsList;
typedef std::map<string, GlobalStatisticsList*> GlobalStatisticsMap;
/**
 *
 */

class GlobalStatistics: public cSimpleModule {
public:
    GlobalStatistics();
    virtual ~GlobalStatistics();
protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
public:
    typedef void* gs_eofType;
public:
    GlobalStatistics& operator<< (gs_eofType& e);
    GlobalStatistics& operator<< (double num);
    GlobalStatistics& changeName(string name);
    void record(string name, int size, ...);
    void output(string name);
    void eof();
public:
    static gs_eofType endl;
private:
    GlobalStatisticsMap globalStatisticsMap;
    std::list<double> unitData;
    string m_name;
};

#endif
