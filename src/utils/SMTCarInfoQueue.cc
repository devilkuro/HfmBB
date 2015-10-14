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

namespace Fanjing {
bool SMTCarInfoQueue::overtakeAllowed = false;
double SMTCarInfoQueue::updateInterval = 0.1;
XMLDocument* SMTCarInfoQueue::doc = NULL;
SMTCarInfoQueue::SMTCarInfoQueue() {
    // TODO Auto-generated constructor stub
    init();
}

SMTCarInfoQueue::SMTCarInfoQueue(string lane, string xmlpath, double length, double outLength) {
    init();
    laneName = lane;
    laneLength = length;
    laneOutLength = outLength;
    doc->LoadFile(xmlpath.c_str());
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
    doc->Clear();
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
    // todo
    // 1st. set the time entering this lane
    carMapById[car.id] = car;
    setEnterTimeOfCar(car.id, time);
    // 1st. judge the length of queue and the time entering the queue
    double queueLength = 0;
    string otherId = getFirstCarIdByOutTime(time);
    while(otherId != ""){
        // while there are some still in this lane
        // and if those car have already enter the queue area when this car entered this lane
        // add its gap and length into the queue length
        if(queueTimeMapById[otherId] < time){
            SMTCarInfo otherCar = getCarInfoById(otherId);
            queueLength += otherCar.minGap + otherCar.length;
            otherId = getNextCarIdByOutTime();
        }
    }
    // 2nd. judge the length of queue considering the cars have not entered the queue yet

    otherId = getFirstCarIdByQueueTime(time);
    // there still have some cars not entered the queue
    while(otherId != ""){
        // if the car has already entered this lane
        if(enterTimeMapById[otherId] < time){
            SMTCarInfo otherCar = getCarInfoById(otherId);
            if(overtakeAllowed){
                // 2nd(optional). overtake judgement process
                // process only if overtake process is enabled
                // if overtake is enable, the car befor this car may not increase its queue length
                // otherwise, all cars before this car will be added into the queue length.

                // calculate the problem of distance
                // assume current car can overtake other car
                // then when the current car reach the queue area
                // the other car should not reach the length + gap of both cars
                // before the queue area
                double overtakeLengthForOtherCar = laneLength - queueLength - car.minGap - car.length;
                // caculate the distance from the start to the queue area if overtake successfully
                double overtakeLengtheForCurrentCar = laneLength - queueLength;
                // caculate the time of the current car reach the queue area if overtake successfully
                double reachTimeForCurrentCar = getTheReachTime(car, overtakeLengtheForCurrentCar, time, false, true);
                double reachTimeForOtherCar = getTheReachTime(otherCar, overtakeLengthForOtherCar,
                        enterTimeMapById[otherId], false, true);
                // compare two time
                if(reachTimeForCurrentCar > reachTimeForOtherCar){
                    // if current car reach later, other car get to the queue before current car
                    queueLength += otherCar.minGap + otherCar.length;
                }else{
                    // the information of other cars which are overtaked by current car needs to be updated
                }
            }else{
                // if overtake process is disabled
                // then all cars before this car will be added into the queue length.
                queueLength += otherCar.minGap + otherCar.length;
            }
        }
        // calculate the overtake length
        double overtakeLength = queueLength;

    }
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

void SMTCarInfoQueue::setThePairMap(map<double, list<string>> &carListMapByTime, map<string, double>&timeMapByCar,
        string id, double time) {
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

string SMTCarInfoQueue::getFirstCarIdByEnterTime(double time) {
    return getFirstCarIdByCertainTime(carMapByEnterTime, itCarMapByEnterTime, litCarMapByEnterTime, time);
}

string SMTCarInfoQueue::getFirstCarIdByQueueTime(double time) {
    return getFirstCarIdByCertainTime(carMapByQueueTime, itcarMapByQueueTime, litcarMapByQueueTime, time);
}

string SMTCarInfoQueue::getFirstCarIdByOutTime(double time) {
    return getFirstCarIdByCertainTime(carMapByOutTime, itcarMapByOutTime, litcarMapByOutTime, time);
}

string SMTCarInfoQueue::getNextCarIdByEnterTime() {
    return getNextCarIdByCertainTime(carMapByEnterTime, itCarMapByEnterTime, litCarMapByEnterTime);
}

string SMTCarInfoQueue::getNextCarIdByQueueTime() {
    return getNextCarIdByCertainTime(carMapByQueueTime, itcarMapByQueueTime, litcarMapByQueueTime);
}

string SMTCarInfoQueue::getNextCarIdByOutTime() {
    return getNextCarIdByCertainTime(carMapByOutTime, itcarMapByOutTime, litcarMapByOutTime);
}

string SMTCarInfoQueue::getFirstCarIdByCertainTime(map<double, list<string> >& carListMapByCertainTime,
        map<double, list<string> >::iterator& it, list<string>::iterator& lit, double time) {
// todo
// get first time node (include given time)
    it = carListMapByCertainTime.lower_bound(time);
    lit = it->second.begin();
    if(lit != it->second.begin()){
        return *lit;
    }
    return "";
}

string SMTCarInfoQueue::getNextCarIdByCertainTime(map<double, list<string> >& carListMapByCertainTime,
        map<double, list<string> >::iterator& it, list<string>::iterator& lit) {
    if(lit != it->second.end()){
        // if the list has more objects, get next
        lit++;
        return *lit;
    }else if(it != carListMapByCertainTime.end()){
        // if the list has no object any more get later time list and return the first car
        it++;
        lit = it->second.begin();
        if(lit != it->second.end()){
            return *lit;
        }
    }
// if the list has no more object and no car after the time return blank string
    return "";
}

// if the out time is not allowed to get out, then return the start of next allowed time
double SMTCarInfoQueue::getFixedOutTime(double time) {
// fixme the final solution is read the xml file, now just use the cycle period
    double preAllowedTime = cycleOffset + cyclePeriod * (int) ((time - cycleOffset) / cyclePeriod);
    if(time - preAllowedTime > allowedInterval){
        time = preAllowedTime + cyclePeriod;
    }
    return time;
}

} /* namespace Fanjing */
