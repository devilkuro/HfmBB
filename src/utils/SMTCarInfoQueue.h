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
#include "tinyxml2.h"
using namespace std;
using namespace tinyxml2;
namespace Fanjing {

/*
 *
 */

class SMTCarInfoQueue {
public:
    SMTCarInfoQueue(string lane, string xmlpath, double length, double outLength);
    virtual ~SMTCarInfoQueue();

    string laneName;
    double laneLength;
    double laneOutLength;
    // insert a car and return its predicted out time
    // neighborFrozenSpace is the length of the overtake disallowed space
    // usually equals to the minimum one of the neighbors' queue length.
    // neighborFrozenSpace = 0 means car can freely overtake others
    double insertCar(SMTCarInfo car, double time, double neighborFrozenSpace = 0);
    // release the old and invalid car
    // not necessary at now... or forever...
    void setCurrentTime(double time);
    // release the old car and invalid resource
    // then return the predicted out time
    double releaseCar(string id, double time);
    // allow or disallow the overtake action
    static void saveResults(string filename);
    static void releaseXML();
    void setCycleInfo(double period, double allowTime, double offset);

    inline static void setOvertakeMode(bool allow) {
        overtakeAllowed = allow;
    }
    inline static void setUpdateInterval(double interval) {
        updateInterval = interval;
    }
protected:
    SMTCarInfoQueue();
    static bool overtakeAllowed;
    static double updateInterval;
    static bool onlyLosseOneCar;
    // car time manage related
    map<string, SMTCarInfo> carMapById;
    // map by time
    // 由时间点进行索引的车辆列表
    map<double, list<string> > carMapByEnterTime;
    map<double, list<string> > carMapByQueueTime;
    map<double, list<string> > carMapByOutTime;
    // map by id
    // 通过车辆id进行索引的个状态时间
    map<string, double> enterTimeMapById;
    map<string, double> queueTimeMapById;
    map<string, double> outTimeMapById;
    // the time car start to get out the queue area
    // 车辆启动并开始离开队列区的时间
    // 该时间取决于前方车辆启动的时间
    // 若前方没有车辆,则取决于控制信号下一个允许通行的时间
    // 若前方没有车辆,且控制信号允许则等于该车进入队列区的时间
    // 最后一种情况需要进行额外处理（因为少了加速减速的过程）
    map<string, double> outQueueTimeMapById;

    map<double, list<string> >::iterator itCarMapByEnterTime;
    map<double, list<string> >::iterator itcarMapByQueueTime;
    map<double, list<string> >::iterator itcarMapByOutTime;
    // car map
    list<string>::iterator litCarMapByEnterTime;
    list<string>::iterator litcarMapByQueueTime;
    list<string>::iterator litcarMapByOutTime;

    double allowedInterval;
    double cyclePeriod;
    double cycleOffset;

    string roadName;
    string nextRoadName;

    // statistic related
    static XMLDocument* doc;
    static bool XMLHasLoaded;
    XMLElement* root;
    XMLElement* element;
    // utils functions
    // update the car with id = id and the cars affected it
    void updateCarQueueInfoAt(string id);
    // set the enter time of a car and update both carMapByEnterTime and enterTimeMapById
    void setEnterTimeOfCar(string id, double time);
    // similar to the above function
    // set the enter time of a car and update both carMapByQueueTime and queueTimeMapById
    void setQueueTimeOfCar(string id, double time);
    // similar to the above function
    // set the enter time of a car and update both carMapByOutTime and outTimeMapById
    void setOutTimeOfCar(string id, double time);
    // get car info by id
    SMTCarInfo getCarInfoById(string id);
    // get the car id of the car of which enter time is before the given time
    string getFirstCarIdByEnterTime(double time);
    string getNextCarIdByEnterTime();
    string getPreviousCarIdByEnterTime();
    // get the car id of the car of which queue time is before the given time
    string getFirstCarIdByQueueTime(double time);
    string getNextCarIdByQueueTime();
    string getPreviousCarIdByQueueTime();
    // get the car id of the car of which queue time is before the given time
    string getFirstCarIdByOutTime(double time);
    string getNextCarIdByOutTime();
    string getPreviousCarIdByOutTime();

    // get the car by time
    string getFirstCarIdByCertainTime(map<double, list<string> > &carListMapByCertainTime,
            map<double, list<string> >::iterator &it, list<string>::iterator &lit, double time);
    string getNextCarIdByCertainTime(map<double, list<string> > &carListMapByCertainTime,
            map<double, list<string> >::iterator &it, list<string>::iterator &lit);
    string getPreviousCarIdByCertainTime(map<double, list<string> > &carListMapByCertainTime,
            map<double, list<string> >::iterator &it, list<string>::iterator &lit);

    // set the pair map
    void setThePairMap(map<double, list<string> > &carListMapByTime, map<string, double>&timeMapByCar, string id,
            double time);
    void setThePairMapAtFrontOfCar(map<double, list<string> > &carListMapByTime, map<string, double>&timeMapByCar,
            string id, string otherId);
    void setThePairMapAtBackOfCar(map<double, list<string> > &carListMapByTime, map<string, double>&timeMapByCar,
            string id, string otherId);
    void removeCar(string id);
    // fix the out time by considering the allowed time
    double getFixedOutTime(double time);
    // judge whether the car can overtake other car successfully or not
    bool isCarACanOvertakeCarB(SMTCarInfo carA, SMTCarInfo carB, double enterTimeA, double enterTimeB,
            double freeSpace);
    // caculate the reach time
    double getTheReachTime(SMTCarInfo car, double length, double startTime, bool considerAccel, bool considerDecel);
private:
    void init();
};

} /* namespace Fanjing */

#endif /* SMTCARINFOQUEUE_H_ */
