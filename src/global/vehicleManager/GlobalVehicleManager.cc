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

#include "GlobalVehicleManager.h"

Define_Module(GlobalVehicleManager);

void GlobalVehicleManager::initialize() {
    // TODO - Generated method body
    // TODO just for test use
    testMsg = new cMessage("testMsg");
    targetNum = 500;
    intMap["roundID"] = 0;
    intMap["turnID"] = 1;
    intMap["carSID"] = 0;
    intMap["tickID"] = 0;
    intMap["oldTickID"] = 0;
    intMap["startTickID"] = 0;
    intMap["interval"] = 10;
    srt = StatisticsRecordTools::request();
    scheduleAt(simTime() + 0.1, testMsg);
}

void GlobalVehicleManager::handleMessage(cMessage *msg) {
    // TODO - Generated method body
    if(msg == testMsg){
        if(getMapSystem()->isInitializedFinished()){
            // the car adding logic
            // record tick number
            intMap["tickID"]++;
            if(intMap["tickID"] == 1){
                if(!getMapSystem()->addOneVehicle("P00", "L00P", "2/4to2/2", 0, 20)){
                    std::cout << "adding car failed:P00,L00P" << std::endl;
                }
            }
            // if active vehicle count is zero, start new round;
            if(getMapSystem()->getActiveVehicleCount() == 1 && intMap["oldTickID"] + 10 < intMap["tickID"]){
                std::cout << intMap["roundID"] << std::endl;
                // modify roundID
                intMap["roundID"]++;
                int num = (intMap["roundID"] - 4 + 15) / (3 * 5); // round = 4, num =1
                int lenInt = ((intMap["roundID"] - 1)/3) % 5;   // round = 4, lenInt=1
                int speedInt = (intMap["roundID"] - 1) % 3; // round = 4, speedInt = 0
                srt->dblMap["roundID"] = intMap["roundID"];
                srt->dblMap["num"] = num;
                srt->dblMap["lenInt"] = lenInt;
                srt->dblMap["speedInt"] = speedInt;

                //int num = (intMap["roundID"] - 2 + 15) / (3 * 5);
                //std::cout << "round = " << intMap["roundID"] << " ,num = " << num << std::endl;

                // set & reset attributes
                // intMap["carSID"] = 0;
                intMap["oldTickID"] = intMap["tickID"];
                intMap["startTickID"] = intMap["tickID"];
                intMap["turnID"] = 0;
                getMapSystem()->getManager()->commandSetTrafficLightPhaseIndex("2/2", 5);
                getMapSystem()->getManager()->commandSetTrafficLightPhaseIndex("0/2", 2);
            }else{
                // for each round
                if(intMap["roundID"] > 0){
                    // caculate the car number: for each number there 15 kinds: Speed:[S/N/F] & fisrt car length:L01~L04,L00(L05==L00);
                    // when the roundID == 1, there no other car in the map system.
                    // when the roundID == 2, L01 insert befrore target car
                    int num = srt->dblMap["num"];
                    int lenInt = srt->dblMap["lenInt"];
                    string lenStr = "L0" + getMapSystem()->int2str(lenInt);
                    int speedInt = srt->dblMap["speedInt"];
                    string speedStr = "";
                    switch(speedInt){
                        case 0:
                            speedStr = "S";
                            break;
                        case 1:
                            speedStr = "N";
                            break;
                        case 2:
                            speedStr = "F";
                            break;
                        default:
                            break;
                    }
                    if(intMap["turnID"] < num){
                        // generate the other cars in different directions
                        string vid = "";
                        string vtype = "";
                        switch((intMap["tickID"] - intMap["startTickID"] + intMap["interval"]) % (intMap["interval"])){
                            case 6:
                                if(intMap["turnID"] == 0){
                                    // generate the first car turn left
                                    intMap["turnID"]++;
                                    intMap["carSID"]++;
                                    string vid = "L" + getMapSystem()->int2str(intMap["carSID"]);
                                    string vtype = lenStr + speedStr;
                                    if(!getMapSystem()->addOneVehicle(vid, vtype, "2/0to2/2", 0, 10, 0, 1)){
                                        std::cout << "adding car failed:" << vid << "," << vtype << std::endl;
                                    }
                                }else{
                                    // turn left
                                    //std::cout << "turn left" << std::endl;
                                    intMap["turnID"]++;
                                    intMap["carSID"]++;
                                    vid = "L" + getMapSystem()->int2str(intMap["carSID"]);
                                    vtype = "L00" + speedStr;
                                    if(!getMapSystem()->addOneVehicle(vid, vtype, "2/0to2/2", 0, 10, 0, 1)){
                                        std::cout << "adding car failed:" << vid << "," << vtype << std::endl;
                                    }
                                }
                                break;
                            case 3:
                                break;
                                // stright
                                //std::cout << "stright" << std::endl;
                                intMap["carSID"]++;
                                vid = "S" + getMapSystem()->int2str(intMap["carSID"]);
                                vtype = vtype + "L00" + "N";
                                if(!getMapSystem()->addOneVehicle(vid, vtype, "2/0to2/2", 0, 10, 0, 1)){
                                    std::cout << "adding car failed:" << vid << "," << vtype << std::endl;
                                }
                                break;
                            case 9:
                                break;
                                // turn right
                                //std::cout << "turn right" << std::endl;
                                intMap["carSID"]++;
                                vid = "R" + getMapSystem()->int2str(intMap["carSID"]);
                                vtype = vtype + "L00" + "N";
                                if(!getMapSystem()->addOneVehicle(vid, vtype, "2/0to2/2", 0, 10, 0, 0)){
                                    std::cout << "adding car failed:" << vid << "," << vtype << std::endl;
                                }
                                break;
                            default:
                                break;
                        }
                    }else if(intMap["turnID"] == num){
                        // generate the target car
                        // turn left
                        intMap["turnID"]++;
                        intMap["carSID"]++;
                        string vtype = "";
                        vtype = vtype + "L00" + "N";
                        string vid = "T" + getMapSystem()->int2str(intMap["carSID"]) + vtype;
                        if(!getMapSystem()->addOneVehicle(vid, vtype, "2/0to2/2", 0, 10, 0, 1)){
                            std::cout << "adding car failed:" << vid << "," << vtype << std::endl;
                        }
                    }

                }
            }
        }
        scheduleAt(simTime() + 0.1, testMsg);
    }
}

void GlobalVehicleManager::finish() {
    // TODO - Generated method body
    for(std::map<string, cMessage*>::iterator it = msgMap.begin(); it != msgMap.end(); it++){
        cancelAndDelete(it->second);
    }
    srt->outputSeparate("passtime.txt", "results");

    srt->release();
}

GlobalMapSystem* GlobalVehicleManager::getMapSystem() {
    // TODO - Generated method body
    if(!map){
        map = GlobalMapSystemAccess().get();
    }
    ASSERT(map);
    return map;
}

void GlobalVehicleManager::updateVehicleList() {
    // TODO - Generated method body
}

void GlobalVehicleManager::addOneVehicle(VehicleType type) {
    // TODO - Generated method body
    getMapSystem()->addOneVehicle();
}

GlobalVehicleManager::GlobalVehicleManager() :
        srt(NULL) {
    map = NULL;
    testMsg = NULL;
    targetNum = 100;
}

GlobalVehicleManager::~GlobalVehicleManager() {
}

void GlobalVehicleManager::addVehicles(VehicleType type, int num) {
    // TODO - Generated method body
    getMapSystem()->addVehicles(num);
}
