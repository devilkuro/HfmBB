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

std::map<string, SMTCarInfo> GlobalVehicleManager::carMapByID;

GlobalVehicleManager::GlobalVehicleManager() :
        srt(NULL) {

    map = NULL;
    testMsg = NULL;
    targetNum = 100;

    // generating car flow and related
    disableSinFix = false;
    enableBurst = false;
    nBurstNum = 10;
    carSpawnTimeLimit = 7200 * 3;
    carFlowXMLPath = "";
    carVTypeXMLPath = "";
    generateNewXMLFile = true;
    maxCarFlowRate = 0.6;
    minCarFlowRate = 0;
    maxFreeCarFlowRate = 0.2;
    minFreeCarFlowRate = 0.1;
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
    disableSinFix = hasPar("disableSinFix") ? par("disableSinFix") : false;
    enableBurst = hasPar("enableBurst") ? par("enableBurst") : false;
    nBurstNum = hasPar("nBurstNum") ? par("nBurstNum") : 10;
    carNumLimit = hasPar("carNumLimit") ? par("carNumLimit") : 19440;
    carFlowXMLPath = hasPar("carFlowXMLPath") ? par("carFlowXMLPath") : "";
    generateNewXMLFile = hasPar("generateNewXMLFile") ? par("generateNewXMLFile") : true;

    maxCarFlowRate = hasPar("maxCarFlowRate") ? par("maxCarFlowRate") : 0.6;
    minCarFlowRate = hasPar("minCarFlowRate") ? par("minCarFlowRate") : 0.0;
    maxFreeCarFlowRate = hasPar("maxFreeCarFlowRate") ? par("maxFreeCarFlowRate") : 0.2;
    minFreeCarFlowRate = hasPar("minFreeCarFlowRate") ? par("minFreeCarFlowRate") : 0.1;
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
                carIdMapByDepartTime.push_front(car.id);
                // load car flow
                loadCarFlowFile();
            }
            // todo
            // generate the cars
            if(itCarIdMapByDepartTime != carIdMapByDepartTime.end()){
                double time = simTime().dbl();
                string id = *itCarIdMapByDepartTime;
                addOneVehicle(carMapByID[id]);
                itCarIdMapByDepartTime++;
                if(itCarIdMapByDepartTime != carIdMapByDepartTime.end()){
                    time = carMapByID[*itCarIdMapByDepartTime].time;
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
            // 若道路第一次出现，先车讯道路车道数，并开始依次用来生成部署车辆
            if(curLastUsedLane.find(car.origin) == curLastUsedLane.end()){
                roadLaneNumber[car.origin] = getMapSystem()->getLaneNumber(car.origin);
                curLastUsedLane[car.origin] = 0;
            }
//            // 降序循环使用车道
//            curLastUsedLane[car.origin] = roadLaneNumber[car.origin] - 1;
//            if(curLastUsedLane[car.origin]<0){
//                curLastUsedLane[car.origin] = roadLaneNumber[car.origin]-1;
//            }
            if(!getMapSystem()->addOneVehicle(car.id, car.vtype, car.origin, car.time, 0, car.maxSpeed,
                    roadLaneNumber[car.origin] - 1)){
//                  curLastUsedLane[car.origin])){
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
        string head = getStartPoint(road);
        string tail = getEndPoint(road);
        if(head == start){
            if(tail != end){
                vecOutStartPoint.push_back(road);
                vecOutPoint.push_back(road);
                // the reverse road is the in point
                vecInStartPoint.push_back(tail + "to" + head);
                vecInPoint.push_back(tail + "to" + head);
            }
        }
    }
    for(list<string>::iterator it = roadList.begin(); it != roadList.end(); it++){
        road = *it;
        // end to start and not start from end
        string head = getStartPoint(road);
        string tail = getEndPoint(road);
        if(head == end){
            if(tail != start){
                vecOutEndPoint.push_back(road);
                vecOutPoint.push_back(road);
                // the reverse road is the in point
                vecInEndPoint.push_back(tail + "to" + head);
                vecInPoint.push_back(tail + "to" + head);
            }
        }
    }
    // after this
    cout << vecInStartPoint.size() << endl;
    // vecInStartPoint      1,2,3->start
    cout << vecOutStartPoint.size() << endl;
    // vecOutStartPoint     start->1,2,3
    cout << vecInEndPoint.size() << endl;
    // vecInEndPoint        1,2,3->end
    cout << vecOutEndPoint.size() << endl;
    // vecOutEndPoint       end->1,2,3
    cout << vecInPoint.size() << endl;
    // vecInPoint           1,2,3->start,1,2,3->end
    cout << vecOutPoint.size() << endl;
    // vecOutPoint          start->1,2,3,end->1,2,3
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
        // from start to any
        for(unsigned int j = 0; j < vecInStartPoint.size(); j++){
            if(dblrand()
                    < SinFuncFixed(time - carSpawnStartTime, carSpawnPeriod, maxSpawnProbabilityForOneJudge,
                            minSpawnProbabilityForOneJudge)){
                string carid = prefix + Fanjing::StringHelper::int2str(carNum++);
                string origin = vecInStartPoint[j];
                int reverseRoad = j;
                int rnd = intrand(vecOutPoint.size() - 1);
                // remove the possible to choose the reverse road of the origin.
                if(rnd == reverseRoad){
                    rnd = vecOutPoint.size() - 1;
                }
                string destination = vecOutPoint[rnd];
                string vType = vecVType[intrand(vecVType.size())];
                carFlowHelper.addODCar(prefix + Fanjing::StringHelper::int2str(carNum), origin, destination, time,
                        vType);
                if(carNumLimit > 0 && carNum > carNumLimit){
                    i = maxJudgeTimes;
                }
            }
        }
        // 后半周期主要车流反向
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
                string origin = vecInEndPoint[j];
                int reverseRoad = j + vecOutStartPoint.size();
                int rnd = intrand(vecOutPoint.size() - 1);
                // remove the possible to choose the reverse road of the origin.
                if(rnd == reverseRoad){
                    rnd = vecOutPoint.size() - 1;
                }
                string destination = vecOutPoint[rnd];
                string vType = vecVType[intrand(vecVType.size())];
                carFlowHelper.addODCar(prefix + Fanjing::StringHelper::int2str(carNum), origin, destination, time,
                        vType);
                if(carNumLimit > 0 && carNum > carNumLimit){
                    i = maxJudgeTimes;
                }
            }
        }
        // 自由车流量
        maxSpawnProbabilityForOneJudge = maxFreeCarFlowRate * carSpawnJudgeInterval;
        minSpawnProbabilityForOneJudge = minFreeCarFlowRate * carSpawnJudgeInterval;
        // free flow from anyIn to anyOut
        for(unsigned int j = 0; j < vecInPoint.size(); j++){
            if(dblrand()
                    < SinFuncFixed(time - carSpawnStartTime, carSpawnPeriod, maxSpawnProbabilityForOneJudge,
                            minSpawnProbabilityForOneJudge)){
                string carid = prefix + Fanjing::StringHelper::int2str(carNum++);
                string origin = vecInPoint[j];
                int reverse = j;
                int rnd = intrand(vecOutPoint.size() - 1);
                // remove the possible to choose the reverse road of the origin.
                if(rnd == reverse){
                    rnd = vecOutPoint.size() - 1;
                }
                string destination = vecOutPoint[rnd];
                string vType = vecVType[intrand(vecVType.size())];
                carFlowHelper.addODCar(prefix + Fanjing::StringHelper::int2str(carNum), origin, destination, time,
                        vType);
                if(carNumLimit > 0 && carNum > carNumLimit){
                    i = maxJudgeTimes;
                }
            }
        }
        // out put the process infomation
        if((i & 1023) == 1023){
            cout << "process: " << "time:" << time << ", t: "
                    << (int) 100 * (time - carSpawnStartTime) / carSpawnTimeLimit << "%";
            if(carNumLimit > 0){
                cout << " car: " << 100 * carNum / carNumLimit << "%";
            }
            cout << endl;
        }
        time += carSpawnJudgeInterval;
    }
    cout << "process: " << "time:" << time << ", t: " << (int) 100 * (time - carSpawnStartTime) / carSpawnTimeLimit
            << "%";
    if(carNumLimit > 0){
        cout << " car: " << 100 * carNum / carNumLimit << "%";
    }
    cout << endl;
    cout << "carSpawnTimeLimit: " << carSpawnTimeLimit << ", carNumLimit: " << carNumLimit << endl;
    cout << " car: " << carNum << endl;
    carFlowHelper.save();
}

