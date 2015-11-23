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
    it = carListMap->lower_bound(time);
    if(it != carListMap->end()){
        lit = it->second.begin();
        if(lit != it->second.end()){
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
    // 更新车辆离开相关信息
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
    //                  a  后方车辆必须位于当前车辆此时队列区时间后方,因为前方的车辆已经被判定不受影响
    //                      即进入队列区时间应不小于当前车辆
    //                  b. 由于当前车辆id的加入，后方车辆用来超车道自由行驶区间只会减少不会增加
    //                      则，对于更新前id后方的第一辆车，若之前其后方的车辆没有超越它，车辆id加入后也无法超越
    //                      因此，若id后方第一辆车发生变化，仅可能是更新前后方第一辆车未超越其前方的车辆
    //                      即，仅需要判定进入时间早于后方第一辆车的车辆即可决定后方紧邻的车辆。
    //                  c. 在a、b条件确定的范围中，以此由进入时间小到大的顺序进行判定
    //                      id[1,2,3,...,n],类似冒泡排序,若1后方m可以超越1，则m为最前方车辆，并继续判定m+1，直到n。
    //              不允许超车时
    //                  后方进入的车辆即为后方跟随车辆
    //      c: 跟随在当前车辆后方的车辆需要满足一下要求才需要继续更新
    //              a. 当前车辆离开道路时后方跟随车辆必须已经进入当前道路
    //              b. 并且，当前车辆离开道路时，后方跟随车辆必须必须能够在接下来的updateInterval时间内能够抵达道路末尾
    //                  若无法抵达道路末尾，则表示后方跟随车辆通过道路的整个行程并未受到当前车辆的阻碍
    //              c. 判断完成,对后方跟随的车辆进行更新
    // a. 更新当前节点进入队列区的时间
    updateCarEnterQueueInfo(id, preId);
    // a+. 更新当前节点的驶离信息(因为当前节点的状态与后方车辆无关,可以在此时确定)
    updateCarOutInfo(id, preId);
    // b. 判定后方跟随车辆
    TraversalHelper queueHelper; // 用于遍历queueTimeMap的迭代器
    // b.a  查找位于前车辆此时队列区时间后方的车辆
    string nextId = queueHelper.getFirstCarId(carMapByQueueTime, queueTimeMapById[id]);
    if(nextId != id){
        // 因为前面updateCarEnterQueueInfo中进行了松弛操作，因此这里理论上不会执行
        cout << "Error@updateCarQueueInfoAt:: the first car should be 'id' HERE" << endl;
    }
    nextId = queueHelper.getNextCarId();
    // 若后方存在车辆，则进行更新判定，反之无需进行更新
    if(nextId != ""){
        // 是否允许超车
        if(overtakeAllowed){
            // 若允许超车，则对后方车辆进行超车判定
            // 查找该车进入道路的时间
            double enterTime = enterTimeMapById[nextId];
            // 获取车辆进入道路时，队列区最前方的车辆
            string queueHeadCar = queueHelper.getFirstCarId(carMapByQueueTime, enterTime);
            // 获得前方队列长度
            double queueLength = getQueueLength(queueHeadCar, id);
            double freespace = laneLength - queueLength;
            // 超车判定(由进入时间的nextId往前)
            TraversalHelper enterHelper;
            string tempPreId = enterHelper.getFirstCarId(carMapByEnterTime, enterTime);
            // 找到nextId(由于TraversalHelper对一个时间点取得的是第一个，因此先要向后遍历找到nextId)
            while(tempPreId != "" && tempPreId != nextId){
                tempPreId = enterHelper.getNextCarId();
            }
            if(tempPreId == ""){
                // 如果carMap和timeMap同步没有问题，那么这里肯定不会执行
                cout << "Error@updateCarQueueInfoAt:: CAR MISSING" << endl;
            }
            while(nextId != "" && enterTimeMapById[nextId] <= enterTime){
                if(!isCarACanOvertakeCarB(nextId, tempPreId, enterTimeMapById[nextId], enterTimeMapById[tempPreId],
                        freespace)){
                    // 若存在无法超越的车辆，则无法被超越的车辆将会成为id后方邻接的车辆
                    nextId = tempPreId;
                    tempPreId = enterHelper.getPreviousCarId();
                }
            }
            // FIXME 需要同步修改队列顺序，用于更新队列信息
        }else{
            // 若不允许超车，则直接更新后方车辆
        }
        updateCarQueueInfoAt(nextId, id);
    }else{
        // 后面没车就不需要进行更新之类的操作了
    }
}

void SMTCarInfoQueue::updateCarEnterQueueInfo(string id, string preId) {
// 说明：
//      用于updateCarQueueInfoAt函数中完成修改车辆进入道路的时间
//  更新当前节点进入队列区的时间
//      a. 查找前方车辆进入队列的时间
//      b. 若前方车辆进入队列时间与当前车辆进入队列时间差值小于updateInterval,则推迟当前车辆进入队列时间
//          b+. 推迟过程中,若有其他车辆存在于该updateInterval时间片内,则依次向后推移

// 获取前方车辆抵达队列区时间
    if(preId != ""){
        // 仅在前方有车的情况下才需要更新进入队列的信息
        double preTime = queueTimeMapById[preId];
        // seek to preId遍历至preId
        TraversalHelper queueTimeHelper;
        string otherId = queueTimeHelper.getFirstCarId(carMapByQueueTime, preTime);
        while(otherId != "" && otherId != preId){
            // 由于前方车辆应该都完成了松弛操作，所以理论上不会进入该循环
            // 因此进入循环后需要打印警告信息
            cout << "Error@updateCarEnterQueueInfo: PRE car is compressed." << endl;
            otherId = queueTimeHelper.getNextCarId();
        }
        // 由preId开始进行松弛操作
        otherId = queueTimeHelper.getNextCarId();
        // 若前方车辆紧跟的不是id，则前面过程存在问题，打印警告信息
        if(otherId != id){
            cout << "Error@updateCarEnterQueueInfo: Current car is not the car after the PRE car" << endl;
            // 尝试找到id为id的车辆
            while(otherId != "" && otherId != id){
                otherId = queueTimeHelper.getNextCarId();
            }
            // 如果没找到。。。一定哪里出了问题
            if(otherId == ""){
                cout << "Error@updateCarEnterQueueInfo: ID IS MISSING" << endl;
            }
        }
        if(onlyLosseOneCar){
            // 若每次只更新单个车辆，则对后方车辆进行一次位移
            if(queueTimeMapById[id] < preTime + updateInterval){
                // 推移当前车辆
                pushCarQueueTimeBack(queueTimeHelper, preTime + updateInterval);
            }
        }else{
            // 反之，若需要更新全部，则依次进行位移直到其后方所有车辆间距均大于updateInterval
            while(otherId != "" && queueTimeMapById[otherId] < preTime + updateInterval){
                // 更新第一辆车（第一轮是id，第二轮是id后方，直到otherId为空或者间隔大于updateInterval
                pushCarQueueTimeBack(queueTimeHelper, preTime + updateInterval);
                // 更新otherId和preTime
                otherId = queueTimeHelper.getNextCarId();
                preTime += updateInterval;
            }
        }
    }else{
        // 如果前方没有车，则什么都不用做。
    }
}

void SMTCarInfoQueue::pushCarQueueTimeBack(TraversalHelper &queueTimeHelper, double time) {
// 说明：
//      将对应遍历器对应的车辆推后至某一时刻(请务必保证遍历助手对应的map为queueTimeMap)
//      重设后queueTimeHelper的指针将重新指向已被移动到time时间节点的原车辆
// 步骤：
//      a. 推后车辆
//      b. 重设进入队列的时间
// a. 推后车辆
    list<string> stack = queueTimeHelper.pushCurrentCarBack(time);
// b. 重设进入队列的时间
    for(list<string>::iterator it = stack.begin(); it != stack.end(); it++){
        queueTimeMapById[*it] = time;
    }
}

double SMTCarInfoQueue::getQueueLength(string fromId, string toId) {
// 获取由队列区队列fromId到toId车辆构成的队列的总长度
    TraversalHelper queueHelper;
    string id = queueHelper.getFirstCarId(carMapByQueueTime, queueTimeMapById[fromId]);
    double length = 0;
// fromId和toId必须存在于队列区队列中
    if(queueTimeMapById.find(fromId) != queueTimeMapById.end()){
        while(id != toId){
            length += carMapById[id].minGap + carMapById[id].length;
            id = queueHelper.getNextCarId();
            if(id == ""){
                // toId 不存在
                cout << "Error@getQueueLength:: NO TO CAR NAMED " << toId << endl;
                return 0;
            }
        }
    }else{
        // fromId 不存在
        cout << "Error@getQueueLength:: NO FROM CAR NAMED " << fromId << endl;
    }
    return length;
}

void SMTCarInfoQueue::pushCarQueueTimeBack(string car, double time) {
    TraversalHelper queueHelper;
    // 说明：另一种推迟方法
    // FIXME 需要检查
    // 找到car所在时刻对应的节点
    string tempId = queueHelper.getFirstCarId(carMapByQueueTime, queueTimeMapById[car]);
    // 找到car
    while(tempId != "" && tempId != car){
        tempId = queueHelper.getNextCarId();
    }
    if(tempId != ""){
        // 如果找到了car，则使用queueHelper进行推迟操作，反之应该是哪里错了
        pushCarQueueTimeBack(queueHelper, time);
    }else{
        cout << "Error@pushCarQueueTimeBack:: NO CAR NAMED " << car << endl;
    }
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
    // 啥都不用干
}

double SMTCarInfoQueue::insertCar(SMTCarInfo car, double time, double neighborFrozenSpace) {
    // 说明:
    // 1. 插入操作需要完成以下操作:
    //      0. 将车辆插入进入时间队列
    //      a.  判定进入队列区的顺序和初步预计的时间(即未进行松弛操作的进入队列区时间)
    //      b.  更新队列顺序并写入初步预计进入队列区的时间
    //          b+. 调用updateCarQueueInfoAt更新当前车辆的后续状态时间，并更新其后方的队列
    //      c.  读取上述更新后该车势力路口的时间，并返回该车进入下一跳道路的预测时间(离开路口时间+通过路口时间)
    // 2. 插入操作的具体过程
    //      a. 由前方进入道路的车辆依次开始判定，是否能够进行超越
    //          a.1. 判定队列区起点车辆
    //              +. 判定超车的超车允许长度由前方队列中车辆累计长度和邻接扯到允许超车范围决定
    //              +.  队列长度的起点为进入道路时间早于当前车辆且未在当前时刻前离开道路的第一辆车
    //          a.2. 计算队列长度
    //          a.3. 计算能否完成超车，若能，则继续计算更前方进入的车辆，反之跳出循环，写入预测的抵达时间。
    //      b. 依照上一步得到的信息更新队列区时间与顺序
    //          b+. 调用队列信息更新函数进行后续车辆通过信息的更新
    //      c. 使用outTime计算进入并返回下一条道路的时间
    // 0. 将车辆插入进入时间队列

    // a.1. 判定队列区起点车辆
    TraversalHelper queueHelper;
    string startCar = queueHelper.getFirstCarId(carMapByQueueTime,time);
    // todo 整个过程需要重新规划编写

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
    // FIXME 用于释放资源
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
    // FIXME 需要判定是否依然存在于队列之中
    removeCar(id);
    return outTime;
}

void SMTCarInfoQueue::setCycleInfo(double period, double allowTime, double offset) {
    cyclePeriod = period;
    allowedInterval = allowTime;
    cycleOffset = offset;
}

void SMTCarInfoQueue::removeCar(string id) {
    // 移除车辆的条件，需要同时满足已经离开了当前道路，并且已经经过了预测离开道路的时间
    carMapById.erase(id);
    carMapByEnterTime[enterTimeMapById[id]].remove(id);
    carMapByQueueTime[queueTimeMapById[id]].remove(id);
    carMapByOutTime[outTimeMapById[id]].remove(id);
    enterTimeMapById.erase(id);
    queueTimeMapById.erase(id);
    outTimeMapById.erase(id);
    outQueueTimeMapById.erase(id);
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

// if the out time is not allowed to get out, then return the start of next allowed time
double SMTCarInfoQueue::getFixedOutTime(double time) {
    // fixme 最终方案应该通过红绿灯相关xml文件进行设定
    // the final solution is read the xml file, now just use the cycle period
    // 先求得当前时间点对应的周期的允许通行时间的起点
    double preAllowedTime = cycleOffset + cyclePeriod * (int) ((time - cycleOffset) / cyclePeriod);
    // 若当前时间超出了该周期的通行时间，则修正通行时间为下一个通行周期，反之不做变动
    if(time - preAllowedTime > allowedInterval){
        time = preAllowedTime + cyclePeriod;
    }
    return time;
}

bool SMTCarInfoQueue::isCarACanOvertakeCarB(string carA, string carB, double enterTimeA, double enterTimeB,
        double freeSpace) {
    // 进行车辆超越相关的判定
    // FIXME 进入时间相等时，需要读取队列进行判定，若B先于A出现则无法超越，反之能够超越
    SMTCarInfo carInfoA = carMapById[carA];
    SMTCarInfo carInfoB = carMapById[carB];
    if(overtakeAllowed){
        // 仅当允许超车时,进行超车判定
        if(enterTimeA >= enterTimeB){
            // A晚于B进入道路
            if(freeSpace - carInfoA.length - carInfoB.minGap < 0){
                // 若空间不够，则A无法超越B
                return false;
            }
            // caculate the time of the current car reach the queue area if A overtake B successfully
            double reachTimeForCarA = getTheReachTime(carInfoA, freeSpace, enterTimeA, false, true);
            double reachTimeForCarB = getTheReachTime(carInfoB,
                    freeSpace - carInfoA.minGap - carInfoA.length - carInfoB.minGap, enterTimeB, false, true);
            // decide A overtake B or not
            bool beOvertake = reachTimeForCarA < reachTimeForCarB;
            return beOvertake;
        }else{
            // A先于B进入道路
            if(freeSpace - carInfoA.length - carInfoB.minGap < 0){
                // 若空间不够，则A在B的前方
                return true;
            }
            // caculate the time of the current car reach the queue area if B could overtake A successfully
            double reachTimeForCarA = getTheReachTime(carInfoA,
                    freeSpace - carInfoB.minGap - carInfoB.length - carInfoA.minGap, enterTimeA, false, true);
            double reachTimeForCarB = getTheReachTime(carInfoB, freeSpace, enterTimeB, false, true);
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

double SMTCarInfoQueue::getStartTimeOfAllowedTime(double time) {
    // 说明：
    //      获取对应时间的通行允许时间的起点
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

double SMTCarInfoQueue::getFixedTimeWithUpdateInterval(double time) {
    return ((int) (time / updateInterval)) * updateInterval;
}

double SMTCarInfoQueue::TraversalHelper::getCurrentKey() {
    if(it != carListMap->end()){
        return it->first;
    }else{
        return -1.0;
    }
}

double SMTCarInfoQueue::TraversalHelper::getNextkey() {
    double key = -1.0;
    if(it != carListMap->end()){
        it++;
        key = getCurrentKey();
        it--;
    }
    return key;
}

double SMTCarInfoQueue::TraversalHelper::getPreviousKey() {
    double key = -1.0;
    if(it != carListMap->begin()){
        it--;
        key = getCurrentKey();
        it++;
    }
    return key;
}

string SMTCarInfoQueue::TraversalHelper::SeekToCar(string car) {
    // get first time node (include given time)
    if(*lit != it->second.end()){
        string curCar = *lit;
        while(curCar != "" && curCar != car){
            curCar = getNextCarId();
        }
    }
    return "";
}

string SMTCarInfoQueue::TraversalHelper::getFirstCarIdAfter(const map<double, list<string> >& carListMapByCertainTime,
        double time) {
    // 获取指定时间点之后的第一个车辆id(包含当前时间点)
    carListMap = &carListMapByCertainTime;
    // get first time node (include given time)
    it = carListMap->upper_bound(time);
    if(it != carListMap->end()){
        lit = it->second.begin();
        if(lit != it->second.end()){
            return *lit;
        }
    }
    return "";
}

list<string> SMTCarInfoQueue::TraversalHelper::pushCurrentCarBack(double time) {
    //说明：
    //      该用能用于推后遍历助手当前车辆至某一时间点
    // 算法过程：
    //      a. 收集当前迭代器指向的车辆到时间小于time的车辆的列表
    //      b. 将这些车辆以此插入到时间为time的节点头部
    list<string> stack;
    if(it != carListMap->end()){
        //a. 收集当前迭代器指向的车辆到时间小于time的车辆的列表
        // 如果it不是end，则lit对应的当前车辆一定是存在的，直接将其与其当前时间节点后方的车辆一起插入到stack中即可
        stack.splice(stack.end(), it->second, lit, it->second.end());
        // 记录被移除内容的时间点
        double oldTime = it->first;
        // 移动至下一时间节点(为保证it与lit的一致性，在末尾需要重设lit指针)
        it++;
        // 若前一个节点已被全部移除，则将对应时间节点移除
        if(carListMap[oldTime].size() == 0){
            carListMap->erase(oldTime);
        }
        // 将时间小于time的所有当前车辆后方的车辆加入stack表
        while(it != carListMap->end() && it->first < time){
            stack.splice(stack.end(), it->second, it->second.begin(), it->second.end());
            // 记录被移除内容的时间点
            oldTime = it->first;
            // 移动至下一时间节点
            it++;
            // 若前一个节点已被全部移除，则将对应时间节点移除
            if(carListMap[oldTime].size() == 0){
                // 实际上如果执行了这个循环上面的移动操作，肯定是全被移除的
                carListMap->erase(oldTime);
            }
        }
        // b. 将这些车辆以此插入到时间为time的节点头部
        // 处理结尾部分
        if(it->first == time){
            // 若存在时间为time的节点，则将stack列表合并至该节点前方
            it->second.insert(it->second.begin(), stack.begin(), stack.end());
        }else{
            // 若不存在时间为time的节点，则将stack中的车辆添加到时间为time的节点上
            carListMap[time] = stack;
        }
        // 同步并重设it与lit指针
        it = carListMap->find(time);
        lit = it->second.begin();
    }else{
        // 当前车辆不存在，则当前算法不应当执行，打印警告信息
        cout << "Error@TraversalHelper::pushCurrentCarBack::MISSING CAR" << endl;
    }
    return stack;
}

} /* namespace Fanjing */
