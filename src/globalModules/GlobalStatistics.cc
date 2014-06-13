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

#include "GlobalStatistics.h"
#include <fstream>
#include <iostream>

Define_Module(GlobalStatistics);

void GlobalStatistics::initialize()
{
    // TODO - Generated method body
}

void GlobalStatistics::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}

GlobalStatistics::GlobalStatistics() {
    // TODO Auto-generated constructor stub
}

GlobalStatistics::~GlobalStatistics() {
    for (GlobalStatisticsMap::iterator it = globalStatisticsMap.begin();
            it != globalStatisticsMap.end(); it++) {
        for (GlobalStatisticsList::iterator lit = it->second.begin();
                lit != it->second.end(); lit++) {
            delete (&lit);
        }
    }
}

void GlobalStatistics::record(string name, int size, ...) {
    GlobalStatisticsMap::iterator it;
    GlobalStatisticsUnit *unit = new GlobalStatisticsUnit(size);
    double val;
    va_list vl;
    va_start(vl,size);
    for(int i = 0; i<size; i++){
        val = va_arg(vl,double);
        unit->setData(val,i);
    }
    va_end(vl);
    it = globalStatisticsMap.find(name);
    if(it==globalStatisticsMap.end()){
        GlobalStatisticsList list;
        globalStatisticsMap.insert(std::pair<string,GlobalStatisticsList>(name,list));
    }
    globalStatisticsMap[name].push_back(*unit);
}

void GlobalStatistics::output(string name) {
    std::fstream fs;
    fs.open (name.c_str(),std::fstream::out);
    for(GlobalStatisticsMap::iterator it = globalStatisticsMap.begin();it!=globalStatisticsMap.end();it++){
        fs<<it->first<<std::endl;
        for(GlobalStatisticsList::iterator lit = it->second.begin();lit!=it->second.end();lit++){
            fs<<lit->toString()<<std::endl;
        }
    }
    fs.close();
}

