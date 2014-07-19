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

void GlobalMapSystem::initialize(int stage) {
    // TODO - Generated method body
    BaseModule::initialize(stage);
    if (stage == 1) {
        bool draw = hasPar("draw") ? par("draw") : true;
        if (draw) {
            annotations = AnnotationManager_ColorfulAccess().getIfExists();
        }
        else {
            annotations = NULL;
        }
        if (annotations)
            annotationGroup = annotations->createGroup("maps");
        startMsg = new cMessage("startMapSystem");
        scheduleAt(0.2, startMsg);
    }
}

GlobalMapSystem::~GlobalMapSystem() {
    // TODO - Generated method body
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
            generateMap();
            debugEV << "Map Generating finished. lane number: " << laneMap.size() << ". edge number: " << edgeMap.size()
                    << endl;
        }
        else {
            scheduleAt(simTime() + 0.1, startMsg);
        }
    }
    else {
        delete msg;
    }
}

void GlobalMapSystem::generateMap() {
    // 0th. this function can only run once.
    if (laneMap.size() != 0 || edgeMap.size() != 0) {
        return;
    }
    // 1st. get all lanes and the edges containing them.
    {
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
            }
            else {
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
    if (!par("noconnect").boolValue()) {
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
    // 3rd. draw the map
    {
        for (map<string, Lane*>::iterator it_lane = laneMap.begin(); it_lane != laneMap.end(); it_lane++) {
            debugEV << "Lane { name :\"" << it_lane->second->name << "\", length : " << it_lane->second->length
                    << ", edge : \"" << it_lane->second->edge->name << "\", linknumber : "
                    << it_lane->second->linkNumber << " };" << endl;
            list<Coord> coords = getManager()->commandGetLaneShape(it_lane->first);
            if (annotations) {
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
}

GlobalMapSystem::GlobalMapSystem() :
        manager(0), annotations(0), annotationGroup(0), laneMap(), edgeMap(), startMsg(NULL) {
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
}

string GlobalMapSystem::double2color(double d) {
    std::stringstream ss;
    d = d > 255 ? 255 : d;
    d = d < 0 ? 0 : d;
    int r = d;
    int g = 255 - d;
    int b = 0;
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
