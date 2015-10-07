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
 *
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
public:

public:
    class Lane;
    class Edge;
    class MapEdge;
    class MapRoute;
    class MapNode;
    class MapEdgeWight;
    class Lane {
    public:
        string name;
        Edge* edge;
        int linkNumber;
        set<Lane*> links;
        double length;
        list<AnnotationManager_Colorful::Line_Colorful*> visualRepresentations;
        void setColor(string color);
    };
    class Edge {
    public:
        string name;
        int laneNumber;
        set<Lane*> lanes;
        int linkNumber;
        set<Edge*> links;
        double length;

        void setColor(string color);
    };
    class Node {
    public:
        string name;
        string type;
        Coord pos;
        double r;
        list<AnnotationManager_Colorful::Line_Colorful*> visualRepresentations;
        void setColor(string color);
    };
    class MapEdge {
    public:
        Edge* edge;
        Coord startPos;
        Coord endPos;
        list<MapRoute*> cacheRoutes;
        vector<MapRoute*> routes;
    };
    class MapRoute {
    public:
        string target;
        double length;
        list<string> edges;
        int getVehicleNum();
    };
    class MapNode {
    public:
        Coord pos;
        map<string,MapEdge*> inEdges;
        map<string,MapEdge*> outEdges;
    };
    class MapEdgeWight {
    public:
        MapEdge* edge;
        MapEdge* preEdge;
        double outTime;
        double getOutTime(double enterTime);
        bool operator<(MapEdgeWight& rhs) {
            return this->outTime < rhs.outTime;
        }
    };
public:
    // API_PART
    // functions to add vehicles
    bool addOneVehicle(string vehicleId = "", string vehicleTypeId = "", string routeId = "", simtime_t emitTime_st =
            simTime(), double emitPosition = 0, double emitSpeed = 0, int8_t emitLane = 0); // add a car of a certain type
    bool addVehicles(int num, string vehicleId = "", string vehicleTypeId = "", string routeId = "",
            simtime_t emitTime_st = simTime(), double emitPosition = 0, double emitSpeed = 0, int8_t emitLane = 0); // add several cars of a certain type
    // lane change mode control
    void setLaneChangeMode(string nodeId, uint32_t bitset);
    void setLaneChangePermission(string nodeId, bool allowed);
    // record vehicle number
    void registerVehiclePosition(string road_id);
    void changeVehiclePosition(string road_from, string road_to, double pass_time = -1);
    void unregisterVehiclePosition(string road_id, double pass_time = -1);
    // get vehicle number
    int getVehicleNumByEdge(string edge);
    double getAvgTravelTimeByEdge(string edge);

    // OTHER_API PART
    virtual bool isInitializedFinished();
    virtual uint32_t getActiveVehicleCount();
    virtual double getTravelTime(string edge, double time, double speed);
    virtual list<string> getFastestRoute(string fromEdge, string toEdge);   // unfinished
    virtual list<string> getShortestRoute(string fromEdge, string toEdge);  // unfinished
    virtual list<string> getRandomRoute(string from, double length = 72000);
    virtual list<string> getAllEdges();
    virtual list<string> getNextEdges(string edge);
    virtual double getEdgeLength(string edge);
    virtual list<Coord> getLaneShape(string lane);
    virtual list<Coord> getEdgeShape(string edge);

    string getRandomEdgeFromCache();

    virtual void setVehicleRouteByEdgeList(string id, list<std::string> route);
    // protected..
    virtual GlobalMobilityLaunchd* getManager() const {
        if(!manager){
            manager = GlobalMobilityLaunchdAccess().get();
        }
        ASSERT(manager);
        return manager;
    }
protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
    virtual int numInitStages() const {
        return std::max(cSimpleModule::numInitStages(), 3);
    }

    // map generating process
    virtual int generateMap(int stage);
    virtual void getLanesAndEdges();
    virtual void getNodes(); // get junctions
    virtual void connectLanesAndEdges();
    virtual void drawMap();
    virtual void reduceMap();
    virtual void optimizeMap();
    virtual void outputMap();
    virtual void weightEdges(); // set the area weight for each edge.

    // non-public APIs
    virtual list<string> getNextLanes(Lane* lane);

protected:
    mutable GlobalMobilityLaunchd* manager;
    AnnotationManager_Colorful* annotations;
    AnnotationManager_Colorful::Group* annotationGroup;
    // used in map generating process
    map<string, Lane*> laneMap;
    map<string, Edge*> edgeMap;
    map<string, Node*> nodeMap;

    cMessage* stateSwitchMsg;
    cMessage* startMsg;
    cMessage* updateMsg;

    int mapstage;
    bool noconnect;
    bool mapSystemInitialized;
    int hostnum;
    int lastHostNo;

public:

    string int2str(int i);
    string dou2str(double i);
    string double2color(double d);
    string rgb2color(int r, int g, int b);
private:
    // used in path-finding
    vector<MapEdge*> cacheEdgeArray;
    map<string, MapEdge*> cacheBackupEdges;
    mutable list<MapEdgeWight> cacheUntappedEdges;
    mutable list<MapEdgeWight> cacheTappedEdges;
    // record vehicle number
    map<string, int> roadVehicleNumMap;
    map<string, double> roadVehiclePassTimeMap;
    int vehicleNumber;
};
class GlobalMapSystemAccess {
public:
    GlobalMapSystem* get() {
        return FindModule<GlobalMapSystem*>::findGlobalModule();
    }
};
#endif
