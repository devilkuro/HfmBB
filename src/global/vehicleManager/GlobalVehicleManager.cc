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

#include "GlobalVehicleManager.h"

Define_Module(GlobalVehicleManager);

void GlobalVehicleManager::initialize()
{
    // TODO - Generated method body
    // TODO just for test use
    testMsg = new cMessage("testMsg");
    scheduleAt(simTime()+0.1,testMsg);
}

void GlobalVehicleManager::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    if(msg==testMsg){
        if (getMapSystem()->isInitializedFinished()) {
            addVehicles(GVM_VEHICLETYPE_NORMAL, 1);
        }else{
            scheduleAt(simTime()+0.1,testMsg);
        }
    }
}

void GlobalVehicleManager::finish() {
    // TODO - Generated method body
}

GlobalMapSystem* GlobalVehicleManager::getMapSystem() {
    // TODO - Generated method body
    if(!map){
        map = GlobalMapSystemAccess().get();
    }
    ASSERT(map);
    return map;
}

void GlobalVehicleManager::updateVehicleList() {
    // TODO - Generated method body
}

void GlobalVehicleManager::addOneVehicle(VehicleType type) {
    // TODO - Generated method body
    getMapSystem()->addOneVehicle("","","-23350929");
}

GlobalVehicleManager::GlobalVehicleManager() {
    map = NULL;
    testMsg = NULL;
}

GlobalVehicleManager::~GlobalVehicleManager() {
}

void GlobalVehicleManager::addVehicles(VehicleType type, int num) {
    // TODO - Generated method body
    getMapSystem()->addVehicles(num);
}
