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

#ifndef __SMARTCITYFLOWRESERVED_GLOBALMOBILITYLAUNCHD_H_
#define __SMARTCITYFLOWRESERVED_GLOBALMOBILITYLAUNCHD_H_

#include <omnetpp.h>
#include "TraCIScenarioManagerLaunchd.h"
#include "TraCIConstants.h"

/**
 *
 */
class GlobalMobilityLaunchd : public TraCIScenarioManagerLaunchd {
public:
    virtual ~GlobalMobilityLaunchd();

public:
    enum GMLLaneChangeMode {
        GML_NO_STRATEGIC = 0x00,
        GML_ALLOW_STRATEGIC = 0x01,
        GML_FORCE_STRATEGIC = 0x02,
        GML_NO_COOPERATIVE = 0x00 << 2,
        GML_ALLOW_COOPERATIVE = 0x01 << 2,
        GML_FORCE_COOPERATIVE = 0x02 << 2,
        GML_NO_SPEED_GAIN = 0x00 << 4,
        GML_ALLOW_SPEED_GAIN = 0x01 << 4,
        GML_FORCE_SPEED_GAIN = 0x02 << 4,
        GML_NO_DRIVE_ON_RIGHT = 0x00 << 6,
        GML_ALLOW_DRIVE_ON_RIGHT = 0x01 << 6,
        GML_FORCE_DRIVE_ON_RIGHT = 0x02 << 6,
        GML_EXT_TRACI_IGNORE_OTHERS = 0x00 << 8,
        GML_EXT_TRACI_AVOID_COLLISIONS = 0x01 << 8,
        GML_EXT_TRACI_ONLY_BY_SPEED = 0x10 << 8,
        GML_EXT_TRACI_NATURAL = 0x11 << 8,
        GML_ALLOW_ALL = GML_ALLOW_STRATEGIC | GML_ALLOW_COOPERATIVE | GML_ALLOW_SPEED_GAIN | GML_ALLOW_DRIVE_ON_RIGHT,
        GML_DISALLOW_ALL = GML_NO_STRATEGIC|GML_NO_COOPERATIVE|GML_NO_SPEED_GAIN|GML_NO_DRIVE_ON_RIGHT
    };
public:
    // xml related
    std::string getLaunchConfigXMLPath();
    uint32_t getActiveVehicleCount();
    std::list<std::string> commandGetLaneLinksIds(std::string laneId);
    void commandAddRoute(std::string routeId, std::list<std::string> route);
    void commandSetRouteByEdgeList(std::string nodeId, std::list<std::string> route);
    void commandSetLaneChangeMode(std::string nodeId, uint32_t bitset);
protected:
    std::list<std::string> laneLinksGetStringList(uint8_t commandId, std::string objectId, uint8_t variableId,
            uint8_t responseId);
};
class GlobalMobilityLaunchdAccess {
public:
    GlobalMobilityLaunchd* get() {
        return FindModule<GlobalMobilityLaunchd*>::findGlobalModule();
    }
};
#endif
