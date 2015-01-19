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
#include "GlobalStatistics.h"

/**
 *
 */
//using std::string;
class TraCIMobility_Fixed : public TraCIMobility {
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
protected:
    // set lane change mode of this node
    void disableLaneChange();
    void allowLaneChange();

    // get lane position of this vehicle
    double getLanePosition();
protected:
    mutable GlobalMapSystem *map;
    mutable GlobalStatistics *gs;

    // path process
    bool hasRouted;
    // statistics members
    bool hasInitialized;
    double statistic_road_enterTime;
    double statistic_start_time;
    int statistic_road_enterVehicleNum;
    int statistic_junction_enterVehicleNum;
    string last_road_id;

};

#endif
