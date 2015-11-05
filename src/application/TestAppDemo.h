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

#ifndef __SMARTCITYFLOWRESERVED_TESTAPPDEMO_H_
#define __SMARTCITYFLOWRESERVED_TESTAPPDEMO_H_

#include "BaseWaveApplLayer.h"
#include "TraCIMobility.h"

/**
 *
 */
class TestAppDemo : public BaseWaveApplLayer {
public:
    virtual void initialize(int stage);

protected:
    TraCIMobility* traci;
    AnnotationManager* annotations;
    simtime_t lastDroveAt;
    bool sentMessage;

protected:
    virtual void onBeacon(WaveShortMessage* wsm);
    virtual void onData(WaveShortMessage* wsm);
    void sendMessage(std::string blockedRoadId);
    virtual void handlePositionUpdate(cObject* obj);
};

#endif
