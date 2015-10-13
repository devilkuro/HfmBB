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

#ifndef __SMARTCITYFLOWRESERVED_TRACIMOBILITY_FIXED_H_
#define __SMARTCITYFLOWRESERVED_TRACIMOBILITY_FIXED_H_

#include <omnetpp.h>
#include "TraCIMobility.h"
#include "GlobalMapSystem.h"
#include "string"
#include "StatisticsRecordTools.h"
#include "ASMTimer.h"
#include "GlobalVehicleManager.h"

using Fanjing::StatisticsRecordTools;
/**
 *
 */
//using std::string;
class TraCIMobility_Fixed : public TraCIMobility {
public:
    TraCIMobility_Fixed();
public:
    virtual void initialize(int);
    virtual void finish();
    virtual void preInitialize(std::string external_id, const Coord& position, std::string road_id = "", double speed =
            -1, double angle = -1);
    virtual void nextPosition(const Coord& position, std::string road_id = "", double speed = -1, double angle = -1,
            TraCIScenarioManager::VehicleSignal signals = TraCIScenarioManager::VEH_SIGNAL_UNDEF);
    virtual void changePosition();
    virtual GlobalMapSystem* getMapSystem() const {
        if(!map){
            map = GlobalMapSystemAccess().get();
        }
        ASSERT(map);
        return map;
    }
    virtual GlobalVehicleManager* getVehicleManager() const {
        if(!vehicleManager){
            vehicleManager = GlobalVehicleManagerAccess().get();
        }
        ASSERT(vehicleManager);
        return vehicleManager;
    }
protected:
    // set lane change mode of this node
    void disableLaneChange();
    void allowLaneChange();

    // get lane position of this vehicle
    double getLanePosition();

protected:
    mutable GlobalMapSystem *map;
    mutable GlobalVehicleManager *vehicleManager;
    StatisticsRecordTools* srt;
    simtime_t lastDroveAt;
    // fixme chage to the correct class
    ASMTimer asmtimer;
    // path process
    bool hasRouted;
    // statistics members
    bool hasInitialized;
    double statistic_road_enterTime;
    double statistic_start_time;
    int statistic_road_enterVehicleNum;
    int statistic_junction_enterVehicleNum;
    string last_road_id;

protected:
    // overload these function in different mobility
    // processAfterRouting
    // this function will run every 0.1 second for each car if routed in routing process!!
    // so, do not do any complicated operations here.
    virtual void processAfterRouting();
    // statisticAtFinish
    virtual void statisticAtFinish();
    // initialize the route in routing process
    virtual void processAtRouting();
    // when road changed from one road to another, not the first time appearing on the map.
    virtual void processWhenChangeRoad();
    // when the car first appear on the map.
    virtual void processWhenInitializingRoad();
    // this function will run every 0.1 second for each car!!
    // so, do not do any complicated operations here.
    virtual void processWhenNextPosition();
};

#endif
