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

#ifndef SMTMAPSYSTEM_H_
#define SMTMAPSYSTEM_H_

#include "GlobalMapSystem.h"
#include "SMTCarInfoQueue.h"

using namespace Fanjing;

class SMTLane {
    string id;
    string edge;
    string tl;
};
class SMTEdge {
    string id;
    vector<string> laneVec;
    vector<string> queueVec;
    vector<string> nextVec;
};
class SMTTrafficLight {
    string id;
    vector<double> allowedInterval;
    vector<double> cyclePeriod;
    vector<double> cycleOffset;
};
class SMTMapSystem : public GlobalMapSystem {
public:
    SMTMapSystem();
    virtual ~SMTMapSystem();
protected:
    list<string> normalEdgeList;
    map<string, SMTEdge> edgeMap;
    map<string, SMTCarInfoQueue> queueMap;

    virtual void initialize(int stage);
    void ganerateMapTopology();

};
class SMTMapSystemAccess {
public:
    SMTMapSystem* get() {
        return FindModule<SMTMapSystem*>::findGlobalModule();
    }
};
#endif /* SMTMAPSYSTEM_H_ */
