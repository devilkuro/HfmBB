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

#include "SMTMobility.h"

Define_Module(SMTMobility)
SMTMobility::SMTMobility() {

}

SMTMobility::~SMTMobility() {

}

void SMTMobility::processAfterRouting() {
}

void SMTMobility::statisticAtFinish() {
}

void SMTMobility::processAtRouting() {
    if(external_id == "car00"){
        commandSetSpeed(0);
    }else{
        carInfo = getVehicleManager()->getCarInfo(external_id);
        string start = "2/2";
        string end = "2/4";
        string to = "to";
        string head = GlobalVehicleManager::getEndPoint(carInfo.origin);
        string tail = GlobalVehicleManager::getStartPoint(carInfo.destination);
        list<string> route;
        route.push_back(carInfo.origin);
        if(head == start){
            // end to end
            if(tail == end){
                route.push_back(start + to + end);
            }
        }
        if(head == end){
            // end to start
            if(tail == start){
                route.push_back(end + to + start);
            }
        }
        route.push_back(carInfo.destination);
        changeRoute(route);
    }
}

void SMTMobility::processWhenChangeRoad() {
}

void SMTMobility::processWhenInitializingRoad() {
}

void SMTMobility::processWhenNextPosition() {
}

void SMTMobility::changeRoute(list<string> route) {
    getMapSystem()->setVehicleRouteByEdgeList(external_id, route);
}
