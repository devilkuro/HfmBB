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
            // when the car first appear on the map.
            processWhenInitializingRoad();
            getMapSystem()->registerVehiclePosition(road_id);
            // switch record process trigger
            hasInitialized = true;
        }else{
            // when road changed
            processWhenChangeRoad();
            // change the vehicle position in map system
            getMapSystem()->changeVehiclePosition(last_road_id, road_id, simTime().dbl() - statistic_road_enterTime);
        }
        // in nextPosition the car has been on the road, then updata the last_road_id and enterTime
        statistic_road_enterTime = simTime().dbl();
        last_road_id = road_id;
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
    // this function will run every 0.1 second for each car after routint process!!
    // so, do not do any complicated operations here.
}

void TraCIMobility_Fixed::statisticAtFinish() {
    // statisticAtFinish
}

void TraCIMobility_Fixed::processAtRouting() {
    // initialize the route
}

void TraCIMobility_Fixed::processWhenChangeRoad() {
    // when road changed after first appear on the map.
}

void TraCIMobility_Fixed::processWhenInitializingRoad() {
    // when the car first appear on the map.
}

void TraCIMobility_Fixed::processWhenNextPosition() {
    // this function will run every 0.1 second for each car!!
    // so, do not do any complicated operations here.
}

TraCIMobility_Fixed::TraCIMobility_Fixed() {
    map = NULL;
    vehicleManager = NULL;
    srt = NULL;
    lastDroveAt = 0;
    hasRouted = false;
    hasInitialized = false;
    statistic_road_enterTime = 0;
    statistic_start_time = 0;
    statistic_road_enterVehicleNum = 0;
    statistic_junction_enterVehicleNum = 0;
    last_road_id = "";

}
