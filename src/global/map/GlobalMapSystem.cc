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

#include "GlobalMapSystem.h"

Define_Module(GlobalMapSystem)

GlobalMapSystem::GlobalMapSystem() :
        manager(NULL), annotations(NULL), annotationGroup(NULL), laneMap(), edgeMap(), stateSwitchMsg(NULL), startMsg(
                NULL), mapstage(0), noconnect(false), initialized(false), hostnum(100), curHostnum(0) {
}

void GlobalMapSystem::initialize(int stage) {
    // TODO - Generated method body
    BaseModule::initialize(stage);
    if(stage == 1){
        bool draw = hasPar("draw") ? par("draw") : true;
        hostnum = hasPar("hostnum") ? par("hostnum") : 100;
        mapstage = 0;
        noconnect = hasPar("noconnect") ? par("noconnect") : false;
        if(draw){
            annotations = AnnotationManager_ColorfulAccess().getIfExists();
            if(annotations){
                annotationGroup = annotations->createGroup("maps");
            }
        }
        startMsg = new cMessage("startMapSystem");
        stateSwitchMsg = new cMessage("switchMapSystemState");
        scheduleAt(0.2, startMsg);
    }
}

GlobalMapSystem::~GlobalMapSystem() {
    for(map<string, Lane*>::iterator it = laneMap.begin(); it != laneMap.end(); it++){
        delete (it->second);
    }
    for(map<string, Edge*>::iterator it = edgeMap.begin(); it != edgeMap.end(); it++){
        delete (it->second);
    }
    for(map<string, MapEdge*>::iterator it = cacheBackupEdges.begin(); it != cacheBackupEdges.end(); it++){
        for(set<MapRoute*>::iterator it_inner = it->second->routes.begin(); it_inner != it->second->routes.end();
                it_inner++){
            delete (*it_inner);
        }
        delete (it->second);
    }
}

void GlobalMapSystem::handleMessage(cMessage *msg) {
    // TODO - Generated method body
    if(msg == startMsg){
        if(getManager()->isConnected()){
            int maxStage = generateMap(mapstage);
            if(mapstage != maxStage){
                scheduleAt(simTime() + 0.1, startMsg);
                mapstage++;
            }
            debugEV << "Map Generating stage " << mapstage << " finished. lane number: " << laneMap.size()
                    << ". edge number: " << edgeMap.size() << ". cache number: " << cacheBackupEdges.size() << endl;
        }else{
            scheduleAt(simTime() + 0.1, startMsg);
        }
    }else{
        delete msg;
    }
}

