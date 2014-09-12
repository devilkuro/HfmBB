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

#include "TraCIMobility_Fixed.h"

Define_Module(TraCIMobility_Fixed)

void TraCIMobility_Fixed::preInitialize(std::string external_id,
        const Coord& position, std::string road_id, double speed,
        double angle) {
    Enter_Method_Silent();
    TraCIMobility::preInitialize(external_id,position,road_id,speed,angle);

}

void TraCIMobility_Fixed::nextPosition(const Coord& position,
        std::string road_id, double speed, double angle,
        TraCIScenarioManager::VehicleSignal signals) {
    Enter_Method_Silent();
    TraCIMobility::nextPosition(position,road_id,speed,angle,signals);

}

void TraCIMobility_Fixed::changePosition() {
    Enter_Method_Silent();
    TraCIMobility::changePosition();
}
