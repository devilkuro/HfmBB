/*
 * SMTCarInfo.cc
 *
 *  Created on: Oct 7, 2015
 *      Author: Fanjing-LAB
 */

#include "SMTCarInfo.h"

namespace Fanjing {
string SMTCarInfo::path = "";
XMLDocument* SMTCarInfo::doc = NULL;
map<string, SMTCarInfo> SMTCarInfo::vTypeMap;
SMTCarInfo::SMTCarInfo() {
    // TODO Auto-generated constructor stub
    id = "";
    type = SMTCARINFO_ROUTETYPE_LAST_TYPE;
    origin = "";
    destination = "";
    loop = "";
    time = -1;
    string vtype = "";

    // type related
    accel = -1;
    decel = -1;
    sigma = -1;
    length = -1;
    minGap = -1;
    maxSpeed = -1;
    color = "";   // useless
}

SMTCarInfo::~SMTCarInfo() {
    // TODO Auto-generated destructor stub
}

void SMTCarInfo::loadVehicleTypeXML(string path) {
    if(doc != NULL){
        vTypeMap.clear();
    }else{
        doc = new XMLDocument();
    }
    XMLError e = doc->LoadFile(path.c_str());
    if(e == XML_SUCCESS){
        XMLElement* root = doc->FirstChildElement("routes");
        if (root!=NULL) {
            XMLElement* e = root->FirstChildElement("vType");
            SMTCarInfo car;
            while(e){
                if(NULL != e->Attribute("id")){
                    car.id = e->Attribute("id");
                    car.accel = e->DoubleAttribute("accel");
                    car.decel = e->DoubleAttribute("decel");
                    car.sigma = e->DoubleAttribute("sigma");
                    car.length = e->DoubleAttribute("length");
                    car.minGap = e->DoubleAttribute("minGap");
                    car.maxSpeed = e->DoubleAttribute("maxSpeed");
                    car.color = e->Attribute("color");
                    vTypeMap[e->Attribute("id")] = car;
                }
                e = e->NextSiblingElement("vType");
            }
        }
    }
    doc->Clear();
}

SMTCarInfo SMTCarInfo::getDefaultVeicleTypeInfo(string vTypeId) {
    if(vTypeMap.find(vTypeId) != vTypeMap.end()){
        return vTypeMap[vTypeId];
    }
    return SMTCarInfo();
}

bool SMTCarInfo::hasInitialized() {
    // todo this is a simple method
    return vTypeMap.size()>0;
}

list<string> SMTCarInfo::getDefaultVeicleTypeList() {
    list<string> result;
    for(map<string, SMTCarInfo>::iterator it = vTypeMap.begin();
            it != vTypeMap.end(); it++){
        result.push_back(it->first);
    }
    return result;
}

} /* namespace Fanjing */

