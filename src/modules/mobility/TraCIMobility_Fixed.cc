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
    Enter_Method_Silent();
    TraCIMobility::preInitialize(external_id, position, road_id, speed, angle);
    hasInitialized = false;
    hasRouted = false;
    map = NULL;
    statistic_road_id = road_id;
}

void TraCIMobility_Fixed::nextPosition(const Coord& position, std::string road_id, double speed, double angle,
        TraCIScenarioManager::VehicleSignal signals) {
    Enter_Method_Silent();
    TraCIMobility::nextPosition(position, road_id, speed, angle, signals);
    // path process
    if(!hasRouted){
        if(getMapSystem()->isInitializedFinished()){
            getMapSystem()->setVehicleRouteByEdgeList(external_id, getMapSystem()->getRandomRoute(road_id));
            hasRouted = true;
        }
    }

    // statistics process
    if(road_id != statistic_road_id){
        if(!hasInitialized){
            // switch record process trigger
            hasInitialized = true;
        }else{
            // start record process
            gs->changeName("road statistics - travel time - " + statistic_road_id)
                    << simTime().dbl() - statistic_road_enterTime << gs->endl;
        }
        statistic_road_enterTime = simTime().dbl();
        statistic_road_id = road_id;
    }
}

void TraCIMobility_Fixed::initialize(int stage) {
    TraCIMobility::initialize(stage);
    gs = GlobalStatisticsAccess().get();
}

void TraCIMobility_Fixed::changePosition() {
    Enter_Method_Silent();
    TraCIMobility::changePosition();
}
