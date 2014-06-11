/*
 * BaseGlobalAppLayer.h
 *
 *  Created on: 2014-6-11
 *      Author: Fanjing-LAB
 */

#ifndef BASEGLOBALAPPLAYER_H_
#define BASEGLOBALAPPLAYER_H_

#include "BaseApplLayer.h"
#include "FindModule.h"
#include "GlobalControlUnit.h"

class BaseGlobalAppLayer: public BaseApplLayer {
public:
    BaseGlobalAppLayer();
    virtual ~BaseGlobalAppLayer();
public:
    virtual void initialize(int stage);
protected:
    GlobalControlUnit* gcu;

private:
    GlobalControlUnit* getGCU();
};

#endif /* BASEGLOBALAPPLAYER_H_ */
