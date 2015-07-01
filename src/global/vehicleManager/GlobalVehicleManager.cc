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
    intMap["turnID"] = 0;
    intMap["carSID"] = 0;
    intMap["tickID"] = 0;
    intMap["oldTickID"] = 0;
    intMap["startTickID"] = 0;
    intMap["interval"] = 10;
    scheduleAt(simTime() + 0.1, testMsg);
}

void GlobalVehicleManager::handleMessage(cMessage *msg) {
    // TODO - Generated method body
    if(msg == testMsg){
        if(getMapSystem()->isInitializedFinished()){
            // the car adding logic
            // record tick number
            intMap["tickID"]++;
            // if active vehicle count is zero, start new round;
            if(getMapSystem()->getActiveVehicleCount() == 0){
                // modify roundID
                intMap["roundID"]++;
                // set & reset attributes
                intMap["carSID"] = 0;
                intMap["oldTickID"] = intMap["tickID"];
                intMap["startTickID"] = intMap["tickID"];
                intMap["turnID"] = 0;
            }
            // for each round
            if(intMap["roundID"] > 0){
                // caculate the car number: for each number there 15 kinds: Speed:[S/N/F] & fisrt car length:L01~L04,L00(L05==L00);
                // when the roundID == 1, there no other car in the map system.
                int num = (intMap["roundID"] - 1) / 3 * 5 + 1;
                int lenInt = (intMap["roundID"] - 1 + 1) % 5;
                string lenStr = "L0" + getMapSystem()->int2str(lenInt);
                int speedInt = ((intMap["roundID"] - 1) / 5) % 3;
                string speedStr = "";
                switch(speedInt){
                    case 0:
                        speedStr = "S";
                        break;
                    case 0:
                        speedStr = "N";
                        break;
                    case 0:
                        speedStr = "F";
                        break;
                    default:
                        break;
                }
                if(intMap["turnID"] < num){
                    // generate the first car
                    if(intMap["startTickID"] == intMap["tickID"]){
                        intMap["turnID"]++;
                        intMap["carSID"]++;
                        string vid = "L" + getMapSystem()->int2str(intMap["carSID"]);
                        string vtype = lenStr + speedStr;
                        getMapSystem()->addVehicles(1, vid, vtype, "2/0to2/2");
                    }else{
                        // generate the other cars in different directions
                        switch((intMap["tickID"] - intMap["startTickID"] + intMap["interval"] + 1)
                                % (intMap["interval"] + 1)){
                            case 3:
                                // turn left
                                intMap["turnID"]++;
                                intMap["carSID"]++;
                                string vid = "L" + getMapSystem()->int2str(intMap["carSID"]);
                                string vtype = "L00" + speedStr;
                                getMapSystem()->addVehicles(1, vid, vtype, "2/0to2/2");
                                break;
                            case 6:
                                // stright
                                intMap["carSID"]++;
                                string vid = "S" + getMapSystem()->int2str(intMap["carSID"]);
                                string vtype = "L00" + "N";
                                getMapSystem()->addVehicles(1, vid, vtype, "2/0to2/2");
                                break;
                            case 9:
                                // turn right
                                intMap["carSID"]++;
                                string vid = "R" + getMapSystem()->int2str(intMap["carSID"]);
                                string vtype = "L00" + "N";
                                getMapSystem()->addVehicles(1, vid, vtype, "2/0to2/2");
                                break;
                            default:
                                break;
                        }
                    }
                }else{
                    // generate the target car
                    // turn left
                    intMap["turnID"]++;
                    intMap["carSID"]++;
                    string vid = "T" + getMapSystem()->int2str(intMap["carSID"]);
                    string vtype = "L00" + "S";
                    getMapSystem()->addVehicles(1, vid, vtype, "2/0to2/2");
                }

            }
        }else{
            scheduleAt(simTime() + 0.1, testMsg);
        }
    }
}

void GlobalVehicleManager::finish() {
    // TODO - Generated method body
    for(std::map<string, cMessage*>::iterator it = msgMap.begin(); it != msgMap.end(); it++){
        cancelAndDelete(it->second);
    }
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

GlobalVehicleManager::GlobalVehicleManager() {
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
