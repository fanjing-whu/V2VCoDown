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
#include "ctime"
#include <fstream>
#include <iostream>

Define_Module(GlobalStatistics);
GlobalStatistics::gs_eofType GlobalStatistics::endl = NULL;
void GlobalStatistics::initialize()
{
    // do nothing
}

void GlobalStatistics::handleMessage(cMessage *msg)
{
    // do nothing
}

GlobalStatistics::GlobalStatistics() {
    // do nothing
}

GlobalStatistics::~GlobalStatistics() {
    for (GlobalStatisticsMap::iterator it = globalStatisticsMap.begin();
            it != globalStatisticsMap.end(); it++) {
        for (GlobalStatisticsList::iterator lit = it->second->begin();
                lit != it->second->end(); lit++) {
            delete (*lit);
        }
        delete (it->second);
    }
    globalStatisticsMap.clear();
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
        GlobalStatisticsList* list = new GlobalStatisticsList();
        globalStatisticsMap[name] = list;
    }
    globalStatisticsMap[name]->push_back(unit);
}

void GlobalStatistics::finish() {
    ostringstream name;
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    name.width(2);
    name<<"result_"
            <<timeinfo->tm_year<<"_"
            <<timeinfo->tm_mon<<"_"
            <<timeinfo->tm_mday<<"_"
            <<timeinfo->tm_hour<<"_"
            <<timeinfo->tm_min<<"_"
            <<timeinfo->tm_sec<<".log";
    output(name.str());
}

GlobalStatistics& GlobalStatistics::operator <<(gs_eofType& e) {
    if (unitData.size()>0) {
        GlobalStatisticsUnit* unit = new GlobalStatisticsUnit(unitData.size());
        int i = 0;
        for(std::list<double>::iterator it = unitData.begin();it!=unitData.end();it++){
            unit->setData(*it,i);
            i++;
        }
        GlobalStatisticsMap::iterator it;
        it = globalStatisticsMap.find(m_name);
        if (it == globalStatisticsMap.end()) {
            GlobalStatisticsList* list = new GlobalStatisticsList();
            globalStatisticsMap[m_name] = list;
        }
        globalStatisticsMap[m_name]->push_back(unit);
        unitData.clear();
    }
    return *this;
}

GlobalStatistics& GlobalStatistics::operator <<(double num) {
    unitData.push_back(num);
    return *this;
}

GlobalStatistics& GlobalStatistics::changeName(string name) {
    GlobalStatisticsMap::iterator it;
    m_name = name;
    it = globalStatisticsMap.find(m_name);
    if (it == globalStatisticsMap.end()) {
        GlobalStatisticsList* list = new GlobalStatisticsList();
        globalStatisticsMap[m_name] = list;
    }
    unitData.clear();
    return *this;
}

void GlobalStatistics::output(string name) {
    std::fstream fs;
    fs.open (name.c_str(),std::fstream::out);
    for(GlobalStatisticsMap::iterator it = globalStatisticsMap.begin();it!=globalStatisticsMap.end();it++){
        fs<<it->first<<std::endl;
        for(GlobalStatisticsList::iterator lit = it->second->begin();lit!=it->second->end();lit++){
            fs<<(*lit)->toString()<<std::endl;
        }
    }
    fs.close();
}

