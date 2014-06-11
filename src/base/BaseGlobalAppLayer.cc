/*
 * BaseGlobalAppLayer.cpp
 *
 *  Created on: 2014-6-11
 *      Author: Fanjing-LAB
 */

#include "BaseGlobalAppLayer.h"

BaseGlobalAppLayer::BaseGlobalAppLayer() :
        gcu(NULL) {
}

BaseGlobalAppLayer::~BaseGlobalAppLayer() {
}

void BaseGlobalAppLayer::initialize(int stage) {
    BaseApplLayer::initialize(stage);
    if (stage == 0) {
        gcu = getGCU();
    }
}

GlobalControlUnit* BaseGlobalAppLayer::getGCU() {
    return FindModule<GlobalControlUnit*>::findSubModule(this->getParentModule());
}
