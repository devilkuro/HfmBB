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
    srt = StatisticsRecordTools::request();
    srt->dblMap["t50mTime"] = 0;
    srt->dblMap["t100mTime"] = 0;
    srt->dblMap["t150mTime"] = 0;
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
            switch(external_id[0]){
                case 'T':
                    // target: "2/0to2/2","2/2to0/2","0/2to0/4";
                    route.push_back("2/0to2/2");
                    route.push_back("2/2to0/2");
                    route.push_back("0/2to0/4");
                    getMapSystem()->setVehicleRouteByEdgeList(external_id, route);
                    break;
                case 'L':
                    // turn left: "2/0to2/2","2/2to0/2";
                    route.push_back("2/0to2/2");
                    route.push_back("2/2to0/2");
                    getMapSystem()->setVehicleRouteByEdgeList(external_id, route);
                    break;
                case 'S':
                    // stright: "2/0to2/2","2/2to2/4";
                    route.push_back("2/0to2/2");
                    route.push_back("2/2to2/4");
                    getMapSystem()->setVehicleRouteByEdgeList(external_id, route);
                    break;
                case 'R':
                    // turn right: "2/0to2/2","2/2to4/2";
                    route.push_back("2/0to2/2");
                    route.push_back("2/2to4/2");
                    getMapSystem()->setVehicleRouteByEdgeList(external_id, route);
                    break;
                case 'P':
                    // parking car;
                    route.push_back("2/4to2/2");
                    getMapSystem()->setVehicleRouteByEdgeList(external_id, route);
                    break;
                default:
                    // default
                    break;
            }
            hasRouted = true;
        }
    }else{
        if(external_id[0] == 'P'){
            commandSetSpeed(0);
        }
    }
    if(road_id != last_road_id){
        // statistics process
        if(!hasInitialized){
            // switch record process trigger
            hasInitialized = true;
        }
        if(external_id[0] == 'T'){
            if(last_road_id == "2/0to2/2"){
                srt->dblMap["droveOutTime"] = simTime().dbl();
                srt->dblMap["passFirstTime"] = simTime().dbl() - srt->dblMap["startTime"];
            }
            if(road_id == "2/2to0/2"){
                srt->dblMap["reachNextEdgeTime"] = simTime().dbl();
                srt->dblMap["passJunctionTime"] = simTime().dbl() - srt->dblMap["passFirstTime"];
            }
        }
        // change the vehicle position in map system
        getMapSystem()->changeVehiclePosition(last_road_id, road_id, simTime().dbl() - statistic_road_enterTime);
        // updata the last_road_id and enterTime
        statistic_road_enterTime = simTime().dbl();
        last_road_id = road_id;
    }
    if(external_id[0] == 'T'){
        if(getSpeed() < 0.1){
            if(road_id == "2/0to2/2" && simTime() - lastDroveAt > 3){
                // set the triffic lights
                getMapSystem()->getManager()->commandSetTrafficLightPhaseIndex("2/2", 6);
                srt->dblMap["stopPos"] = 280.6 - this->getLanePosition();
                srt->dblMap["startTime"] = simTime().dbl();
            }
            if(road_id == "2/2to0/2"){
                getMapSystem()->getManager()->commandSetTrafficLightPhaseIndex("0/2", 0);
                srt->dblMap["secodStopPos"] = 280.6 - this->getLanePosition();
                srt->dblMap["secodStopTime"] = simTime().dbl();
                srt->dblMap["throughNextEdgeTime"] = simTime().dbl() - srt->dblMap["reachNextEdgeTime"];
            }
        }else{
            lastDroveAt = simTime();
            if(road_id == "2/2to0/2"){
                if(srt->dblMap["t50mTime"] < srt->dblMap["reachNextEdgeTime"] && getLanePosition() > 50){
                    srt->dblMap["t50mTime"] = simTime().dbl();
                    srt->dblMap["50mTime"] = simTime().dbl() - srt->dblMap["reachNextEdgeTime"];
                }
                if(srt->dblMap["t100mTime"] < srt->dblMap["reachNextEdgeTime"] && getLanePosition() > 100){
                    srt->dblMap["t100mTime"] = simTime().dbl();
                    srt->dblMap["100mTime"] = simTime().dbl() - srt->dblMap["reachNextEdgeTime"];
                }
                if(srt->dblMap["t150mTime"] < srt->dblMap["reachNextEdgeTime"] && getLanePosition() > 150){
                    srt->dblMap["t150mTime"] = simTime().dbl();
                    srt->dblMap["150mTime"] = simTime().dbl() - srt->dblMap["reachNextEdgeTime"];
                }
            }
        }
    }
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
    if(external_id[0] == 'T'){
        string name = "passtime" + getMapSystem()->dou2str(srt->dblMap["speedInt"])
                + external_id.substr(external_id.length() - 1);
        string title = "";
        title += "roundID,num,lenInt,speedInt,";
        title += "stopPos,startTime,passFirstTime,droveOutTime,passJunctionTime,reachNextEdgeTime,";
        title += "throughNextEdgeTime,secodStopTime,secodStopPos,50mTime,100mTime,150mTime";
        srt->changeName(name, title) << srt->dblMap["roundID"] << srt->dblMap["num"] << srt->dblMap["lenInt"]
                << srt->dblMap["speedInt"] << srt->dblMap["stopPos"] << srt->dblMap["startTime"]
                << srt->dblMap["passFirstTime"] << srt->dblMap["droveOutTime"] << srt->dblMap["passJunctionTime"]
                << srt->dblMap["reachNextEdgeTime"] << srt->dblMap["throughNextEdgeTime"]
                << srt->dblMap["secodStopTime"] << srt->dblMap["secodStopPos"] << srt->dblMap["50mTime"]
                << srt->dblMap["100mTime"] << srt->dblMap["150mTime"] << srt->endl;
    }
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
