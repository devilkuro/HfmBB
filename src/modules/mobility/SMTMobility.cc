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

SMTMobility::~SMTMobility() {

}

void SMTMobility::processAfterRouting() {
    // 选路之后每个周期都会执行(请确保判定完备,不要执行复杂度过高的操作)
}

void SMTMobility::statisticAtFinish() {
    // 结束时的统计方法
}

void SMTMobility::processAtRouting() {
    // 选路阶段
    if(external_id == "car00"){
        commandSetSpeed(0);
    }else{
        enableLaneChangeControl = par("enableLaneChangeControl").boolValue();
        carInfo = getVehicleManager()->getCarInfo(external_id);
        string start = "2/2";
        string end = "2/4";
        string to = "to";
        string head = GlobalVehicleManager::getEndPoint(carInfo.origin);
        string tail = GlobalVehicleManager::getStartPoint(carInfo.destination);
        route.clear();
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
        if(hasPar("disallowOvertake") ? par("disallowOvertake").boolValue() : true){
            getMapSystem()->disableOvertake(external_id);
        }
    }
}

void SMTMobility::processWhenChangeRoad() {
    // 当车辆首次进入某条道路时执行
    int duration = 5000;
    if(road_id == "2/4to2/2" || road_id == "2/2to2/4"){
        if(true){
            for(list<string>::iterator it = route.begin(); it != route.end(); it++){
                if(*it == "2/4to2/2"){
                    it++;
                    if(it != route.end()){
                        if(*it == "2/2to0/2"){
                            getMapSystem()->getManager()->commandChangeLane(external_id, 0, duration);
                        }else if(*it == "2/2to2/0"){
                            getMapSystem()->getManager()->commandChangeLane(external_id, 1, duration);
                        }else if(*it == "2/2to4/2"){
                            getMapSystem()->getManager()->commandChangeLane(external_id, 2, duration);
                        }else{
                            // 什么都不做
                        }
                        break;
                    }else{
                        break;
                    }
                }else if(*it == "2/2to2/4"){
                    it++;
                    if(it != route.end()){
                        if(*it == "2/4to4/4"){
                            getMapSystem()->getManager()->commandChangeLane(external_id, 0, duration);
                        }else if(*it == "2/4to2/6"){
                            getMapSystem()->getManager()->commandChangeLane(external_id, 1, duration);
                        }else if(*it == "2/4to0/4"){
                            getMapSystem()->getManager()->commandChangeLane(external_id, 2, duration);
                        }else{
                            // 什么都不做
                        }
                        break;
                    }else{
                        break;
                    }
                }
            }
        }
        getMapSystem()->uploadRoute(carInfo, route, simTime().dbl());
        beWatched = true;
    }else if(beWatched){
        getMapSystem()->enterRoad(carInfo, road_id, simTime().dbl());
    }

}

void SMTMobility::processWhenInitializingRoad() {
    // 车辆首次出现在地图上时执行
}

void SMTMobility::processWhenNextPosition() {
    // 车辆变更位置时出现(请确保判定完备,不要执行复杂度过高的操作)
}

void SMTMobility::changeRoute(list<string> route) {
    getMapSystem()->setVehicleRouteByEdgeList(external_id, route);
}
