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

#ifndef __SMARTCITYFLOWRESERVED_GLOBALMAPSYSTEM_H_
#define __SMARTCITYFLOWRESERVED_GLOBALMAPSYSTEM_H_

#include <string>
#include <fstream>
#include <list>
#include <map>
#include <stdexcept>

#include "FindModule.h"
#include "BaseModule.h"
#include "global/launchd/GlobalMobilityLaunchd.h"

/**
 * TODO - Generated class
 */
using std::string;
using std::map;
using std::list;
using std::set;

class GlobalMapSystem: public BaseModule {
public:
    GlobalMapSystem() :
            manager(NULL), laneMap(), edgeMap(), startMsg(NULL) {
    }
    virtual ~GlobalMapSystem();
protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const {
        return std::max(cSimpleModule::numInitStages(), 3);
    }

public:
    class Lane {
    public:
        string name;
        string edgeID;
        int linkNumber;
        set<Lane*> links;
    };
    class Edge {
    public:
        string name;
        int linkNumber;
        set<Lane*> links;
        int edgeNumber;
        set<Edge*> edges;
    };
public:
    virtual void generateMap();
    virtual GlobalMobilityLaunchd* getManager() const {
        if (!manager)
            manager = GlobalMobilityLaunchdAccess().get();
        return manager;
    }
protected:
    list<string> getLanes(Lane* lane);

protected:
    mutable TraCIScenarioManager* manager;
    map<string, Lane*> laneMap;
    map<string, Edge*> edgeMap;

    cMessage* startMsg;
};

#endif
