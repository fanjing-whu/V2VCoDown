/*
 * GlobalStaticsUnit.cc
 *
 *  Created on: 1405051354
 *      Author: Fanjing
 */

#include "GlobalStatisticsUnit.h"
#include <sstream>

GlobalStatisticsUnit::GlobalStatisticsUnit(int size) {
    this->size = size;
    data = new double[size];
}

GlobalStatisticsUnit::~GlobalStatisticsUnit() {

    size = 0;
    delete(data);
    data = NULL;
}

double GlobalStatisticsUnit::getData(int index) const {
    if(index<0||index>=size)
        return -1;
    return data[index];
}

void GlobalStatisticsUnit::setData(double data, int index) {
    if (index >= 0 && index < size) {
        this->data[index] = data;

    }
}


int GlobalStatisticsUnit::getSize() const {
    return size;
}

string GlobalStatisticsUnit::toString() {
    stringstream ss;
    for(int i = 0;i<size;i++){
        if(i!=0)
            ss<<",";
        ss<<data[i];
    }
    return ss.str();
}
