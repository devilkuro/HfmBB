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
#include <set>
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
    // 用于遍历的内部类
    class TraversalHelper {
        // iterator 便利迭代器
    public:
        TraversalHelper();
        virtual ~TraversalHelper();
        map<double, list<string> > *carListMap;
        map<double, list<string> >::iterator it;
        list<string>::iterator lit;
        // get the car by time
        string seekToCar(string car);
        string getFirstCarId(map<double, list<string> > &carListMapByCertainTime, double time);
        string getFirstCarIdAfter(map<double, list<string> > &carListMapByCertainTime, double time);
        string getNextCarId();
        string getPreviousCarId();
        string getLastCarCurrent();
        // key 相关方法仅读取，不修改当前遍历状态(两个迭代器修改必须同步)
        double getCurrentKey();
        double getNextkey();
        double getPreviousKey();
        // 将当前车辆向后推迟至时间点,并返回被推迟车辆的列表
        list<string> pushCurrentCarBack(double time);
    };
public:
    // xmlpath只能设置一次，之后的设置将不改变xml文件路径
    // 当系统终止时，需要调用saveResults方法保存结果，之后xml文件会被释放。
    SMTCarInfoQueue();
    SMTCarInfoQueue(string lane, string xmlpath, double length, double outLength);
    virtual ~SMTCarInfoQueue();

    string xmlName;
    string txtName;
    string laneName;
    double laneLength;
    double laneOutLength;
    // insert a car and return its predicted out time
    // neighborFrozenSpace is the length of the overtake disallowed space
    // usually equals to the minimum one of the neighbors' queue length
    // neighborFrozenSpace = 0 means car can freely overtake others
    double insertCar(SMTCarInfo car, double time, double neighborFrozenSpace = 0);
    // release the old and invalid car
    // not necessary at now... or forever...
    void setCurrentTime(double time);
    // release the old car and invalid resource
    // then return the predicted out time
    double releaseCar(string id, double time, double avgTime);
    void saveResults(string filename);
    void releaseXML();
    void setCycleInfo(double period, double allowTime, double offset);

    // allow or disallow the overtake action
    inline static void setOvertakeMode(bool allow) {
        overtakeAllowed = allow;
    }
    inline static void setUpdateInterval(double interval) {
        updateInterval = interval;
    }
protected:
    static bool overtakeAllowed;
    static double updateInterval;
    static bool onlyLosseOneCar;
    static double startDelay; // 在前方车辆启动后，当前车辆相对于前方车的启动延迟
    // car time manage related
    map<string, SMTCarInfo> carMapById;
    // map by time
    // 由时间点进行索引的车辆列表,用于通过时间反向查找遍历对应车辆
    map<double, list<string> > carMapByEnterTime;
    map<double, list<string> > carMapByQueueTime;
    map<double, list<string> > carMapByOutTime;
    // map by id
    // 通过车辆id进行索引的个状态时间
    // 车辆进入道路的时间
    map<string, double> enterTimeMapById;
    // 车辆进入队列区的时间，需要根据当前队列长度，道路长度，超车判定共同决定
    // 当前队列长度等于
    // 1. 进入队列区时间早于当前车辆进入道路时间
    // 2. 驶离道路时间晚于当前车辆进入道路时间
    // 的车辆队列长度的总和
    // (其中2为估算终止条件，因为车辆进入队列过程中，车是会动的，实际队列长度要小于该总和)
    map<string, double> queueTimeMapById;
    // 车辆离开道路的时间，由离开队列区时间和
    map<string, double> outTimeMapById;
    // the time car start to get out the queue area
    // 车辆启动并开始离开队列区的时间
    // 该时间取决于前方车辆启动的时间
    // 若前方没有车辆,则取决于控制信号下一个允许通行的时间
    // 若前方没有车辆,且控制信号允许则等于该车进入队列区的时间
    // 最后一种情况需要进行额外处理（因为少了加速减速的过程）
    map<string, double> outQueueTimeMapById;
    map<string, double> nextRoadTimeMapById;
    map<string, double> lastNcarPassTime;
    map<string, double> lastNminPassTime;
    map<string, double> predicOutTime;
    map<string, double> historialOutTime;
    map<string, double> historialAccurateTime;
    // 失效车辆集合
    set<string> invaildCarSet;

    double allowedInterval;
    double cyclePeriod;
    double cycleOffset;

    string roadName;
    string nextRoadName;

    // statistic related
    XMLDocument* doc;
    bool XMLHasLoaded;
    static int global_xml_index;
    string xml_suffix;

    XMLElement* root;
    XMLElement* element;
    // utils functions
    // update the car with id = id and the cars affected it
    void updateCarQueueInfoAt(string id, string preId);
    // 更新车辆进入队列的时间
    void updateCarEnterQueueInfo(string id, string preId);
    // 更新车辆离开相关信息(包括启动离开队列区的时间、具体离开队列区的时间)
    void updateCarOutInfo(string id, string preId);
    // set the enter time of a car and update both carMapByEnterTime and enterTimeMapById
    void setEnterTimeOfCar(string id, double time);
    // similar to the above function
    // set the enter time of a car and update both carMapByQueueTime and queueTimeMapById
    void setQueueTimeOfCar(string id, double time);
    // similar to the above function
    // set the enter time of a car and update both carMapByOutTime and outTimeMapById
    void setOutTimeOfCar(string id, double time);
    // 将对应遍历器对应的车辆推后至某一时刻(请务必保证遍历助手对应的map为queueTimeMap)
    void pushCarQueueTimeBack(TraversalHelper &queueTimeHelper, double time);
    void pushCarQueueTimeBack(string car, double time);
    // get car info by id
    SMTCarInfo getCarInfoById(string id);

    // set the pair map
    void setThePairMap(map<double, list<string> > &carListMapByTime, map<string, double>&timeMapByCar, string id,
            double time);
    void setThePairMapAtFrontOfCar(map<double, list<string> > &carListMapByTime, map<string, double>&timeMapByCar,
            string id, string otherId);
    void setThePairMapAtBackOfCar(map<double, list<string> > &carListMapByTime, map<string, double>&timeMapByCar,
            string id, string otherId);
    void setCarAtFirstOfCertainTime(map<double, list<string> > &carListMapByTime, map<string, double>&timeMapByCar,
            string id, double time);
    void removeCar(string id);

    // fix the out time by considering the allowed time
    double getFixedOutTime(double time);
    // 获取对应时间的通行允许时间的起点
    // 若当前时间可以通行，则返回当前时间通行周期的起点
    // 反之，则返回下一个通行周期的起点
    double getStartTimeOfAllowedTime(double time);
    // judge whether the car can overtake other car successfully or not
    bool isCarACanOvertakeCarB(string carA, string carB, double enterTimeA, double enterTimeB, double freeSpace);
    // caculate the reach time
    // 计算车辆行驶指定距离需要消耗的时间,考虑或者不考虑前后加减速阶段
    double getTheReachTime(SMTCarInfo car, double length, double startTime, bool considerAccel, bool considerDecel);
    // 将时间转换为updateInterval的整数倍
    double getFixedTimeWithUpdateInterval(double time);
    // 获取由队列区队列fromId到toId车辆构成的队列的总长度(fromId和toId必须存在于队列区队列中)
    double getQueueLength(string fromId, string toId);
private:
    void init();
};

} /* namespace Fanjing */

#endif /* SMTCARINFOQUEUE_H_ */
