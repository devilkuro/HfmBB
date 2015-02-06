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
    // TODO 2014-11-9 debug use
//    if (getMapSystem()->isInitializedFinished()) {
//        double pos = getLanePosition();
//        if(pos>0){
//            if(pos > 25){
//                allowLaneChange();
//            }else{
//                disableLaneChange();
//            }
//        }
//    }
    // path process
    if(!hasRouted){
        if(getMapSystem()->isInitializedFinished()){
            EV << "cfg.init finished!" << endl;
            // todo
            // 1st. set start and end road
            string start = road_id;
            // fixme change to fixed road
            //string end = getMapSystem()->getRandomEdgeFromCache();
            string end = "4006702#2";

            if (external_id=="node499") {
                // init seek function
                acot.init(getMapSystem());
                asmtimer.start();   // timer start
                // call seek function
                acot.seekRoute(start,end); //¿ªÊ¼ËÑË÷
                asmtimer.end(); // timer end

                // output result
                gs->changeName("node")<<external_id<<(int)asmtimer.getMilliseconds()<<gs->endl;
                gs->output("nodeTimer.txt");
                //endSimulation();// fixme comment this line
                EV<< start << ":"<< end<<endl;
                //shortPath.route = getMapSystem()->getRandomRoute(start);
                EV<< acot.seleted_route.size() <<endl;
                getMapSystem()->setVehicleRouteByEdgeList(external_id, acot.seleted_route);
                statistic_start_time = simTime().dbl();
            }else{
                list<string> route = getMapSystem()->getRandomRoute(start);
                if (start[0]!=':') {
                    getMapSystem()->setVehicleRouteByEdgeList(external_id, route);
                }
            }
            hasRouted = true;
        }
    }
    // fixme reseek flag
    bool reseekFlag = false;
    // add judgement here
    // example:  if(road_id != last_road_id){reseekFlag = true}

    if (reseekFlag) {
        if (external_id=="node499") {
            // fixme add reseek function here
            // acot.reseek(road_id,end);
            // getMapSystem()->setVehicleRouteByEdgeList(external_id, acot.seleted_route);
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
                gs->changeName("road statistics - vehicle number - travel time - " + last_road_id) << simTime().dbl()
                        << statistic_road_enterTime << statistic_junction_enterVehicleNum
                        << statistic_road_enterVehicleNum << simTime().dbl() - statistic_road_enterTime << gs->endl;
            }
        }
        // record the data while entering new edge
        if(road_id == "1/1to1/2"){
            statistic_road_enterVehicleNum = getMapSystem()->getVehicleNumByEdge("2/2to1/2");
            statistic_junction_enterVehicleNum = statistic_road_enterVehicleNum;
            statistic_junction_enterVehicleNum += getMapSystem()->getVehicleNumByEdge("1/3to1/2");
            statistic_junction_enterVehicleNum += getMapSystem()->getVehicleNumByEdge("0/2to1/2");
            statistic_junction_enterVehicleNum += getMapSystem()->getVehicleNumByEdge("1/1to1/2");
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
    gs = GlobalStatisticsAccess().get();
}

void TraCIMobility_Fixed::finish() {
    TraCIMobility::finish();
    hasRouted = false;
    hasInitialized = false;
    getMapSystem()->unregisterVehiclePosition(last_road_id, simTime().dbl()-statistic_road_enterTime);
    if(external_id == "node499"){
        gs->changeName("node")<<external_id<<simTime().dbl()-statistic_start_time<<gs->endl;
        gs->output("node499.txt");
        endSimulation();
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
