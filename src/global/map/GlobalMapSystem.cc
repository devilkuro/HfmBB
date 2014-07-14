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

#include "GlobalMapSystem.h"

Define_Module(GlobalMapSystem);

void GlobalMapSystem::initialize(int stage)
{
    // TODO - Generated method body
    BaseModule::initialize(stage);
    if(stage == 2){
        startMsg = new cMessage("startMapSystem");
        scheduleAt(0.2,startMsg);
    }
}

GlobalMapSystem::~GlobalMapSystem() {
    // TODO - Generated method body
}

void GlobalMapSystem::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    if (msg == startMsg) {
        list<string> lanelist = getManager()->commandGetLaneIds();
        Lane* lane = new Lane();
        list<string>::iterator it_ll = lanelist.begin();
        it_ll++;
        it_ll++;
        lane->name = *it_ll;
        debugEV << lane->name << endl;
        list<string> lanelist_l0 = getLanes(lane);
        for (list<string>::iterator it = lanelist_l0.begin();
                it != lanelist_l0.end(); it++) {
            debugEV << (*it) << endl;
        }
    }
}

void GlobalMapSystem::generateMap() {
    // TODO - Generated method body
}

list<string> GlobalMapSystem::getLanes(Lane* lane) {
    return getManager()->commandGetLaneIds(lane->name);
}
