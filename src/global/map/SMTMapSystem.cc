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

#include "SMTMapSystem.h"
Define_Module(SMTMapSystem)
SMTMapSystem::SMTMapSystem() {
    releasedCarNum = 0;
}

SMTMapSystem::~SMTMapSystem() {

}

void SMTMapSystem::initialize(int stage) {
    GlobalMapSystem::initialize(stage);
    if(stage == 1){
        list<string> allEgetList = getAllEdges();
        // 生成主要道路列表
        for(list<string>::iterator it = allEgetList.begin(); it != allEgetList.end(); it++){
            if((*it)[0] != ':'){
                normalEdgeList.push_back(*it);
            }
        }
        xmlName = "matrixlog";
        // 生成主要道路连接拓扑
        ganerateMapTopology();
        // todo 改为由xml读入？感觉有点蛋疼啊，先写静态的吧。
        queueMap["2/4to2/2_0"] = SMTCarInfoQueue("2/4to2/2_0", xmlName, 577.30, 5);    // to="2/2to0/2"
        queueMap["2/4to2/2_0"].setCycleInfo(120,120,0);
        queueMap["2/4to2/2_1"] = SMTCarInfoQueue("2/4to2/2_1", xmlName, 577.30, 23.01);    // to="2/2to2/0"
        queueMap["2/4to2/2_1"].setCycleInfo(120,30,30);
        queueMap["2/4to2/2_2"] = SMTCarInfoQueue("2/4to2/2_2", xmlName, 577.30, 8.23); // to="2/2to4/2"
        queueMap["2/4to2/2_2"].setCycleInfo(120,30,30);
        queueMap["2/2to2/4_0"] = SMTCarInfoQueue("2/2to2/4_0", xmlName, 577.30, 7.85); // to="2/4to4/4"
        queueMap["2/2to2/4_0"].setCycleInfo(120,120,0);
        queueMap["2/2to2/4_1"] = SMTCarInfoQueue("2/2to2/4_1", xmlName, 577.30, 23.01); // to="2/4to2/6"
        queueMap["2/2to2/4_1"].setCycleInfo(120,30,90);
        queueMap["2/2to2/4_2"] = SMTCarInfoQueue("2/2to2/4_2", xmlName, 577.30, 8.23); // to="2/4to0/4"
        queueMap["2/2to2/4_2"].setCycleInfo(120,30,90);
        // todo
    }
}

void SMTMapSystem::uploadRoute(SMTCarInfo car, list<string> route, double time) {
    for(list<string>::iterator it = route.begin(); it != route.end(); it++){
        if(*it == "2/4to2/2"){
            it++;
            if(it != route.end()){
                if(*it == "2/2to0/2"){
                    queueMap["2/4to2/2_0"].insertCar(car, time, 0);
                }else if(*it == "2/2to2/0"){
                    queueMap["2/4to2/2_1"].insertCar(car, time, 0);
                }else if(*it == "2/2to4/2"){
                    queueMap["2/4to2/2_2"].insertCar(car, time, 0);
                }else{
                    // 什么都不做
                }
                break;
            }else{
                break;
            }
        }
        if(*it == "2/2to2/4"){
            it++;
            if(it != route.end()){
                if(*it == "2/4to4/4"){
                    queueMap["2/2to2/4_0"].insertCar(car, time, 0);
                }else if(*it == "2/4to2/6"){
                    queueMap["2/2to2/4_1"].insertCar(car, time, 0);
                }else if(*it == "2/4to0/4"){
                    queueMap["2/2to2/4_2"].insertCar(car, time, 0);
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

void SMTMapSystem::enterRoad(SMTCarInfo car, string road, double time) {
    releasedCarNum++;
    if(road == "2/2to0/2"){
        queueMap["2/4to2/2_0"].releaseCar(car.id, time,getAvgTravelTimeByEdge("2/4to2/2"));
    }else if(road == "2/2to2/0"){
        queueMap["2/4to2/2_1"].releaseCar(car.id, time,getAvgTravelTimeByEdge("2/4to2/2"));
    }else if(road == "2/2to4/2"){
        queueMap["2/4to2/2_2"].releaseCar(car.id, time,getAvgTravelTimeByEdge("2/4to2/2"));
    }else if(road == "2/4to4/4"){
        queueMap["2/2to2/4_0"].releaseCar(car.id, time,getAvgTravelTimeByEdge("2/2to2/4"));
    }else if(road == "2/4to2/6"){
        queueMap["2/2to2/4_1"].releaseCar(car.id, time,getAvgTravelTimeByEdge("2/2to2/4"));
    }else if(road == "2/4to0/4"){
        queueMap["2/2to2/4_2"].releaseCar(car.id, time,getAvgTravelTimeByEdge("2/2to2/4"));
    }else{
        // 什么都不做
        releasedCarNum--;
    }
    if((releasedCarNum&255) ==0){
        cout<<"releasedCarNum: "<<releasedCarNum<<endl;
    }
    // 统计1000辆车后结束仿真
}

void SMTMapSystem::finish() {
    SMTCarInfoQueue qinfo;
    qinfo.saveResults(xmlName);
    qinfo.releaseXML();
}

void SMTMapSystem::ganerateMapTopology() {
    // todo 暂时什么都没做，之后需要改为由xml录入
}
