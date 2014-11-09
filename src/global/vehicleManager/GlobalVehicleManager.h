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

#ifndef __SMARTCITYFLOWRESERVED_GLOBALVEHICLEMANAGER_H_
#define __SMARTCITYFLOWRESERVED_GLOBALVEHICLEMANAGER_H_

#include <omnetpp.h>
#include "GlobalMapSystem.h"

/**
 * TODO - Generated class
 */
class GlobalVehicleManager : public cSimpleModule {
public:
    GlobalVehicleManager();
    virtual ~GlobalVehicleManager();
public:
    enum VehicleType {
        GVM_VEHICLETYPE_NORMAL = 0, // normal vehicle: use a random path
        GVM_VEHICLETYPE_ONDUTY,   // on-duty vehicle: drive to work place and back
        GVM_VEHICLETYPE_BUS,    // bus: fixed loop path
        GVM_VEHICLETYPE_TAXI,   // taxi: continually random path
        GVM_VEHICLETYPE_EMERGENCE,  // emergence vehicle: go to a random location and back to the station
        GVM_VEHICLETYPE_ADMIN,  // unnecessary! road administration: come out when the transport system is idle
        GVM_VEHICLETYPE_SHOPPING, // unimportant! shopping vehicle: go to a shop and back. just a car with a random destination now.
        // add new vehicle type above if any.
        GVM_VEHICLETYPE_NUMEND_MARK // means the number of the vehicle type
    };

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

private:
    cMessage* testMsg;
    // members
    GlobalMapSystem *map;
    // used in vehicle generating process
    std::map<int, int> targetVehicleNumPerType;   // the target vehicles number of each vehicle type
    std::map<int, int> vehicleNumPerType;   // the vehicles number of each vehicle type

    // functions
    GlobalMapSystem* getMapSystem();

    // car generating process
    void updateVehicleList();   // generate vehicles to keep there are certain number vehicles in the network
    // functions to add vehicles
    // the mobility module must get its vehicle type by using function - getVehiclesType()
    // if else that the vehicles will be marked as normal vehicle
    void addOneVehicle(VehicleType type);   // add a car of a certain type
    void addVehicles(VehicleType type, int num); // add several cars of a certain type
};

#endif
