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
    hasInitialized = false; // if false, the car does not exist on the SUMO map.
    hasRouted = false;
    map = NULL;
    last_road_id = "";  // in the preInitialize function, the car has not been on certain road already.
    srt = StatisticsRecordTools::request();
    srt->dblMap["t50mTime"] = 0;
    srt->dblMap["t100mTime"] = 0;
    srt->dblMap["t150mTime"] = 0;
}

void TraCIMobility_Fixed::nextPosition(const Coord& position, std::string road_id, double speed, double angle,
        TraCIScenarioManager::VehicleSignal signals) {
    Enter_Method_Silent
    ();
    TraCIMobility::nextPosition(position, road_id, speed, angle, signals);
    // routing process
    if(!hasRouted){
        if(getMapSystem()->isInitializedFinished()){
            // Map system must be initialized first
            EV << "cfg.init finished!" << endl;
            // initialize the route
            processAtRouting();
            hasRouted = true;
        }
    }else{
        // process after the routing
        processAfterRouting();
    }
    // road change
    if(road_id != last_road_id){
        // statistics process
        if(!hasInitialized){
            getMapSystem()->registerVehiclePosition(road_id);
            // switch record process trigger
            hasInitialized = true;
        }else{
            // change the vehicle position in map system
            getMapSystem()->changeVehiclePosition(last_road_id, road_id, simTime().dbl() - statistic_road_enterTime);
        }
        // in nextPosition the car has been on the road, then updata the last_road_id and enterTime
        statistic_road_enterTime = simTime().dbl();
        last_road_id = road_id;
        // when road changed
        processWhenChangeRoad();
    }
    // normal process
    processWhenNextPosition();
}

void TraCIMobility_Fixed::initialize(int stage) {
    TraCIMobility::initialize(stage);
    lastDroveAt = simTime();
}

void TraCIMobility_Fixed::finish() {
    TraCIMobility::finish();
    hasRouted = false;
    hasInitialized = false;
    getMapSystem()->unregisterVehiclePosition(last_road_id, simTime().dbl() - statistic_road_enterTime);
    // statistic process
    statisticAtFinish();
}

void TraCIMobility_Fixed::changePosition() {
    Enter_Method_Silent
    ();
    TraCIMobility::changePosition();
}

void TraCIMobility_Fixed::disableLaneChange() {
    getMapSystem()->setLaneChangePermission(getExternalId(), false);
}

void TraCIMobility_Fixed::allowLaneChange() {
    getMapSystem()->setLaneChangePermission(getExternalId(), true);
}

double TraCIMobility_Fixed::getLanePosition() {
    return getManager()->commandGetLanePosition(getExternalId());
}

void TraCIMobility_Fixed::processAfterRouting() {
    // processAfterRouting
}

void TraCIMobility_Fixed::statisticAtFinish() {
    // statisticAtFinish
}

void TraCIMobility_Fixed::processAtRouting() {
    // initialize the route
}

void TraCIMobility_Fixed::processWhenChangeRoad() {
    //
}

void TraCIMobility_Fixed::processWhenNextPosition() {
    //
}
