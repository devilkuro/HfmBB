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

#include "GlobalMobilityLaunchd.h"

Define_Module(GlobalMobilityLaunchd)

GlobalMobilityLaunchd::~GlobalMobilityLaunchd() {
}

std::list<std::string> GlobalMobilityLaunchd::commandGetLaneLinksIds(std::string laneId) {
    return laneLinksGetStringList(CMD_GET_LANE_VARIABLE, laneId, LANE_LINKS, RESPONSE_GET_LANE_VARIABLE);
}

void GlobalMobilityLaunchd::commandAddRoute(std::string routeId, std::list<std::string> route) {
    uint8_t variableId = ADD;
    uint8_t variableType = TYPE_STRINGLIST;
    int32_t count = route.size();
    TraCIBuffer buf = TraCIBuffer();
    buf << variableId << routeId << variableType << count;
    for(std::list<std::string>::iterator it = route.begin(); it != route.end(); it++){
        buf << (*it);
    }
    TraCIBuffer buf0 = queryTraCI(CMD_SET_ROUTE_VARIABLE, buf);
    ASSERT(buf0.eof());
}

void GlobalMobilityLaunchd::commandSetRouteByEdgeList(std::string nodeId, std::list<std::string> route) {
    uint8_t variableId = VAR_ROUTE;
    uint8_t variableType = TYPE_STRINGLIST;
    int32_t count = route.size();
    TraCIBuffer buf = TraCIBuffer();
    buf << variableId << nodeId << variableType << count;
    for(std::list<std::string>::iterator it = route.begin(); it != route.end(); it++){
        buf << (*it);
    }
    TraCIBuffer buf0 = queryTraCI(CMD_SET_VEHICLE_VARIABLE, buf);
    ASSERT(buf0.eof());
}

uint32_t GlobalMobilityLaunchd::getActiveVehicleCount() {
    return activeVehicleCount;
}

void GlobalMobilityLaunchd::commandSetLaneChangeMode(std::string nodeId, uint32_t bitset) {
    uint8_t variableId = VAR_LANECHANGE_MODE;
    uint8_t variableType = TYPE_INTEGER;
    TraCIBuffer buf = queryTraCI(CMD_SET_VEHICLE_VARIABLE,
            TraCIBuffer() << variableId << nodeId << variableType << bitset);
    ASSERT(buf.eof());
}

std::string GlobalMobilityLaunchd::getLaunchConfigXMLPath() {
    return hasPar("launchConfig") ? par("launchConfig").str().substr(14, par("launchConfig").str().find(':') - 14) : "";
}

std::string GlobalMobilityLaunchd::getRouXMLFromLaunchConfig(std::string launchFilePath) {
    return getXXXXMLFromLaunchConfig(launchFilePath,"rou.xml");
}

std::string GlobalMobilityLaunchd::getXXXXMLFromLaunchConfig(std::string launchFilePath, std::string endStr) {
    tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
    if(doc != NULL){
        doc->LoadFile(launchFilePath.c_str());
        tinyxml2::XMLElement* root = doc->FirstChildElement("launch");
        if(root != NULL){
            tinyxml2::XMLElement* e = root->FirstChildElement("copy");
            while(e){
                if(e->Attribute("file") != NULL){
                    std::string file = e->Attribute("file");
                    // if the file is end with "rou.xml", it is the RouXML file.
                    if(file.rfind(endStr) == file.length() - std::string(endStr).length()){
                        return file;
                    }
                }
                e = e->NextSiblingElement("copy");
            }
        }
    }
    return "";
}

std::list<std::string> GlobalMobilityLaunchd::laneLinksGetStringList(uint8_t commandId, std::string objectId,
        uint8_t variableId, uint8_t responseId) {
    uint8_t resultTypeId = TYPE_COMPOUND;
    std::list<std::string> res;

    TraCIBuffer buf = queryTraCI(commandId, TraCIBuffer() << variableId << objectId);

    uint8_t cmdLength;
    buf >> cmdLength;
    if(cmdLength == 0){
        uint32_t cmdLengthX;
        buf >> cmdLengthX;
    }
    uint8_t commandId_r;
    buf >> commandId_r;
    ASSERT(commandId_r == responseId);
    uint8_t varId;
    buf >> varId;
    ASSERT(varId == variableId);
    std::string objectId_r;
    buf >> objectId_r;
    ASSERT(objectId_r == objectId);
    uint8_t resType_r;
    buf >> resType_r;
    ASSERT(resType_r == resultTypeId);
    uint32_t count;
    buf >> count;
    uint8_t numType;
    buf >> numType;
    ASSERT(numType == TYPE_INTEGER);
    uint32_t linkNumber;
    buf >> linkNumber;
    for(uint32_t i = 0; i < linkNumber; i++){
        // external id
        // consecutive not internal lane
        {
            uint8_t typeId;
            buf >> typeId;
            ASSERT(typeId == TYPE_STRING);
            std::string str;
            buf >> str;
            res.push_back(str);
        }
        // internal id
        // consecutive internal lane
        {
            uint8_t typeId;
            buf >> typeId;
            ASSERT(typeId == TYPE_STRING);
            std::string str;
            buf >> str;
        }
        // has priority (=1) or not (=0)
        {
            uint8_t typeId;
            buf >> typeId;
            ASSERT(typeId == TYPE_UBYTE);
            uint8_t u;
            buf >> u;
        }
        // is opened (=1) or not (=0)
        {
            uint8_t typeId;
            buf >> typeId;
            ASSERT(typeId == TYPE_UBYTE);
            uint8_t u;
            buf >> u;
        }
        // has approaching foe (=1) or not (=0)
        {
            uint8_t typeId;
            buf >> typeId;
            ASSERT(typeId == TYPE_UBYTE);
            uint8_t u;
            buf >> u;
        }
        // (current) state (not yet implemented, always="")
        {
            uint8_t typeId;
            buf >> typeId;
            ASSERT(typeId == TYPE_STRING);
            std::string str;
            buf >> str;
        }
        // direction (not yet implemented, always="")
        {
            uint8_t typeId;
            buf >> typeId;
            ASSERT(typeId == TYPE_STRING);
            std::string str;
            buf >> str;
        }
        // length [m] - only valid if not using internal lanes
        {
            uint8_t typeId;
            buf >> typeId;
            ASSERT(typeId == TYPE_DOUBLE);
            double d;
            buf >> d;
        }
    }

    ASSERT(buf.eof());

    return res;
}
