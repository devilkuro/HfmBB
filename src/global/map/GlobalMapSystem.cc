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
                NULL), mapstage(0), noconnect(false), initialized(false) {
}

void GlobalMapSystem::initialize(int stage) {
    // TODO - Generated method body
    BaseModule::initialize(stage);
    if (stage == 1) {
        bool draw = hasPar("draw") ? par("draw") : true;
        mapstage = 0;
        noconnect = hasPar("noconnect") ? par("noconnect") : false;
        if (draw) {
            annotations = AnnotationManager_ColorfulAccess().getIfExists();
            if (annotations) {
                annotationGroup = annotations->createGroup("maps");
            }
        }
        startMsg = new cMessage("startMapSystem");
        stateSwitchMsg = new cMessage("switchMapSystemState");
        scheduleAt(0.2, startMsg);
    }
}

GlobalMapSystem::~GlobalMapSystem() {
    for (map<string, Lane*>::iterator it = laneMap.begin(); it != laneMap.end(); it++) {
        delete (it->second);
    }
    for (map<string, Edge*>::iterator it = edgeMap.begin(); it != edgeMap.end(); it++) {
        delete (it->second);
    }
}

void GlobalMapSystem::handleMessage(cMessage *msg) {
    // TODO - Generated method body
    if (msg == startMsg) {
        if (getManager()->isConnected()) {
            int maxStage = generateMap(mapstage);
            if (mapstage != maxStage) {
                scheduleAt(simTime() + 0.1, startMsg);
                mapstage++;
            }
            debugEV << "Map Generating stage " << mapstage << " finished. lane number: " << laneMap.size()
                    << ". edge number: " << edgeMap.size() << endl;
        } else {
            scheduleAt(simTime() + 0.1, startMsg);
        }
    } else {
        delete msg;
    }
}

int GlobalMapSystem::generateMap(int stage) {
    int maxStage = -1;
    // 1st. get all lanes and the edges containing them.
    maxStage++;
    if (stage == maxStage) {
        // 0th. this function can only run at stage0 once.
        if ((laneMap.size() != 0 || edgeMap.size() != 0)) {
            return -1;
        }
        list<string> laneList = getManager()->commandGetLaneIds();
        for (list<string>::iterator it = laneList.begin(); it != laneList.end(); it++) {
            Lane* lane = new Lane();
            lane->name = (*it);
            lane->linkNumber = 0;
            lane->length = getManager()->commandGetLaneLength(lane->name);
            laneMap[lane->name] = lane;
            Edge* edge;
            string edgeName = getManager()->commandGetLaneEdgeId(lane->name);
            // get the edge of this lane
            if (edgeMap.find(edgeName) == edgeMap.end()) {
                // if the edge is not exist
                edge = new Edge();
                edge->name = edgeName;
                edge->edgeNumber = 0;
                edge->linkNumber = 0;
                edgeMap[edgeName] = edge;
            } else {
                // if the edge is exist
                edge = edgeMap[edgeName];
            }
            // set the lane's edge
            lane->edge = edge;
            // modify the edge.
            edge->links.insert(lane);
            edge->linkNumber++;
        }
    }
    // 2nd. connect lanes and edges
    maxStage++;
    if (stage == maxStage) {
        if (!noconnect) {
            for (map<string, Lane*>::iterator it_lane = laneMap.begin(); it_lane != laneMap.end(); it_lane++) {
                // get the name list of this lane's links
                list<string> linkList = getLanes(it_lane->second);
                // connect lanes and edges
                for (list<string>::iterator it_link = linkList.begin(); it_link != linkList.end(); it_link++) {
                    // connect links to the lane
                    if (it_lane->second->links.insert(laneMap[*it_link]).second) {
                        it_lane->second->linkNumber++;
                        // connect links' edge to the lane's edge
                        if (it_lane->second->edge->edges.insert(laneMap[*it_link]->edge).second) {
                            it_lane->second->edge->edgeNumber++;
                        }
                    }
                }
            }
        }
    }
    // 3rd. draw the map
    maxStage++;
    if (stage == maxStage) {
        if (annotations) {
            for (map<string, Lane*>::iterator it_lane = laneMap.begin(); it_lane != laneMap.end(); it_lane++) {
                debugEV << "Lane { name :\"" << it_lane->second->name << "\", length : " << it_lane->second->length
                        << ", edge : \"" << it_lane->second->edge->name << "\", linknumber : "
                        << it_lane->second->linkNumber << " };" << endl;
                list<Coord> coords = getManager()->commandGetLaneShape(it_lane->first);
                list<Coord>::iterator it_coord = coords.begin();
                Coord lastCoord = *it_coord;
                for (it_coord++; it_coord != coords.end(); it_coord++) {
                    it_lane->second->visualRepresentations.push_back(
                            annotations->drawLine_Colorful(lastCoord, *it_coord, "black", annotationGroup));
                    lastCoord = *it_coord;
                }
            }
        }
    }
    // 4th. reduce the map
    maxStage++;
    if (stage == maxStage) {

    }
    if (stage == maxStage) {
        initialized = true;
    }
    return maxStage;
}

list<string> GlobalMapSystem::getLanes(Lane* lane) {
    return getManager()->commandGetLaneLinksIds(lane->name);
}

void GlobalMapSystem::Lane::setColor(string color) {
    for (list<AnnotationManager_Colorful::Line_Colorful*>::iterator it = visualRepresentations.begin();
            it != visualRepresentations.end(); it++) {
        (*it)->setColor(color);
    }
}

void GlobalMapSystem::Edge::setColor(string color) {
    for (set<Lane*>::iterator it = links.begin(); it != links.end(); it++) {
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
    double travelTime = (*(edgeMap[edge]->links.begin()))->length / speed;
    return travelTime;
}

list<string> GlobalMapSystem::getFastestRoute(string fromEdge, string toEdge) {
    // TODO - Generated method body
    list<string> route;
    route.push_back(fromEdge);
    route.push_back(toEdge);
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
