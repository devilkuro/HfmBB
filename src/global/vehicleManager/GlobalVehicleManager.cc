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

GlobalVehicleManager::GlobalVehicleManager() :
        srt(NULL) {
    map = NULL;
    testMsg = NULL;
    targetNum = 100;

    // generating car flow and related
    carSpawnTimeLimit = 7200 * 3;
    carFlowXMLPath = "";
    carVTypeXMLPath = "";
    generateNewXMLFile = true;
    maxCarFlowRate = 0.6;
    minCarFlowRate = 0;
    carSpawnJudgeInterval = 1;
    carSpawnPeriod = 7200;
    carSpawnOffset = 900;
    carSpawnStartTime = 1000;
    useCarSpawnOffset = true;
    carNumLimit = 3 * (minCarFlowRate + maxCarFlowRate) * 0.5 * carSpawnTimeLimit;
    reverseCarFlowRate = 0.6;
}

GlobalVehicleManager::~GlobalVehicleManager() {
}

void GlobalVehicleManager::initialize() {
    // TODO - Generated method body
    // TODO just for test use
    testMsg = new cMessage("testMsg");
    targetNum = 500;

    // generating car flow and related
    carNumLimit = hasPar("carNumLimit") ? par("carNumLimit") : 19440;
    carFlowXMLPath = hasPar("carFlowXMLPath") ? par("carFlowXMLPath") : "";
    generateNewXMLFile = hasPar("generateNewXMLFile") ? par("generateNewXMLFile") : true;

    maxCarFlowRate = hasPar("maxCarFlowRate") ? par("maxCarFlowRate") : 0.6;
    minCarFlowRate = hasPar("minCarFlowRate") ? par("minCarFlowRate") : 0;
    carSpawnJudgeInterval = hasPar("carSpawnJudgeInterval") ? par("carSpawnJudgeInterval") : 1;
    carSpawnPeriod = hasPar("carSpawnPeriod") ? par("carSpawnPeriod") : 7200;
    carSpawnOffset = hasPar("carSpawnOffset") ? par("carSpawnOffset") : 900;
    carSpawnStartTime = hasPar("carSpawnStartTime") ? par("carSpawnStartTime") : 1000;
    useCarSpawnOffset = hasPar("useCarSpawnOffset") ? par("useCarSpawnOffset") : true;
    carSpawnTimeLimit = hasPar("carSpawnTimeLimit") ? par("carSpawnTimeLimit") : carSpawnPeriod * 3;
    reverseCarFlowRate = hasPar("reverseCarFlowRate") ? par("reverseCarFlowRate") : 0.6;

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
            if(carFlowXMLPath == ""){
                string launchConfigFile = getRouXMLFromLaunchConfig();
                carFlowXMLPath = launchConfigFile.replace(launchConfigFile.length() - 7, 7, "cfg.xml");
            }
            // generate car flow
            if(generateNewXMLFile){
                // start generating process
                generateNewXMLFile = false;
                cout << carFlowXMLPath << endl;
                generateCarFlowFile();
            }
            // load car flow
            if(carMapByID.size() == 0){
                cout << "load car flow file..." << endl;
                // add the dummy car
                SMTCarInfo car = SMTCarInfo::getDefaultVeicleTypeInfo(*SMTCarInfo::getDefaultVeicleTypeList().begin());
                car.id = "car00";
                car.time = simTime().dbl();
                car.type = SMTCarInfo::SMTCARINFO_ROUTETYPE_OD;
                car.origin = "0/0to2/0";
                car.destination = "0/0to2/0";
                carMapByID[car.id] = car;
                carIdMapByDepartTime[car.time] = car.id;
                // load car flow
                loadCarFlowFile();
            }
            // todo
            // generate the cars
            if(itCarIdMapByDepartTime != carIdMapByDepartTime.end()){
                double time = simTime().dbl();
                string id = itCarIdMapByDepartTime->second;
                addOneVehicle(carMapByID[id]);
                itCarIdMapByDepartTime++;
                if(itCarIdMapByDepartTime != carIdMapByDepartTime.end()){
                    time = itCarIdMapByDepartTime->first;
                }else{
                    time = simTime().dbl() + 3600;
                }
                scheduleAt(time, testMsg);
            }else{
                endSimulation();
            }
            //endSimulation();
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

void GlobalVehicleManager::addOneVehicle(SMTCarInfo car) {
    // TODO - Generated method body
    //cout << "generate a car at " << car.time << " second" << endl;
    switch(car.type){
        case SMTCarInfo::SMTCARINFO_ROUTETYPE_OD:
            if(!getMapSystem()->addOneVehicle(car.id, car.vtype, car.origin, car.time, 0, car.maxSpeed,
                    getMapSystem()->getLaneNumber(car.origin) - 1)){
                cout << "car id: " << car.id << ", road: " << car.origin << ", @" << car.time << endl;
            }
            break;
        case SMTCarInfo::SMTCARINFO_ROUTETYPE_LOOP:
            // can not handle this type now.
            break;
        default:
            break;
    }
}

void GlobalVehicleManager::addVehicles(VehicleType type, int num) {
    // TODO - Generated method body
    getMapSystem()->addVehicles(num);
}

void GlobalVehicleManager::generateCarFlowFile() {
    // TODO generate the car flow file.
    // generate the start and end edges
    vector<string> vecInStartPoint;
    vector<string> vecInEndPoint;
    vector<string> vecOutEndPoint;
    vector<string> vecOutStartPoint;
    vector<string> vecInPoint;
    vector<string> vecOutPoint;
    list<string> roadList = getMapSystem()->getAllEdges();
    string road = "";
    string start = "2/2";
    string end = "2/4";

    for(list<string>::iterator it = roadList.begin(); it != roadList.end(); it++){
        road = *it;
        // end to start and not start from end
        if(road.substr(road.length() - start.length()) == start && road.substr(0, end.length()) != end){
            vecInStartPoint.push_back(road);
            cout << road << endl;
        }
        // end to end and not start from start
        if(road.substr(road.length() - end.length()) == end && road.substr(0, start.length()) != start){
            vecOutStartPoint.push_back(road);
            cout << road << endl;
        }
        // start from end and not end to start
        if(road.substr(0, end.length()) == end && road.substr(road.length() - start.length()) != start){
            vecOutEndPoint.push_back(road);
            cout << road << endl;
        }
        // start from start and not end to end
        if(road.substr(0, start.length()) == start && road.substr(road.length() - end.length()) != end){
            vecInEndPoint.push_back(road);
            cout << road << endl;
        }
    }
    cout << vecInStartPoint.size() << endl;
    cout << vecOutStartPoint.size() << endl;
    cout << vecOutEndPoint.size() << endl;
    cout << vecInEndPoint.size() << endl;
    // generate vtype vector
    list<string> listVType = SMTCarInfo::getDefaultVeicleTypeList();
    vector<string> vecVType = vector<string>(listVType.begin(), listVType.end());
    // generate car flow xml files
    carFlowHelper.loadXML(carFlowXMLPath);
    carFlowHelper.clear(true);
    double time = carSpawnStartTime;
    string prefix = "car";
    int carNum = 0;
    int maxJudgeTimes = carSpawnTimeLimit / carSpawnJudgeInterval; // default: mjt = cstl(21600)/csji(1) = 216000
    double maxSpawnProbabilityForOneJudge;
    double minSpawnProbabilityForOneJudge;
    for(int i = 0; i < maxJudgeTimes; ++i){
        minSpawnProbabilityForOneJudge = minCarFlowRate * carSpawnJudgeInterval;
        if((int) (time / carSpawnPeriod) % 2 == 0){
            maxSpawnProbabilityForOneJudge = maxCarFlowRate * carSpawnJudgeInterval;
        }else{
            maxSpawnProbabilityForOneJudge = (reverseCarFlowRate * (maxCarFlowRate - minCarFlowRate) + minCarFlowRate)
                    * carSpawnJudgeInterval;
        }
        // from start to end
        for(unsigned int j = 0; j < vecInStartPoint.size(); j++){
            if(dblrand()
                    < SinFuncFixed(time - carSpawnStartTime, carSpawnPeriod, maxSpawnProbabilityForOneJudge,
                            minSpawnProbabilityForOneJudge)){
                string carid = prefix + Fanjing::StringHelper::int2str(carNum++);
                string origin = vecInStartPoint[j];
                string destination = vecOutEndPoint[intrand(vecOutEndPoint.size())];
                string vType = vecVType[intrand(vecVType.size())];
                carFlowHelper.addODCar(prefix + Fanjing::StringHelper::int2str(carNum), origin, destination, time,
                        vType);
                if(carNum > carNumLimit){
                    i = maxJudgeTimes;
                }
            }
        }
        if((int) (time / carSpawnPeriod) % 2 == 1){
            maxSpawnProbabilityForOneJudge = maxCarFlowRate * carSpawnJudgeInterval;
        }else{
            maxSpawnProbabilityForOneJudge = (reverseCarFlowRate * (maxCarFlowRate - minCarFlowRate) + minCarFlowRate)
                    * carSpawnJudgeInterval;
        }
        // from end to start
        for(unsigned int j = 0; j < vecInEndPoint.size(); j++){
            if(dblrand()
                    < SinFuncFixed(time - carSpawnStartTime, carSpawnPeriod, maxSpawnProbabilityForOneJudge,
                            minSpawnProbabilityForOneJudge)){
                string carid = prefix + Fanjing::StringHelper::int2str(carNum++);
                string origin = vecOutStartPoint[j];
                string destination = vecInEndPoint[intrand(vecInEndPoint.size())];
                string vType = vecVType[intrand(vecVType.size())];
                carFlowHelper.addODCar(prefix + Fanjing::StringHelper::int2str(carNum), origin, destination, time,
                        vType);
                if(carNum > carNumLimit){
                    i = maxJudgeTimes;
                }
            }
        }
        // out put the process infomation
        if((i & 1023) == 1023){
            cout << "process: " << "t: " << (int) 100 * time / carSpawnTimeLimit << "%, car: "
                    << 100 * carNum / carNumLimit << "%" << endl;
        }
        time += carSpawnJudgeInterval;
    }
    cout << "finished: " << "t: " << (int) 100 * time / carSpawnTimeLimit << "%, car: " << 100 * carNum / carNumLimit
            << "%" << endl;
    cout << "carSpawnTimeLimit: " << carSpawnTimeLimit << ", carNumLimit: " << carNumLimit << endl;
    carFlowHelper.save();
}


string GlobalVehicleManager::getStartPoint(string road) {
    string result = "";
    if(road.find("to")!=string::npos){
        result = road.substr(0,road.find("to"));
    }
    return result;
}

string GlobalVehicleManager::getEndPoint(string road) {
    string result = "";
    if(road.find("to")!=string::npos){
        if (road.find("_"==string::npos)) {
            result = road.substr(road.find("to"));
        }else{
            result = road.substr(road.find("to"),road.find("_")-road.find("to"));
        }
    }
    return result;
}

SMTCarInfo GlobalVehicleManager::getCarInfo(string id) {
    return carMapByID[id];
}

double GlobalVehicleManager::SinFuncFixed(double t, double period, double up, double down) {
    return down + (up - down) / 2 * (1 + sin((t / period) * (2 * M_PI) - M_PI / 2));
}

void GlobalVehicleManager::loadCarFlowFile() {
    if(carFlowHelper.loadXML(carFlowXMLPath) == false){
        generateCarFlowFile();
        carFlowHelper.loadXML(carFlowXMLPath);
    }
    SMTCarInfo car = carFlowHelper.getFirstCar();
    while(car.id != ""){
        if(useCarSpawnOffset){
            car.time += ((double) intrand(2 * carSpawnOffset * 10)) / 10;
        }else{
            car.time += carSpawnOffset;
        }
        carMapByID[car.id] = car;
        carIdMapByDepartTime[car.time] = car.id;
        car = carFlowHelper.getNextCar();
    }
    itCarIdMapByDepartTime = carIdMapByDepartTime.begin();
}

string GlobalVehicleManager::getRouXMLFromLaunchConfig(string launchFilePath) {
    if(launchFilePath == ""){
        return GlobalMobilityLaunchdAccess().get()->getRouXML();
    }else{
        return GlobalMobilityLaunchd::getRouXMLFromLaunchConfig(launchFilePath);
    }
}

