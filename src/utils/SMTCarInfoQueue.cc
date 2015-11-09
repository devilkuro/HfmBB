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

#include <SMTCarInfoQueue.h>
#include <cmath>
#include <iostream>

namespace Fanjing {
bool SMTCarInfoQueue::overtakeAllowed = false;
double SMTCarInfoQueue::updateInterval = 0.1;
XMLDocument* SMTCarInfoQueue::doc = NULL;
bool SMTCarInfoQueue::XMLHasLoaded = false;
bool SMTCarInfoQueue::onlyLosseOneCar = true;   // 是否每次仅松散一个车辆

SMTCarInfoQueue::TraversalHelper::TraversalHelper() {
    carListMap = 0;
}

SMTCarInfoQueue::TraversalHelper::~TraversalHelper() {
}

string SMTCarInfoQueue::TraversalHelper::getFirstCarId(const map<double, list<string> > &carListMapByCertainTime,
        double time) {
    // 获取指定时间点之后的第一个车辆id(包含当前时间点)
    carListMap = &carListMapByCertainTime;
    // get first time node (include given time)
    it = carListMapByCertainTime.lower_bound(time);
    if(it != carListMapByCertainTime.end()){
        lit = it->second.begin();
        if(lit != it->second.begin()){
            return *lit;
        }
    }
    return "";
}

string SMTCarInfoQueue::TraversalHelper::getNextCarId() {
    if(lit != it->second.end()){
        // if the list has more objects, get next
        lit++;
        return *lit;
    }else if(it != carListMap->end()){
        // if the list has no object any more get later time list and return the first car
        it++;
        if(it != carListMap->end()){
            lit = it->second.begin();
            if(lit != it->second.end()){
                return *lit;
            }
        }
    }
    // if the list has no more object
    return "";
}

string SMTCarInfoQueue::TraversalHelper::getPreviousCarId() {
    if(lit != it->second.begin()){
        // if the list has more objects before, get previous
        lit--;
        return *lit;
    }else if(it != carListMap->begin()){
        // if the list has no object any more , get cars in previous time list and return the last car
        it--;
        lit = it->second.end();
        if(lit != it->second.begin()){
            lit--;
            return *lit;
        }
    }
    // if the list has no more object
    return "";
}

SMTCarInfoQueue::SMTCarInfoQueue() {
    init();
}

SMTCarInfoQueue::SMTCarInfoQueue(string lane, string xmlpath, double length, double outLength) {
    init();
    laneName = lane;
    laneLength = length;
    laneOutLength = outLength;
    if(!XMLHasLoaded){
        // load xml file only if has not
        doc->LoadFile(xmlpath.c_str());
    }
    root = doc->FirstChildElement("results");
    if(root == NULL){
        root = doc->NewElement("results");
        doc->LinkEndChild(root);
    }
    root = root->FirstChildElement("set");
    while(root != NULL){
        if(root->Attribute("laneName", lane.c_str()) != NULL){
            break;
        }
        root = root->NextSiblingElement("set");
    }
    if(root == NULL){
        root = doc->NewElement("set");
        root->SetAttribute("laneName", lane.c_str());
        doc->LinkEndChild(root);
    }
}

void SMTCarInfoQueue::saveResults(string filename) {
    doc->SaveFile(filename.c_str());
    releaseXML();
}

void SMTCarInfoQueue::releaseXML() {
    if(doc != NULL){
        doc->Clear();
        delete doc;
        doc = NULL;
    }
}

void SMTCarInfoQueue::updateCarOutInfo(string id, string preId) {
    // todo 更新车辆离开相关信息
    // 1. 该功能需要完成的操作
    //      a. 修改outQueueTimeMap
    //      b. 修改outTimeMap和对应的反向Map
    // 2. 该功能的具体过程为
    //      a. 判定车辆启动并离开队列区的时间
    //          该时间由当前车辆到达队列区时间和前方车辆开始驶离队列区时间共同影响决定
    //          若车辆离开队列区时未受前方车辆影响，则
    //          该车将由进入道路后，保持最高速度，直接驶离道路
    //      b. 若车辆启动离开过程需要在队列区进行加速启动，则
    //          1. 判定驶离队列区时当前车辆在队列区的位置
    //              考虑启动的问题，该位置由当前车辆开始驶离时间所在通行周期内通过道路的车辆长度累加确定
    //          2. 通过所在队列区的位置进行加速启动，并离开道路
    //      c. 判定车辆离开的时间（不考虑交通信号）
    //          通过getTheReachTime确定
    //          c+. 考虑是否受前方车辆影响，其离开时间不应该早于前方车辆离开路口后updateInterval时间
    //      d. 进行离开时间修正
    //          通过getFixedOutTime确定
    //          d+. 若受交通信号影响，则需要修改离开队列区时间
    //      e. 设置outQueueTimeMap和outTimeMap
    double outTime = 0;
    bool isObstructed = false;
    if(preId == ""){
        // 处理前方没有车辆的情况
        // a. 判定启动离开队列区的时间
        // 在没有前方车辆的情况下，启动离开时间等于进入队列区时间
        outQueueTimeMapById[id] = queueTimeMapById[id];
        //  c. 判定车辆离开的时间（不考虑交通信号）
        // 在没有前方车辆的情况下，则无需考虑加速过程和前方车辆的影响，直接全速通过道路
        outTime = getTheReachTime(carMapById[id], laneLength, enterTimeMapById[id], false, false);
    }else{
        // a. 判定启动离开队列区的时间
        if(queueTimeMapById[id] >= outQueueTimeMapById[preId] + updateInterval){
            // 若当前车辆进入队列区时，前方车辆已经启动，则认为前方车辆不会阻碍当前车辆
            // 此时，当前开始驶离队列区时间等于其进入队列区的时间
            // 注意：实际上在此条件下有小概率会影响，即后方车辆全速，前方车辆在大于updateInterval时间前开始加速
            // 此时存在前方车辆未完全加速时影响后方车辆，使后方车辆减速的可能性，但概率较低，予以忽略
            outQueueTimeMapById[id] = queueTimeMapById[id];
            // c. 判定车辆离开的时间（不考虑交通信号）
            // 由于未被阻碍，则无需考虑加速过程，直接全速通过道路，但仍需在后面判定是否受前方车辆影响
            outTime = getTheReachTime(carMapById[id], laneLength, enterTimeMapById[id], false, false);
        }else{
            // 若当前车辆驶离队列区时受前方车辆影响
            // 此时当前车辆驶离队列区时间等于前方车辆开始驶离队列区时间+updateInterval
            outQueueTimeMapById[id] = outQueueTimeMapById[preId] + updateInterval;
            // c. 判定车辆离开的时间（不考虑交通信号）
            // 由于被阻碍，则需要在队列区末尾重新加速，因此需要计算队列区长度
            // b. 判定驶离队列区时的队列长度。
            // 计算启动时的队列长度
            // 首先计算启动时间所在的通行周期的起始时间
            double startCircleTime = getStartTimeOfAllowedTime(outQueueTimeMapById[id]);
            // 对当前车辆前方队列中的车辆长度进行累加,求得队列长度
            double queueLength = 0;
            // 按照驶离路口时间遍历并累加队列区车辆
            TraversalHelper outTimeHelper;
            TraversalHelper queueTimeHelper;
            // 获取离开路口时间在的当前通行周期的时间内的第一个车辆
            // 注意：在该时间点之后可能没有车辆离开道路，或者取得的车辆在队列区的位置可能不在当前车辆前方,需要进行判定
            string headCar = outTimeHelper.getFirstCarId(carMapByOutTime, startCircleTime);
            if(headCar != ""){
                // 若对应时间点之后存在其他车辆，则对当前队列区队列中在当前车辆的前方的车辆计算队列长度
                // 将对应的车辆由离开路口队列映射到进入队列区时间的队列
                string queueCar = queueTimeHelper.getFirstCarId(carMapByQueueTime, queueTimeMapById[headCar]);
                // 遍历至对应的车辆（在正确操作之后，队列区所有车辆进入队列区时间不会重合，即以下步骤在正确操作时并不会执行）
                while(queueCar != "" && queueCar != headCar){
                    queueCar = queueTimeHelper.getNextCarId();
                }
                while(queueCar != "" && queueTimeMapById[headCar] < queueTimeMapById[id]){
                    // 若在通行周期开始后通过路口的车辆中，存在在当前队列区队列中在当前车辆的前方的车辆，则将其加入队列区长度中
                    queueLength += carMapById[queueCar].minGap + carMapById[queueCar].length;
                    queueCar = queueTimeHelper.getNextCarId();
                }
            }else{
                // 若在此时间后不存在车辆
                // （即当前车辆是此时间后唯一的车辆，则该队列中没有其他车辆，以当前车辆的最小间距作为队列长度）
            }
            // 在前方车辆的基础上加入当前车辆的前方最小间距
            queueLength += carMapById[id].minGap;
            // 由于被阻碍，则需要在队列区末尾重新加速
            outTime = getTheReachTime(carMapById[id], queueLength, outQueueTimeMapById[id], true, false);
        }
        // c. 判定车辆离开的时间
        // （前面已经判断preId!=""，所以此时前方存在其他车辆）
        // c+. 考虑是否受前方车辆影响，其离开时间不应该早于前方车辆离开路口后updateInterval时间
        if(outTime < outTimeMapById[preId] + updateInterval){
            outTime = outTimeMapById[preId] + updateInterval;
        }
    }
    // d. 离开时间的交通信号修正
    // 判定是否受到交通控制信号影响
    double fixedOutTime = getFixedOutTime(outTime);
    if(fixedOutTime > outTime){
        // 若受到交通控制信号影响，则需要修改驶离队列时间为下一个红绿灯允许的时间，并计算新的离开路口时间
        outQueueTimeMapById[id] = fixedOutTime + updateInterval;
        fixedOutTime = getTheReachTime(carMapById[id], carMapById[id].minGap, fixedOutTime, true, false);
    }
    // 判定完成后修改离开路口时间
    setOutTimeOfCar(id, fixedOutTime);
}

void SMTCarInfoQueue::updateCarQueueInfoAt(string id, string preId) {
    // 说明:
    // 1. 更新操作需要完成以下操作
    //      a. 更新当前节点进入队列区的时间
    //          a+. 更新当前节点的驶离信息(因为当前节点的状态与后方车辆无关,可以在此时确定)
    //      b. 判定后方跟随车辆
    //      c. 更新后方跟随车辆
    // 2. 各步骤的大致内容
    //      a: 修改当前节点进入队列时间,该时间应晚于队列区前方车辆至少updateInterval时间距离
    //      b: 判定后方跟随车辆
    //              允许超车时
    //                  a  后方车辆必须位于当前车辆此时队列区时间后方
    //                      因为前方的车辆已经被判定不受影响
    //                  b. 在当前车辆后方的车辆中，设最早进入道路车辆为P,P抵达队列区的时间为t
    //                      则只有早于t进入道路的车辆正才有可能在超车之后成为当前车辆后方的车辆(仅用于缩小判定范围,并不准确)
    //                  c. 在a条件确定的范围中，以此由进入时间小到大的顺序进行判定
    //                      id[1,2,3,...,n],类似冒泡排序,若1后方m可以超越1，则m为最前方车辆，并继续判定m+1，直到n。
    //              不允许超车时
    //                  后方进入的车辆即为后方跟随车辆
    //      c: 跟随在当前车辆后方的车辆需要满足一下要求才需要继续更新
    //              a. 当前车辆离开道路时后方跟随车辆必须已经进入当前道路
    //              b. 并且，当前车辆离开道路时，后方跟随车辆必须必须能够在接下来的updateInterval时间内能够抵达道路末尾
    //                  若无法抵达道路末尾，则表示后方跟随车辆通过道路的整个行程并未受到当前车辆的阻碍
    //              c. 判断完成,对后方跟随的车辆进行更新

    //1. 更新当前节点进入队列区的时间
    //      a. 查找前方车辆进入队列的时间
    //      b. 若前方车辆进入队列时间与当前车辆进入队列时间差值小于updateInterval,则推迟当前车辆进入队列时间
    //          b+. 推迟过程中,若有其他车辆存在于该updateInterval时间片内,则依次向后推移
    // todo 需要在此处更新当前车辆进入队列区的时间
    // todo
    //  a+. 更新当前节点的驶离信息(因为当前节点的状态与后方车辆无关,可以在此时确定)
    updateCarOutInfo(id, preId);
    // todo 需要重写过程
    // FIXME 后面都是旧代码，没有用了几乎
    if(overtakeAllowed){
        // 0th. config this function
        double startTime = queueTimeMapById[id];
        double timeOffset = 0;
        list<string> cacheQueueList;
        // 1st. seek to id
        bool isFinished = false;
        map<double, list<string> >::iterator itQTMap = carMapByQueueTime.lower_bound(queueTimeMapById[id]);
        list<string>::iterator itQTList = itQTMap->second.begin();
        while(itQTList != itQTMap->second.end()){
            if(*itQTList != id){
                itQTList++;
            }else{
                break;
            }
        }
        if(itQTList == itQTMap->second.end()){
            cout << "Error@updateCarQueueInfoAt()::ID_MISSING" << endl;
        }
        // 2nd. stack the compacted cars into cache list
        if(onlyLosseOneCar){
            // do nothing
        }else{
            while(!isFinished && itQTMap != carMapByQueueTime.end()){
                if(itQTMap->first != startTime){
                    itQTList = itQTMap->second.begin();
                }
                while(!isFinished && itQTList != itQTMap->second.end()){
                    if(itQTMap->first < startTime + timeOffset){
                        // modify the time offset
                        timeOffset += updateInterval;
                        cacheQueueList.push_back(*itQTList);
                    }else{
                        isFinished = true;
                    }
                    itQTList++;
                }
                itQTMap++;
            }
        }
        // 3rd. insert compacted cars back into the time map.
        // todo 松弛过于密集的车辆
        // 4th. update the next car if necessary
    }else{
        // 如果不允许超车行为，则修改当前车辆进入队列区时间，更新离开状态，然后更新下一个车辆信息
        // 1.st 若当前车辆进入队列的预测时间
        // todo  不允许超车时队列区时间的更新
    }
}

void SMTCarInfoQueue::setThePairMapAtBackOfCar(map<double, list<string> >& carListMapByTime,
        map<string, double>& timeMapByCar, string id, string otherId) {
    // 将id为id车辆插入到id为otherId的车辆的的后方
    // 0th. config function
    double time = -1;
    // 1st. record the time of other id
    if(timeMapByCar.find(otherId) == timeMapByCar.end()){
        cout << "Error@setThePairMapAtBackOfCar()::OTHER_ID_MISSING" << endl;
        return;
    }else{
        time = timeMapByCar[otherId];
    }
    // 1st+. seek to the other id
    map<double, list<string> >::iterator itcarMapByTime = carListMapByTime.lower_bound(time);
    list<string>::iterator litcarMapByTime = itcarMapByTime->second.begin();
    while(litcarMapByTime != itcarMapByTime->second.end()){
        if(*litcarMapByTime == otherId){
            break;
        }
        litcarMapByTime++;
    }
    // 2nd. remove id from current time map
    if(timeMapByCar.find(id) != timeMapByCar.end()){
        // if the car is already here, update the related information
        // 1st. remove the old information
        carListMapByTime[timeMapByCar[id]].remove(id);
    }
    // 3rd. insert the id before other id and modify the related time in the id map
    // add this car at back of other id
    if(litcarMapByTime == itcarMapByTime->second.end()){
        // 判定是否存在other id，理论上肯定存在，若进入此代码，则那里出了问题
        cout << "Error@setThePairMapAtBackOfCar()::OTHER_ID_MISSING::2" << endl;
        return;
    }
    litcarMapByTime++;
    carListMapByTime[time].insert(litcarMapByTime, id);
    // modify the related time
    timeMapByCar[id] = time;
}

bool SMTCarInfoQueue::isCarACanOvertakeCarB(SMTCarInfo carA, SMTCarInfo carB, double enterTimeA, double enterTimeB,
        double freeSpace) {
    if(overtakeAllowed){
        // 仅当允许超车时,进行超车判定
        if(enterTimeA >= enterTimeB){
            // A晚于B进入道路
            if(freeSpace - carA.length - carB.minGap < 0){
                // 若空间不够，则A无法超越B
                return false;
            }
            // caculate the time of the current car reach the queue area if A overtake B successfully
            double reachTimeForCarA = getTheReachTime(carA, freeSpace, enterTimeA, false, true);
            double reachTimeForCarB = getTheReachTime(carB, freeSpace - carA.minGap - carA.length - carB.minGap,
                    enterTimeB, false, true);
            // decide A overtake B or not
            bool beOvertake = reachTimeForCarA < reachTimeForCarB;
            return beOvertake;
        }else{
            // A先于B进入道路
            if(freeSpace - carA.length - carB.minGap < 0){
                // 若空间不够，则A在B的前方
                return true;
            }
            // caculate the time of the current car reach the queue area if B could overtake A successfully
            double reachTimeForCarA = getTheReachTime(carA, freeSpace - carB.minGap - carB.length - carA.minGap,
                    enterTimeA, false, true);
            double reachTimeForCarB = getTheReachTime(carB, freeSpace, enterTimeB, false, true);
            // decide B overtake A or not
            bool beOvertake = reachTimeForCarA > reachTimeForCarB;
            return !beOvertake;
        }
    }else{
        // 在不允许超车的情况下,如果A的进入时间早于B,则A在B的前方
        // 在A和B同时进入的情况下,默认A无法超越B
        return enterTimeA < enterTimeB;
    }
}

double SMTCarInfoQueue::getFixedTimeWithUpdateInterval(double time) {
    return ((int) (time / updateInterval)) * updateInterval;
}

void SMTCarInfoQueue::init() {
    laneLength = 0;
    allowedInterval = 0;
    cyclePeriod = 0;
    cycleOffset = 0;
    if(doc == NULL){
        doc = new XMLDocument();
    }
    root = NULL;
    element = NULL;
}

SMTCarInfoQueue::~SMTCarInfoQueue() {
    // TODO Auto-generated destructor stub
}

double SMTCarInfoQueue::insertCar(SMTCarInfo car, double time, double neighborFrozenSpace) {
    // 说明:
    // 1. 插入操作需要完成以下操作:
    //      a.    // 插入操作的具体过程
    // todo 整个过程需要重新规划编写
    TraversalHelper enterTimeHelper;
    TraversalHelper queueTimeHelper;
    TraversalHelper outTimeHelper;
    // 1st. set the time entering this lane
    carMapById[car.id] = car;
    setEnterTimeOfCar(car.id, time);
    // 1st. judge the length of queue and the time entering the queue
    double queueLength = 0;
    string otherId = outTimeHelper.getFirstCarId(carMapByOutTime, time);
    while(otherId != ""){
        // while there are some still in this lane
        // and if those car have already enter the queue area when this car entered this lane
        // add its gap and length into the queue length
        if(queueTimeMapById[otherId] <= time){
            SMTCarInfo otherCar = getCarInfoById(otherId);
            queueLength += otherCar.minGap + otherCar.length;
            otherId = outTimeHelper.getNextCarId();
        }
    }
    // 2nd. judge the length of queue considering the cars have not entered the queue yet
    otherId = queueTimeHelper.getFirstCarId(carMapByQueueTime, time);
    double preCarQueueTime;
    string preQueueCarId;   // the car before current after current enter queue area.
    // there still have some cars not entered the queue
    if(overtakeAllowed){
        while(otherId != ""){
            // if the car has not reach queue area yet
            SMTCarInfo otherCar = getCarInfoById(otherId);
            // 2nd(optional). overtake judgement process
            // process only if overtake process is enabled
            // if overtake is enable, the car befor this car may not increase its queue length
            // otherwise, all cars before this car will be added into the queue length.

            // calculate the problem of distance
            if(enterTimeMapById[otherId] <= enterTimeMapById[car.id]){
                // if the other car enter this lane first
                // assume current car can overtake other car
                // then when the current car reach the queue area
                // the other car should not reach the length + gap of both cars
                // caculate the distance from the start to the queue area if overtake successfully
                // this distance equals to the overtake allowed distance
                double overtakeLengtheForCurrentCar =
                        laneLength - queueLength > neighborFrozenSpace ? queueLength : neighborFrozenSpace;
                // before the queue area
                double overtakeLengthForOtherCar = overtakeLengtheForCurrentCar - car.minGap - car.length;
                // caculate the time of the current car reach the queue area if overtake successfully
                double reachTimeForCurrentCar = getTheReachTime(car, overtakeLengtheForCurrentCar, time, false, true);
                double reachTimeForOtherCar = getTheReachTime(otherCar, overtakeLengthForOtherCar,
                        enterTimeMapById[otherId], false, true);
                // decide overtake or not
                bool beOvertake = reachTimeForCurrentCar < reachTimeForOtherCar;
                // compare two time and decide overtake or not
                // and if overtake, the car enter queue area later needs to be updated
                if(beOvertake){
                    // if overtake happens, the time that current car enter the queue become seated.
                    // since the rest existing car enter queue later than current car.
                    // and if overtake, the car enter queue area later needs to be updated after caculated current car
                    // set the queue time
                    reachTimeForCurrentCar = getTheReachTime(car, laneLength - queueLength, time, false, true);
                    // make sure the current queue time not earlier than the previous one.
                    if(reachTimeForCurrentCar > preCarQueueTime){
                        setQueueTimeOfCar(car.id, reachTimeForCurrentCar);
                    }else{
                        setQueueTimeOfCar(car.id, preCarQueueTime);
                    }
                    // set the other car's queue time after current car
                    // set the same time after current will make other car in later position than current car
                    // and even more, the new queue time for other will become earlier than its old one
                    // and the queue time of current will be more earlier than the other's old one
                    // so this operation will not break the original order of the queue time
                    reachTimeForOtherCar = getTheReachTime(otherCar, laneLength - queueLength - car.minGap - car.length,
                            enterTimeMapById[otherId], false, true);
                    // make sure the current queue time not earlier than the previous one.
                    if(reachTimeForOtherCar > preCarQueueTime){
                        setQueueTimeOfCar(car.id, reachTimeForOtherCar);
                    }else{
                        setQueueTimeOfCar(car.id, preCarQueueTime);
                    }
                    break;
                }else{
                    // if current car reach later, other car get to the queue before current car
                    // and then judge next car reach the queue area
                    queueLength += otherCar.minGap + otherCar.length;
                    preCarQueueTime = queueTimeMapById[otherId];
                    otherId = queueTimeHelper.getNextCarId();
                    if(otherId == ""){
                        // fixme !! resort the time map, important... the overtak process can not work proper
                        // there no other cars, the current car's queue time is seated.
                        reachTimeForCurrentCar = getTheReachTime(car, laneLength - queueLength, time, false, true);
                        // make sure the current queue time not earlier than the previous one.
                        if(reachTimeForCurrentCar > preCarQueueTime){
                            setQueueTimeOfCar(car.id, reachTimeForCurrentCar);
                        }else{
                            setQueueTimeOfCar(car.id, preCarQueueTime);
                        }
                    }
                }
            }else{
                // if the other car enter this lane later
                // assume other car can overtake current car
                // then the same as the above
                double overtakeLengthForOtherCar =
                        laneLength - queueLength > neighborFrozenSpace ? queueLength : neighborFrozenSpace;
                double overtakeLengtheForCurrentCar = overtakeLengthForOtherCar - car.minGap - car.length;
                // caculate the time of the current car reach the queue area if overtake successfully
                double reachTimeForCurrentCar = getTheReachTime(car, overtakeLengtheForCurrentCar, time, false, true);
                double reachTimeForOtherCar = getTheReachTime(otherCar, overtakeLengthForOtherCar,
                        enterTimeMapById[otherId], false, true);
                // decide overtake or not
                // if other car reach before then it overtake current car
                bool beOvertake = reachTimeForOtherCar < reachTimeForCurrentCar;
                // compare two time and decide overtake or not
                // and if overtake, nothing change, continue to judge next car
                if(beOvertake){
                    // if current car reach first, the other car get to the queue before current car
                    // in this situation, the queue time of other car do not change
                    // and then judge next car reach the queue area
                    queueLength += otherCar.minGap + otherCar.length;
                    preCarQueueTime = queueTimeMapById[otherId];
                    otherId = queueTimeHelper.getNextCarId();
                    {
                        // fixme !! resort the time map, important... the overtak process can not work proper
                        if(otherId == ""){
                            // there no other cars, the current car's queue time is seated.
                            reachTimeForCurrentCar = getTheReachTime(car, laneLength - queueLength, time, false, true);
                            // make sure the current queue time not earlier than the previous one.
                            if(reachTimeForCurrentCar > preCarQueueTime){
                                setQueueTimeOfCar(car.id, reachTimeForCurrentCar);
                            }else{
                                setQueueTimeOfCar(car.id, preCarQueueTime);
                            }
                        }
                    }
                }else{
                    // if overtake not happens, the time that current car enter the queue become seated.
                    // since the rest existing car enter queue later than current car.
                    // and if not overtake
                    // the car enter queue area later needs to be updated after caculated current car
                    // set the queue time
                    reachTimeForCurrentCar = getTheReachTime(car, laneLength - queueLength, time, false, true);
                    // set the other later cars' queue time after current car, if they were before current one
                    // get car list that needs to be modified
                    // fixme !! resort the time map, important... the overtak process can not work proper
                    list<string> modifiedCarList;
                    // list<string>::it
                    setQueueTimeOfCar(car.id, reachTimeForCurrentCar);
                    while(otherId != ""){
                        if(queueTimeMapById[otherId] <= reachTimeForCurrentCar){
                            setQueueTimeOfCar(otherId, reachTimeForCurrentCar);
                            // fixme not finished
                            //otherId =
                        }else{
                            break;
                        }
                    }
                    break;
                }
            }
        }

    }else{
        // if overtake process is disabled
        // then all cars before this car will be added into the queue length.

        // get the previous car entering this lane
        string preEnterCarId = enterTimeHelper.getFirstCarId(carMapByEnterTime, time);
        string nextToPreEnterCarId = enterTimeHelper.getNextCarId();
        // if the next one of the preEnterCarId is later than current car, find next.
        // actually, this time can only equal or bigger than time, if equal, find next.
        while(enterTimeMapById[nextToPreEnterCarId] <= time){
            // find next
            preEnterCarId = nextToPreEnterCarId;
            nextToPreEnterCarId = enterTimeHelper.getNextCarId();
        }
        while(otherId != nextToPreEnterCarId){
            // if the car has not reach queue area yet
            // when overtake disabled, the last other car is the car enter next to the pre enter.
            SMTCarInfo otherCar = getCarInfoById(otherId);
            // if other car enter this lane before current car, increase queue length
            queueLength += otherCar.minGap + otherCar.length;
            otherId = queueTimeHelper.getNextCarId();
        }
        // cacluate the reach queue time if no other car affecting it
        double reachQueueTimeForCurrentCar = getTheReachTime(car, laneLength - queueLength, time, false, true);
        if(queueTimeMapById[preEnterCarId] + updateInterval >= reachQueueTimeForCurrentCar){
            // current car is obstruct by previous car
            reachQueueTimeForCurrentCar = queueTimeMapById[preEnterCarId] + updateInterval;
        }
        setQueueTimeOfCar(car.id, reachQueueTimeForCurrentCar);
    }
    // 3rd. update the affected cars from this car
    updateCarQueueInfoAt(car.id, otherId);
    // 4th. caculate the time current car start to level the queue.
    // before this setp, the queue time should be updated.
    double startOutQueueTime = queueTimeMapById[car.id];
    if(startOutQueueTime < outQueueTimeMapById[preQueueCarId] + updateInterval){
        startOutQueueTime = outQueueTimeMapById[preQueueCarId] + updateInterval;
    }
    double outTimeWithoutAffected = getTheReachTime(car, queueLength, startOutQueueTime, true, false);
    // calculate the out time affected by previous cars
    if(outTimeWithoutAffected < outTimeMapById[preQueueCarId] + updateInterval){
        outTimeWithoutAffected = outTimeMapById[preQueueCarId] + updateInterval;
    }
    outTimeMapById[car.id] = outTimeWithoutAffected;
    // 5th. return the finial out time
    // fixme needs to be fixed by the triffic lights
    return outTimeMapById[car.id];
}

double SMTCarInfoQueue::getTheReachTime(SMTCarInfo car, double length, double startTime, bool considerAccel,
        bool considerDecel) {
    // if consider both accel phases
    double time = 0;
    if(considerAccel && considerDecel){
        // if accel finished
        // S > v^2/2a(decel) + v^2/2a(accel)
        if(length > car.maxSpeed * car.maxSpeed / (2 * car.decel) + car.maxSpeed * car.maxSpeed / (2 * car.accel)){
            // S = v*(t-v/a(decel)-v/a(accel)) + v^2/2a(decel) + v^2/2a(accel)
            // ==> t = S/v + v/2a(decel)-v/2a(accel)
            time = car.maxSpeed / (2 * car.accel) + car.maxSpeed / (2 * car.decel) + length / car.maxSpeed;
        }else{
            // a(accel)t(accel) = a(decel)t(decel) && S = a(accel)t(accel)^2/2 + a(decel)t(decel)^2/2
            // ==> t = sqrt(2*S*(a(accel)+a(decel))/(a(accel)a(decel)))
            time = sqrt(2 * length * (car.accel + car.decel) / (car.accel * car.decel));
        }
    }else if(considerAccel){
        // if only consider accel phase
        if(length > car.maxSpeed * car.maxSpeed / (2 * car.accel)){
            // if accel finished
            // S = vt - v^2/2a ==> t = (S+v^2/2a)/v = S/v + v/2a
            time = length / car.maxSpeed + car.maxSpeed / (2 * car.accel);
        }else{
            // S = at^2/2 ==> t = sqrt(2S/a)
            time = sqrt(2 * length / car.accel);
        }
    }else if(considerDecel){
        // if only consider decel phase
        if(length > car.maxSpeed * car.maxSpeed / (2 * car.decel)){
            // if accel finished
            // S = vt - v^2/2a ==> t = (S+v^2/2a)/v = S/v + v/2a
            time = length / car.maxSpeed + car.maxSpeed / (2 * car.decel);
        }else{
            // S = at^2/2 ==> t = sqrt(2S/a)
            time = sqrt(2 * length / car.decel);
        }
    }else{
        // consider neither accel or decel phase
        time = length / car.maxSpeed;
    }
    return time + startTime;
}

SMTCarInfo SMTCarInfoQueue::getCarInfoById(string id) {
    return carMapById[id];
}

void SMTCarInfoQueue::setCurrentTime(double time) {
    // not necessary at now...
}

double SMTCarInfoQueue::releaseCar(string id, double time) {
    // make recording
    element = doc->NewElement("result");
    element->SetAttribute("car", id.c_str());
    element->SetAttribute("enterTime", enterTimeMapById[id]);
    element->SetAttribute("queueTime", queueTimeMapById[id]);
    element->SetAttribute("startOutQueueTime", outQueueTimeMapById[id]);
    element->SetAttribute("outTime", outTimeMapById[id]);
    element->SetAttribute("actualOutTime", time);
    root->LinkEndChild(element);
    // release the old car and return the predicted out time
    double outTime = outTimeMapById[id];
    removeCar(id);
    return outTime;
}

void SMTCarInfoQueue::setCycleInfo(double period, double allowTime, double offset) {
    cyclePeriod = period;
    allowedInterval = allowTime;
    cycleOffset = offset;
}

void SMTCarInfoQueue::removeCar(string id) {
    carMapById.erase(id);
    carMapByEnterTime[enterTimeMapById[id]].remove(id);
    carMapByQueueTime[queueTimeMapById[id]].remove(id);
    carMapByOutTime[outTimeMapById[id]].remove(id);
    enterTimeMapById.erase(id);
    queueTimeMapById.erase(id);
    outTimeMapById.erase(id);
    outQueueTimeMapById.erase(id);
    // fixme needs update later car or not?
}
void SMTCarInfoQueue::setEnterTimeOfCar(string id, double time) {
// set the enter time of a car and update both carMapByEnterTime and enterTimeMapById
    setThePairMap(carMapByEnterTime, enterTimeMapById, id, time);
}

void SMTCarInfoQueue::setQueueTimeOfCar(string id, double time) {
    // set the enter time of a car and update both carMapByQueueTime and queueTimeMapById
    setThePairMap(carMapByQueueTime, queueTimeMapById, id, time);
}

void SMTCarInfoQueue::setOutTimeOfCar(string id, double time) {
// set the enter time of a car and update both carMapByOutTime and outTimeMapById
    setThePairMap(carMapByOutTime, outTimeMapById, id, time);
}

void SMTCarInfoQueue::setThePairMap(map<double, list<string> > &carListMapByTime, map<string, double>&timeMapByCar,
        string id, double time) {
    // FIXME (可能）此处为唯一修改timeMapByCar中相关时间的方法，因此将时间修正方法放在此处
    // 修正时间
    time = getFixedTimeWithUpdateInterval(time);
    // set the pair map
    if(timeMapByCar.find(id) == timeMapByCar.end()){
        // insert new car
        timeMapByCar[id] = time;
        // insert this car to time
        carListMapByTime[time].push_back(id);
    }else{
        // if the car is already here, update the related information
        // 1st. remove the old information
        carListMapByTime[timeMapByCar[id]].remove(id);
        // 2nd. add this car at new time
        carListMapByTime[time].push_back(id);
    }
}

void SMTCarInfoQueue::setThePairMapAtFrontOfCar(map<double, list<string> >& carListMapByTime,
        map<string, double>& timeMapByCar, string id, string otherId) {
    // 将id为id的车辆插入到id为otherId的车辆的前方
    // 0th. config function
    double time = -1;
    // 1st. record the time of other id
    if(timeMapByCar.find(otherId) == timeMapByCar.end()){
        cout << "Error@setThePairMapAtFrontOfCar()::OTHER_ID_MISSING" << endl;
        return;
    }else{
        time = timeMapByCar[otherId];
    }
    // 1st+. seek to the other id
    map<double, list<string> >::iterator itcarMapByTime = carListMapByTime.lower_bound(time);
    list<string>::iterator litcarMapByTime = itcarMapByTime->second.begin();
    while(litcarMapByTime != itcarMapByTime->second.end()){
        if(*litcarMapByTime == otherId){
            break;
        }
        litcarMapByTime++;
    }
    // 2nd. remove id from current time map
    if(timeMapByCar.find(id) != timeMapByCar.end()){
        // if the car is already here, update the related information
        // 1st. remove the old information
        carListMapByTime[timeMapByCar[id]].remove(id);
    }
    // 3rd. insert the id before other id and modify the related time in the id map
    if(litcarMapByTime == itcarMapByTime->second.end()){
        // 判定是否存在other id，理论上肯定存在，若进入此代码，则那里出了问题
        cout << "Error@setThePairMapAtFrontOfCar()::OTHER_ID_MISSING::2" << endl;
        return;
    }
    // add this car at front of other id
    carListMapByTime[time].insert(litcarMapByTime, id);
    // modify the related time
    timeMapByCar[id] = time;
}

// if the out time is not allowed to get out, then return the start of next allowed time
double SMTCarInfoQueue::getFixedOutTime(double time) {
    // fixme the final solution is read the xml file, now just use the cycle period
    // 先求得当前时间点对应的周期的允许通行时间的起点
    double preAllowedTime = cycleOffset + cyclePeriod * (int) ((time - cycleOffset) / cyclePeriod);
    // 若当前时间超出了该周期的通行时间，则修正通行时间为下一个通行周期，反之不做变动
    if(time - preAllowedTime > allowedInterval){
        time = preAllowedTime + cyclePeriod;
    }
    return time;
}

double SMTCarInfoQueue::getStartTimeOfAllowedTime(double time) {
    // 获取对应时间的通行允许时间的起点

    // 先求得当前时间点对应的周期的允许通行时间的起点
    double preAllowedTime = cycleOffset + cyclePeriod * (int) ((time - cycleOffset) / cyclePeriod);
    if(time - preAllowedTime > allowedInterval){
        // 若当前时间无法通行，则返回下一个通行周期的起点
        return preAllowedTime + cyclePeriod;
    }else{
        // 若当前时间可以通行，则返回当前时间通行周期的起点
        return preAllowedTime;
    }
}

} /* namespace Fanjing */