string GlobalVehicleManager::getStartPoint(string road) {
    string result = "";
    string to = "to";
    if(road.find(to) != string::npos){
        result = road.substr(0, road.find(to));
    }
    return result;
}

string GlobalVehicleManager::getEndPoint(string road) {
    string result = "";
    string to = "to";
    if(road.find(to) != string::npos){
        if(road.find("_") != string::npos){
            result = road.substr(road.find(to) + to.length(), road.find("_") - road.find(to));
        }else{
            result = road.substr(road.find(to) + to.length());
        }
    }
    return result;
}

SMTCarInfo GlobalVehicleManager::getCarInfo(string id) {
    return carMapByID[id];
}

bool GlobalVehicleManager::compare_departTime(string& first, string& second) {
    return carMapByID[first].time < carMapByID[second].time;
}

double GlobalVehicleManager::SinFuncFixed(double t, double period, double up, double down) {
    if(disableSinFix){
        return up;
    }
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
        carIdMapByDepartTime.push_back(car.id);
        car = carFlowHelper.getNextCar();
    }
    // TODO 修复下面添加突发车辆的部分

    if(enableBurst && nBurstNum > 0){
        // 设置路径备选集
        // generate vtype vector
        list<string> listVType = SMTCarInfo::getDefaultVeicleTypeList();
        vector<string> vecVType = vector<string>(listVType.begin(), listVType.end());
        vector<string> vecInStartPoint;
        vector<string> vecInEndPoint;
        vector<string> vecOutEndPoint;
        vector<string> vecOutStartPoint;
        list<string> roadList = getMapSystem()->getAllEdges();
        string road = "";
        string start = "2/4";
        string end = "2/2";
        string AtoB = "2/4to2/2";
        for(list<string>::iterator it = roadList.begin(); it != roadList.end(); it++){
            road = *it;
            // end to start and not start from end
            string head = getStartPoint(road);
            string tail = getEndPoint(road);
            if(head == end){
                if(tail != start){
                    vecOutEndPoint.push_back(road);
                }
            }
        }
        // 初始化突发车辆信息
        double burstStart = carSpawnStartTime + carSpawnPeriod;
        int carNum = 1;
        for(unsigned int j = 0; j < nBurstNum; j++){
            string carid = "burst" + Fanjing::StringHelper::int2str(carNum++);
            string origin = "0/2to2/2";
            int rnd = intrand(vecOutEndPoint.size());
            string destination = vecOutEndPoint[rnd];
            string vType = vecVType[intrand(vecVType.size())];
            SMTCarInfo car;
            car.id = carid;

            car.vtype = vType;
            car.time = burstStart + j * 2;
            car.type = SMTCarInfo::SMTCARINFO_ROUTETYPE_OD;
            car.origin = origin;
            car.destination = destination;
            carMapByID[car.id] = car;
            carIdMapByDepartTime.push_back(car.id);
        }
    }
    carIdMapByDepartTime.sort(compare_departTime);
    itCarIdMapByDepartTime = carIdMapByDepartTime.begin();
}

string GlobalVehicleManager::getRouXMLFromLaunchConfig(string launchFilePath) {
    if(launchFilePath == ""){
        return GlobalMobilityLaunchdAccess().get()->getRouXML();
    }else{
        return GlobalMobilityLaunchd::getRouXMLFromLaunchConfig(launchFilePath);
    }
}

