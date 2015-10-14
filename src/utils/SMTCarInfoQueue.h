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

#ifndef SMTCARINFOQUEUE_H_
#define SMTCARINFOQUEUE_H_

#include <map>
#include <string>
#include "SMTCarInfo.h"

using namespace std;

namespace Fanjing {

/*
 *
 */

class SMTCarInfoQueue {
public:
    SMTCarInfoQueue();
    virtual ~SMTCarInfoQueue();

    // insert a car and return its predicted out time
    // neighborFrozenSpace is the length of the overtake disallowed space
    // usually equals to the minimum one of the neighbors' queue length.
    // neighborFrozenSpace = 0 means car can freely overtake others
    double insertCar(SMTCarInfo car, double time, double neighborFrozenSpace = 0);
    // release the old and invalid car
    void setCurrentTime(double time);
    // release the old car and invalid resource
    // then return the predicted out time
    double releaseCar(string id, double time);
    // allow or disallow the overtake action
    inline static void setOvertakeMode(bool allow) {
        overtakeAllowed = allow;
    }

protected:
    static bool overtakeAllowed;
    // car time manage related
    map<string, SMTCarInfo> carMapById;
    map<double, list<string>> carMapByEnterTime;
    map<double, list<string>> carMapByQueueTime;
    map<double, list<string>> carMapByOutTime;
    map<string, double> enterTimeMapById;
    map<string, double> queueTimeMapById;
    map<string, double> outTimeMapById;

    map<double, list<string>>::iterator itCarMapByEnterTime;
    list<string>::iterator litCarMapByEnterTime;
    map<double, list<string>>::iterator itcarMapByQueueTime;
    list<string>::iterator litcarMapByQueueTime;
    map<double, list<string>>::iterator itcarMapByOutTime;
    list<string>::iterator litcarMapByOutTime;

    double allowedInterval;
    double cyclePeriod;
    double offset;

    string roadName;
    string nextRoadName;

    // utils functions
    // set the enter time of a car and update both carMapByEnterTime and enterTimeMapById
    void setEnterTimeOfCar(string id, double time);
    // similar to the above function
    // set the enter time of a car and update both carMapByQueueTime and queueTimeMapById
    void setQueueTimeOfCar(string id, double time);
    // similar to the above function
    // set the enter time of a car and update both carMapByOutTime and outTimeMapById
    void setOutTimeOfCar(string id, double time);
    // set the pair map
    void setThePairMap(map<double, list<string>> &carListMapByTime, map<string, double>&timeMapByCar, string id,
            double time);
    // get the car id of the car of which enter time is before the given time
    string getFirstCarIdByEnterTime(double time);
    string getNextCarIdByEnterTime();
    // get the car id of the car of which queue time is before the given time
    string getFirstCarIdByQueueTime(double time);
    string getNextCarIdByQueueTime();
    // get the car id of the car of which queue time is before the given time
    string getFirstCarIdByOutTime(double time);
    string getNextCarIdByOutTime();

    // get the car by time
    string getFirstCarIdByCertainTime(map<double, list<string>> &carListMapByCertainTime,
            map<double, list<string>>::iterator &it, list<string>::iterator &lit, double time);
    string getNextCarIdByCertainTime(map<double, list<string>> &carListMapByCertainTime,
            map<double, list<string>>::iterator &it, list<string>::iterator &lit);
    // fix the out time by considering the allowed time
    double getFixedOutTime(double time);
};

} /* namespace Fanjing */

#endif /* SMTCARINFOQUEUE_H_ */
