/*
 * GlobalStaticsUnit.h
 *
 *  Created on: 1405051354
 *      Author: Fanjing
 */

#ifndef GLOBALSTATISTICSUNIT_H_
#define GLOBALSTATISTICSUNIT_H_

#include <string>
using namespace std;
/*
 *
 */
class GlobalStatisticsUnit {
public:
    GlobalStatisticsUnit(int size);
    virtual ~GlobalStatisticsUnit();
public:
    double getData(int index) const;
    void setData(double data, int index);
    int getSize() const;
    string toString();

private:
    int size;       // read only, it must be set at initialize phase.
    double *data;   // statistics data
};

#endif /* GLOBALSTATISTICSUNIT_H_ */
