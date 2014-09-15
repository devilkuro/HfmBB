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
class GlobalMobilityLaunchd: public TraCIScenarioManagerLaunchd {
public:
    virtual ~GlobalMobilityLaunchd();

public:
    uint32_t getActiveVehicleCount();
    std::list<std::string> commandGetLaneLinksIds(std::string laneId);
    void commandAddRoute(std::string routeId, std::list<std::string> route);
    void commandChangeRouteByEdgeList(std::string nodeId, std::list<std::string> route);
protected:
    std::list<std::string> laneLinksGetStringList(uint8_t commandId, std::string objectId, uint8_t variableId, uint8_t responseId);
};
class GlobalMobilityLaunchdAccess {
public:
    GlobalMobilityLaunchd* get() {
        return FindModule<GlobalMobilityLaunchd*>::findGlobalModule();
    }
};
#endif
