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
            getMapSystem()->setVehicleRouteByEdgeList(external_id, getMapSystem()->getRandomRoute(road_id));
            hasRouted = true;
        }
    }

    if(road_id != last_road_id){
        // statistics process
        if(!hasInitialized){
            // switch record process trigger
            hasInitialized = true;
        }else{
            if(last_road_id == "1/1to1/2"){
                // start record process
                gs->changeName("road statistics - vehicle number - travel time - " + last_road_id)
                        << statistic_road_enterVehicleNum << simTime().dbl() - statistic_road_enterTime << gs->endl;
            }
        }
        // record the data while entering new edge
        statistic_road_enterVehicleNum = getMapSystem()->getVehicleNumByEdge(road_id);
        statistic_road_enterTime = simTime().dbl();
        // change the vehicle position in map system
        getMapSystem()->changeVehiclePosition(last_road_id, road_id);
        // updata the last_road_id
        last_road_id = road_id;
    }
}

void TraCIMobility_Fixed::initialize(int stage) {
    TraCIMobility::initialize(stage);
    gs = GlobalStatisticsAccess().get();
}

void TraCIMobility_Fixed::finish() {
    TraCIMobility::finish();
    hasRouted = false;
    hasInitialized = false;
    gs = NULL;
    map = NULL;
    getMapSystem()->unregisterVehiclePosition(last_road_id);
}

void TraCIMobility_Fixed::changePosition() {
    Enter_Method_Silent
    ();
    TraCIMobility::changePosition();
}
