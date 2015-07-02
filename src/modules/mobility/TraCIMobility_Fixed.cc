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

#include "TraCIMobility_Fixed.h"
Define_Module(TraCIMobility_Fixed)

void TraCIMobility_Fixed::preInitialize(std::string external_id, const Coord& position, std::string road_id,
        double speed, double angle) {
    Enter_Method_Silent
    ();
    TraCIMobility::preInitialize(external_id, position, road_id, speed, angle);
    hasInitialized = false;
    hasRouted = false;
    map = NULL;
    last_road_id = road_id;
    // record the road_id
    getMapSystem()->registerVehiclePosition(road_id);
}

void TraCIMobility_Fixed::nextPosition(const Coord& position, std::string road_id, double speed, double angle,
        TraCIScenarioManager::VehicleSignal signals) {
    Enter_Method_Silent
    ();
    TraCIMobility::nextPosition(position, road_id, speed, angle, signals);
    // path process
    if(!hasRouted){
        if(getMapSystem()->isInitializedFinished()){
            EV << "cfg.init finished!" << endl;
            list<string> route;
            switch (external_id[0]) {
                case 'T':
                case 'L':
                    // turn left: "2/0to2/2","2/2to0/2","0/2to0/4";
                    route.push_back("2/0to2/2");
                    route.push_back("2/2to0/2");
                    route.push_back("0/2to0/4");
                    getMapSystem()->setVehicleRouteByEdgeList(external_id,route);
                    break;
                case 'S':
                    // stright: "2/0to2/2","2/2to2/4","2/4to4/4";
                    route.push_back("2/0to2/2");
                    route.push_back("2/2to2/4");
                    route.push_back("2/4to4/4");
                    getMapSystem()->setVehicleRouteByEdgeList(external_id,route);
                    break;
                case 'R':
                    // turn right: "2/0to2/2","2/2to4/2","4/2to4/4";
                    route.push_back("2/0to2/2");
                    route.push_back("2/2to4/2");
                    route.push_back("4/2to4/4");
                    getMapSystem()->setVehicleRouteByEdgeList(external_id,route);
                    break;
                case 'P':
                    // turn right: "2/0to2/2","2/2to4/2","4/2to4/4";
                    route.push_back("2/4to2/2");
                    route.push_back("2/2to0/2");
                    getMapSystem()->setVehicleRouteByEdgeList(external_id,route);
                    break;
                default:
                    // default
                    break;
            }
            hasRouted = true;
        }
    }else{
        if(external_id[0]=='P'){
            commandSetSpeed(0);
        }
    }
    if(road_id != last_road_id){
        // statistics process
        if(!hasInitialized){
            // switch record process trigger
            hasInitialized = true;
        }
        // change the vehicle position in map system
        getMapSystem()->changeVehiclePosition(last_road_id, road_id, simTime().dbl()-statistic_road_enterTime);
        // updata the last_road_id and enterTime
        statistic_road_enterTime = simTime().dbl();
        last_road_id = road_id;
    }
}

void TraCIMobility_Fixed::initialize(int stage) {
    TraCIMobility::initialize(stage);
    srtool = StatisticsRecordTools::request();
}

void TraCIMobility_Fixed::finish() {
    TraCIMobility::finish();
    hasRouted = false;
    hasInitialized = false;
    getMapSystem()->unregisterVehiclePosition(last_road_id, simTime().dbl()-statistic_road_enterTime);
    if(external_id == "nodeTarget"){

    }
}

void TraCIMobility_Fixed::changePosition() {
    Enter_Method_Silent
    ();
    TraCIMobility::changePosition();
}

void TraCIMobility_Fixed::disableLaneChange() {
    getMapSystem()->setLaneChangePermission(getExternalId(),false);
}

void TraCIMobility_Fixed::allowLaneChange() {
    getMapSystem()->setLaneChangePermission(getExternalId(),true);
}

double TraCIMobility_Fixed::getLanePosition() {
    return getManager()->commandGetLanePosition(getExternalId());
}
