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

    // generating car flow and related
    carNumLimit = hasPar("carNumLimit") ? par("carNumLimit") : 2000;
    carFlowXMLPath = hasPar("carFlowXMLPath") ? par("carFlowXMLPath") : "";
    generateNewXMLFile = hasPar("generateNewXMLFile") ? par("generateNewXMLFile") : true;

    maxCarFlowRate = hasPar("maxCarFlowRate") ? par("maxCarFlowRate") : 0.6;
    minCarFlowRate = hasPar("minCarFlowRate") ? par("minCarFlowRate") : 0;
    carSpawnJudgeInterval = hasPar("carSpawnJudgeInterval") ? par("carSpawnJudgeInterval") : 0.1;
    carSpawnPeriod = hasPar("carSpawnPeriod") ? par("carSpawnPeriod") : 7200;
    carSpawnOffset = hasPar("carSpawnOffset") ? par("carSpawnOffset") : 900;

    // initialize the vtype infomation in class SMTCarInfo
    std::string strLaunchConfig = GlobalMobilityLaunchdAccess().get()->getLaunchConfigXMLPath();

    carVTypeXMLPath = hasPar("carVTypeXMLPath") ? par("carVTypeXMLPath") : "";
    if(carVTypeXMLPath == ""){
        carVTypeXMLPath = getRouXMLFromLaunchConfig(strLaunchConfig);
    }
    Fanjing::SMTCarInfo::loadVehicleTypeXML(carVTypeXMLPath);

    srt = StatisticsRecordTools::request();
    scheduleAt(simTime() + 0.1, testMsg);
}

void GlobalVehicleManager::handleMessage(cMessage *msg) {
    // TODO - Generated method body
    if(msg == testMsg){
        // generate cars here
        if(getMapSystem()->isInitializedFinished()){
            // generate car flow
            if(generateNewXMLFile || carFlowXMLPath == ""){
                // start generating process
                generateNewXMLFile = false;
                string launchConfigFile = getRouXMLFromLaunchConfig();
                carFlowXMLPath = launchConfigFile.replace(launchConfigFile.length()-7,7,"cfg.xml");
                cout<<carFlowXMLPath<<endl;
            }
            // load car flow
            // todo
            cout << (Fanjing::SMTCarInfo::hasInitialized() ? "true" : "false") << endl;
            if(Fanjing::SMTCarInfo::getDefaultVeicleTypeList().size() > 0){
                cout
                        << Fanjing::SMTCarInfo::getDefaultVeicleTypeInfo(
                                *Fanjing::SMTCarInfo::getDefaultVeicleTypeList().begin()).id << endl;
            }
            generateCarFlowFile();
            cout << GlobalMobilityLaunchdAccess().get()->getLaunchConfigXMLPath() << endl;
            endSimulation();
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

    // generating car flow and related
    carNumLimit = 2000;
    carSpawnTimeLimit = 7200 * 3;
    carFlowXMLPath = "";
    carVTypeXMLPath = "";
    generateNewXMLFile = true;
    maxCarFlowRate = 0.6;
    minCarFlowRate = 0;
    carSpawnJudgeInterval = 0.1;
    carSpawnPeriod = 7200;
    carSpawnOffset = 900;

}

GlobalVehicleManager::~GlobalVehicleManager() {
}

void GlobalVehicleManager::addVehicles(VehicleType type, int num) {
    // TODO - Generated method body
    getMapSystem()->addVehicles(num);
}

void GlobalVehicleManager::generateCarFlowFile() {
    // TODO generate the car flow file.
    vector<string> vecStartPoint;
    vector<string> vecEndPoint;
    list<string> roadList = getMapSystem()->getAllEdges();
    string road = "";
    string target = "2/2";
    for(list<string>::iterator it = roadList.begin(); it != roadList.end(); it++){
        road = *it;
        if((road).substr(road.length() - target.length()) == target){
            vecStartPoint.push_back(road);
            cout << road << endl;
        }
    }
    road = "";
    target = "4/2";
    for(list<string>::iterator it = roadList.begin(); it != roadList.end(); it++){
        road = *it;
        if((road).substr(0, target.length()) == target){
            vecEndPoint.push_back(road);
            cout << road << endl;
        }
    }
}

string GlobalVehicleManager::getRouXMLFromLaunchConfig(string launchFilePath) {
    if (launchFilePath == "") {
        return GlobalMobilityLaunchdAccess().get()->getRouXML();
    }else{
        return GlobalMobilityLaunchd::getRouXMLFromLaunchConfig(launchFilePath);
    }
}
