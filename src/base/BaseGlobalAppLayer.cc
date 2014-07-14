/*
 * BaseGlobalAppLayer.cpp
 *
 *  Created on: 2014-6-11
 *      Author: Fanjing-LAB
 */

#include "BaseGlobalAppLayer.h"

BaseGlobalAppLayer::BaseGlobalAppLayer() :
        gs(NULL) {
}

BaseGlobalAppLayer::~BaseGlobalAppLayer() {
}

void BaseGlobalAppLayer::initialize(int stage) {
    BaseApplLayer::initialize(stage);
    if (stage == 0) {
        gs = getGS();
    }
}

GlobalStatistics* BaseGlobalAppLayer::getGS() {
    gs = FindModule<GlobalStatistics*>::findGlobalModule();
    return gs;
}
