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

#include "TestAppDemo.h"

Define_Module(TestAppDemo)

void TestAppDemo::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);
    if(stage == 0){
        traci = TraCIMobilityAccess().get(getParentModule());
        annotations = AnnotationManagerAccess().getIfExists();
        ASSERT(annotations);

        sentMessage = false;
        if(sendBeaconEvt->isScheduled()){
            cancelEvent(sendBeaconEvt);
        }
        lastDroveAt = simTime();
    }
}

void TestAppDemo::onBeacon(WaveShortMessage* wsm) {
}

void TestAppDemo::onData(WaveShortMessage* wsm) {
    findHost()->getDisplayString().updateWith("r=16,green");
    annotations->scheduleErase(1, annotations->drawLine(wsm->getSenderPos(), traci->getPositionAt(simTime()), "blue"));

    if(traci->getRoadId()[0] != ':')
        traci->commandChangeRoute(wsm->getWsmData(), 9999);
    if(!sentMessage)
        sendMessage(wsm->getWsmData());
}

void TestAppDemo::sendMessage(std::string blockedRoadId) {
    sentMessage = true;

    t_channel channel = dataOnSch ? type_SCH : type_CCH;
    WaveShortMessage* wsm = prepareWSM("data", dataLengthBits, channel, dataPriority, -1, 2);
    wsm->setWsmData(blockedRoadId.c_str());
    sendWSM(wsm);
}

void TestAppDemo::handlePositionUpdate(cObject* obj) {
    BaseWaveApplLayer::handlePositionUpdate(obj);

    /*
     // stopped for for at least 10s?
     if(traci->getSpeed() < 1){
     if(simTime() - lastDroveAt >= 10){
     findHost()->getDisplayString().updateWith("r=16,red");
     if(!sentMessage)
     sendMessage(traci->getRoadId());
     }
     }else{
     lastDroveAt = simTime();
     }
     */
}
