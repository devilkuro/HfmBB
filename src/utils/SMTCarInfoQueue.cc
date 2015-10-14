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

namespace Fanjing {
bool SMTCarInfoQueue::overtakeAllowed = false;

SMTCarInfoQueue::SMTCarInfoQueue() {
    // TODO Auto-generated constructor stub
    allowedInterval = 0;
    cyclePeriod = 0;
    offset = 0;

}

SMTCarInfoQueue::~SMTCarInfoQueue() {
    // TODO Auto-generated destructor stub
}

double SMTCarInfoQueue::insertCar(SMTCarInfo car, double time, double neighborFrozenSpace) {
    // todo
}

void SMTCarInfoQueue::setCurrentTime(double time) {
    // todo
}

double SMTCarInfoQueue::releaseCar(string id, double time) {
    // todo
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

double SMTCarInfoQueue::getFixedOutTime(double time) {
    // todo
}

} /* namespace Fanjing */
