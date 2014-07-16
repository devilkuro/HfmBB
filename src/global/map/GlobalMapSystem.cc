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
;

void GlobalMapSystem::initialize(int stage) {
    // TODO - Generated method body
    BaseModule::initialize(stage);
    if (stage == 1) {
        bool draw = hasPar("draw") ? par("draw") : true;
        if (draw) {
            annotations = AnnotationManagerAccess().getIfExists();
        }
        else {
            annotations = NULL;
        }
        if (annotations)
            annotationGroup = annotations->createGroup("maps");
        stepMsg = new cMessage("step message");
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
            scheduleAt(simTime() + 0.1, stepMsg);
        }
        else {
            scheduleAt(simTime() + 0.1, startMsg);
        }
    }
    if (msg == stepMsg) {
        static map<string, Lane*>::iterator step_it_lane_list = laneMap.begin();
        // 3rd. draw the map
        {
            list<Coord> coords = getManager()->commandGetLaneShape(step_it_lane_list->first);
            if (annotations) {
                list<Coord>::iterator it = coords.begin();
                Coord lastCoord = *it;
                for (it++; it != coords.end(); it++) {
                    annotations->drawLine(lastCoord, *it, "black", annotationGroup);
                    lastCoord = *it;
                }
            }
        }
        if (step_it_lane_list != laneMap.end()) {
            step_it_lane_list++;
            scheduleAt(simTime() + 0.1, stepMsg);
        }
    }
    else {
        delete msg;
    }
}

void GlobalMapSystem::generateMap() {
    // TODO - Generated method body
    // 1st. get all lanes and the edges containing them.
    {
        list<string> laneList = getManager()->commandGetLaneIds();
        for (list<string>::iterator it = laneList.begin(); it != laneList.end(); it++) {
            Lane* lane = new Lane();
            lane->name = (*it);
            lane->linkNumber = 0;
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
    /*
     {
     for (map<string, Lane*>::iterator it_lane = laneMap.begin();
     it_lane != laneMap.end(); it_lane++) {
     // get the name list of this lane's links
     list<string> linkList = getLanes(it_lane->second);
     // connect lanes and edges
     for (list<string>::iterator it_link = linkList.begin();
     it_link != linkList.end(); it_link++) {
     // connect links to the lane
     if (it_lane->second->links.insert(laneMap[*it_link]).second) {
     it_lane->second->linkNumber++;
     // connect links' edge to the lane's edge
     if (it_lane->second->edge->edges.insert(
     laneMap[*it_link]->edge).second) {
     it_lane->second->edge->edgeNumber++;
     }
     }
     }
     }
     }
     */
}

GlobalMapSystem::GlobalMapSystem() :
        manager(0), annotations(0), annotationGroup(0), laneMap(), edgeMap(), stepMsg(NULL), startMsg(NULL) {
}

list<string> GlobalMapSystem::getLanes(Lane* lane) {
    return getManager()->commandGetLaneLinksIds(lane->name);
}