int GlobalMapSystem::generateMap(int stage) {
    int maxStage = -1;
    // 1st. get all lanes and the edges containing them.
    maxStage++;
    if(stage == maxStage){
        // 0th. this function can only run at stage0 once.
        if((laneMap.size() != 0 || edgeMap.size() != 0)){
            return -1;
        }
        list<string> laneList = getManager()->commandGetLaneIds();
        for(list<string>::iterator it = laneList.begin(); it != laneList.end(); it++){
            Lane* lane = new Lane();
            lane->name = (*it);
            lane->linkNumber = 0;
            lane->length = getManager()->commandGetLaneLength(lane->name);
            laneMap[lane->name] = lane;
            Edge* edge;
            string edgeName = getManager()->commandGetLaneEdgeId(lane->name);
            // get the edge of this lane
            if(edgeMap.find(edgeName) == edgeMap.end()){
                // if the edge is not exist
                edge = new Edge();
                edge->name = edgeName;
                edge->linkNumber = 0;
                edge->laneNumber = 0;
                edge->length = 0;
                edgeMap[edgeName] = edge;
            }else{
                // if the edge is exist
                edge = edgeMap[edgeName];
            }
            // set the lane's edge
            lane->edge = edge;
            // modify the edge.
            edge->lanes.insert(lane);
            edge->laneNumber++;
        }
    }
    // 2nd. associate lanes and edges
    maxStage++;
    if(stage == maxStage){
        if(!noconnect){
            for(map<string, Lane*>::iterator it_lane = laneMap.begin(); it_lane != laneMap.end(); it_lane++){
                // get the name list of this lane's links
                list<string> linkList = commandGetLanes(it_lane->second);
                // connect lanes and edges
                for(list<string>::iterator it_link = linkList.begin(); it_link != linkList.end(); it_link++){
                    // connect links to the lane
                    if(it_lane->second->links.insert(laneMap[*it_link]).second){
                        it_lane->second->linkNumber++;
                        // connect links' edge to the lane's edge
                        if(it_lane->second->edge->links.insert(laneMap[*it_link]->edge).second){
                            it_lane->second->edge->length = (it_lane->second->edge->length
                                    * it_lane->second->edge->linkNumber + it_lane->second->length)
                                    / (it_lane->second->edge->linkNumber + 1);
                            it_lane->second->edge->linkNumber++;
                        }
                    }
                }
            }
        }
    }
    // 3rd. draw the map
    maxStage++;
    if(stage == maxStage){
        if(annotations){
            for(map<string, Lane*>::iterator it_lane = laneMap.begin(); it_lane != laneMap.end(); it_lane++){
                debugEV << "Lane { name :\"" << it_lane->second->name << "\", length : " << it_lane->second->length
                        << ", edge : \"" << it_lane->second->edge->name << "\", linknumber : "
                        << it_lane->second->linkNumber << " };" << endl;
                list<Coord> coords = getManager()->commandGetLaneShape(it_lane->first);
                list<Coord>::iterator it_coord = coords.begin();
                Coord lastCoord = *it_coord;
                for(it_coord++; it_coord != coords.end(); it_coord++){
                    it_lane->second->visualRepresentations.push_back(
                            annotations->drawLine_Colorful(lastCoord, *it_coord, "black", annotationGroup));
                    lastCoord = *it_coord;
                }
            }
        }
    }
    // 4th. reduce the map
    maxStage++;
    if(stage == maxStage){
        for(map<string, Edge*>::iterator it_edge = edgeMap.begin(); it_edge != edgeMap.end(); it_edge++){
            if(it_edge->second->linkNumber != 1){
                MapEdge* mapEdge = new MapEdge();
                mapEdge->edge = it_edge->second;
                for(set<Edge*>::iterator it = it_edge->second->links.begin(); it != it_edge->second->links.end(); it++){
                    MapRoute* mapRoute = new MapRoute();
                    Edge* curEdge = *it;
                    while(curEdge->linkNumber == 1){
                        mapRoute->length += curEdge->length;
                        mapRoute->edges.push_back(curEdge->name);
                        ASSERT(curEdge->links.size() > 0);
                        curEdge = *(curEdge->links.begin());
                    }
                    mapRoute->target = curEdge->name;
                    mapRoute->length += curEdge->length;
                    mapRoute->edges.push_back(curEdge->name);
                    mapEdge->routes.insert(mapRoute);
                    debugEV << "MapRoute { target :\"" << mapRoute->target << "\", edgeNumber : "
                            << mapRoute->edges.size() << ", length :" << mapRoute->length << " };" << endl;
                }
                list<string> route;
                route.push_back(it_edge->first);
                //route.assign((*mapEdge->routes.begin())->edges.begin(), (*mapEdge->routes.begin())->edges.end());
                debugEV << "route { name :\"" << it_edge->first << "\", first edge: \"" << (*(route.begin())) << "\" };"
                        << endl;
                getManager()->commandAddRoute(it_edge->first, route);
                cacheBackupEdges[it_edge->first] = mapEdge;
                debugEV << "MapEdge { name :\"" << mapEdge->edge->name << "\", linkNumber : "
                        << mapEdge->edge->linkNumber << ", length :" << mapEdge->edge->length << " };" << endl;
            }
        }
    }
    // 5th. generate cars
    for(int i = 0; i < 3 * hostnum; i++){
        maxStage++;
        if(stage == maxStage && i % 3 == 0){
            string vid = "node" + int2str(curHostnum);
            string start = getRandomEdgeFromCache();
            double pos = 0;
            if((int) (edgeMap[start]->length) > 10){
                pos = rand() % ((int) (edgeMap[start]->length - 10));
            }
            getManager()->commandAddVehicle(vid, "vtype0", start, simTime(), pos, 0, 0);
        }else if(stage == maxStage && i % 3 == 2){
            string vid = "node" + int2str(curHostnum++);
            string start = getManager()->commandGetEdgeId(vid);
            debugEV << start << endl;
            if(start != ""){
                list<string> route = getRandomRoute(start);
                if(debug){
                    for(list<string>::iterator it = route.begin(); it != route.end(); it++){
                        debugEV << (*it) << endl;
                    }
                }
                getManager()->commandChangeRouteByEdgeList(vid, route);
            }
        }
    }
    if(stage == maxStage){
        initialized = true;
    }
    return maxStage;
}

