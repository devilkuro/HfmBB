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
#include "ShortPath.h"
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
    if (getMapSystem()->isInitializedFinished()) {
        double pos = getLanePosition();
        if(pos>0){
            if(pos > 25){
                allowLaneChange();
            }else{
                disableLaneChange();
            }
        }
    }
    // path process
    if(!hasRouted){
        if(getMapSystem()->isInitializedFinished()){
            Config cfg;
            cfg.init(getMapSystem());
            EV << "cfg.init finished!" << endl;
            CShortPath shortPath;
            shortPath.InitData(&cfg); //初始化
            string start = road_id;
            string end = "4900041#1";
            shortPath.Search(start,end); //开始搜索
            EV<< start << ":"<< end<<endl;
            //shortPath.route = getMapSystem()->getRandomRoute(start);
            EV<< shortPath.route.size() <<endl;
            for(list<string>::iterator it = shortPath.route.begin();it!=shortPath.route.end();it++){
                EV<<*it<<endl;
            }
            getMapSystem()->setVehicleRouteByEdgeList(external_id, shortPath.route);
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
            statistic_road_enterTime = simTime().dbl();
        }
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

void TraCIMobility_Fixed::disableLaneChange() {
    getMapSystem()->setLaneChangePermission(getExternalId(),false);
}

void TraCIMobility_Fixed::allowLaneChange() {
    getMapSystem()->setLaneChangePermission(getExternalId(),true);
}

double TraCIMobility_Fixed::getLanePosition() {
    return getManager()->commandGetLanePosition(getExternalId());
}
