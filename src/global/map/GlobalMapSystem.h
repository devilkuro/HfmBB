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
#include <sstream>
#include <list>
#include <map>
#include <stdexcept>

#include "FindModule.h"
#include "BaseModule.h"
#include "global/launchd/GlobalMobilityLaunchd.h"
#include "AnnotationManager_Colorful.h"

/**
 * TODO - Generated class
 */
using std::string;
using std::map;
using std::list;
using std::set;
using std::vector;

class GlobalMapSystem : public BaseModule {
public:
    GlobalMapSystem();
    virtual ~GlobalMapSystem();
protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const {
        return std::max(cSimpleModule::numInitStages(), 3);
    }

public:
    virtual void generateMap();
    virtual GlobalMobilityLaunchd* getManager() const {
        if (!manager) {
            manager = GlobalMobilityLaunchdAccess().get();
        }
        ASSERT(manager);
        return manager;
    }
public:
    class Lane;
    class Edge;
    class Lane {
    public:
        string name;
        Edge* edge;
        int linkNumber;
        set<Lane*> links;
        list<AnnotationManager_Colorful::Line_Colorful*> visualRepresentations;
        void setColor(AnnotationManager_Colorful*annotations, string color);
    };
    class Edge {
    public:
        string name;
        int linkNumber;
        set<Lane*> links;
        int edgeNumber;
        set<Edge*> edges;
        void setColor(AnnotationManager_Colorful* annotations, string color);
    };
    class Node {
        string name;
        string type;
        Coord pos;
        double r;
        void setColor(string color);
    };
protected:
    list<string> getLanes(Lane* lane);

protected:
    mutable GlobalMobilityLaunchd* manager;
    AnnotationManager_Colorful* annotations;
    AnnotationManager_Colorful::Group* annotationGroup;
    map<string, Lane*> laneMap;
    map<string, Edge*> edgeMap;

    cMessage* startMsg;
private:
    string double2color(double d);
};

#endif