list<string> GlobalMapSystem::commandGetLanes(Lane* lane) {
    return getManager()->commandGetLaneLinksIds(lane->name);
}

void GlobalMapSystem::Lane::setColor(string color) {
    for(list<AnnotationManager_Colorful::Line_Colorful*>::iterator it = visualRepresentations.begin();
            it != visualRepresentations.end(); it++){
        (*it)->setColor(color);
    }
}

void GlobalMapSystem::Edge::setColor(string color) {
    for(set<Lane*>::iterator it = lanes.begin(); it != lanes.end(); it++){
        (*it)->setColor(color);
    }
}

void GlobalMapSystem::Node::setColor(string color) {
// do nothing for now
}

void GlobalMapSystem::finish() {
    cancelAndDelete(startMsg);
    cancelAndDelete(stateSwitchMsg);
}

double GlobalMapSystem::getTravelTime(string edge, double time, double speed) {
// TODO - Generated method body
    double travelTime = (*(edgeMap[edge]->lanes.begin()))->length / speed;
    return travelTime;
}

list<string> GlobalMapSystem::getFastestRoute(string fromEdge, string toEdge) {
// TODO - Generated method body
    list<string> route;
    route.push_back(fromEdge);
    route.push_back(toEdge);
    return route;
}

list<string> GlobalMapSystem::getRandomRoute(string from, double length) {
    // TODO - Generated method body
    debugEV << "random route from edge: " << from << endl;
    list<string> route;
    route.push_back(from);
    double len = 0;
    if(cacheBackupEdges.find(from) == cacheBackupEdges.end()){
        debugEV << "cache failed: " << from << endl;
        return route;
    }
    MapEdge* edge = cacheBackupEdges[from];
    while(len < length){
        // dead end
        if(edge->routes.size() == 0){
            break;
        }
        int r = rand() % edge->routes.size();
        set<MapRoute*>::iterator it = edge->routes.begin();
        for(; r > 0; r--){
            it++;
        }
        edge = cacheBackupEdges[(*it)->target];
        len += (*it)->length;
        route.insert(route.end(), (*it)->edges.begin(), (*it)->edges.end());
    }
    debugEV << "len: " << len << endl;
    return route;
}

string GlobalMapSystem::double2color(double d) {
    d = d > 255 ? 255 : d;
    d = d < 0 ? 0 : d;
    int r = d;
    int g = 255 - d;
    int b = 0;
    return rgb2color(r, g, b);
}

string GlobalMapSystem::int2str(int i) {
    std::stringstream ss;
    std::string str;
    ss << i;
    str = ss.str();
    ss.clear();
    return str;
}

string GlobalMapSystem::getRandomEdgeFromCache() {
    int r = rand() % cacheBackupEdges.size();
    map<string, MapEdge*>::iterator it = cacheBackupEdges.begin();
    for(; r > 0; r--){
        it++;
    }
    debugEV << "random edge from cache: " << it->first << endl;
    return it->first;
}

string GlobalMapSystem::rgb2color(int r, int g, int b) {
    std::stringstream ss;
    ss << "#";
    ss.setf(std::ios::hex, std::ios::basefield);
    ss.width(2);
    ss.fill('0');
    ss << r;
    ss.width(2);
    ss.fill('0');
    ss << g;
    ss.width(2);
    ss.fill('0');
    ss << b;
    std::string str = ss.str();
    ss.clear();
    return str;
}
