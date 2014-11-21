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

#include "GlobalStatistics.h"
#include "ctime"
#include <fstream>
#include <iostream>

Define_Module(GlobalStatistics)

GlobalStatistics::gs_eofType GlobalStatistics::endl = NULL;
void GlobalStatistics::initialize() {
    // do nothing
}

void GlobalStatistics::handleMessage(cMessage *msg) {
    // do nothing
}

GlobalStatistics::GlobalStatistics() {
    // do nothing
}

GlobalStatistics::~GlobalStatistics() {
    for(GlobalStatisticsMap::iterator it = globalStatisticsMap.begin(); it != globalStatisticsMap.end(); it++){
        for(GlobalStatisticsList::iterator lit = it->second->begin(); lit != it->second->end(); lit++){
            delete (*lit);
        }
        delete (it->second);
    }
    globalStatisticsMap.clear();
}

void GlobalStatistics::finish() {
    ostringstream name;
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    name.width(2);
    name << "result_" << timeinfo->tm_year << "_" << timeinfo->tm_mon << "_" << timeinfo->tm_mday << "_"
            << timeinfo->tm_hour << "_" << timeinfo->tm_min << "_" << timeinfo->tm_sec << ".txt";
    output(name.str());
}

GlobalStatistics& GlobalStatistics::operator <<(gs_eofType& e) {
    if(unitData.size() > 0){
        GlobalStatisticsUnit* unit = new GlobalStatisticsUnit(unitData.size());
        int i = 0;
        for(std::list<GlobalStatisticsUnit::DataUnit>::iterator it = unitData.begin(); it != unitData.end(); it++){
            switch ((*it).type) {
                case GlobalStatisticsUnit::UNIT_TYPE_INT:
                    unit->setData((*it).intData, i);
                    break;
                case GlobalStatisticsUnit::UNIT_TYPE_DOUBLE:
                    unit->setData((*it).douData, i);
                    break;
                case GlobalStatisticsUnit::UNIT_TYPE_STRING:
                    unit->setData((*it).strData, i);
                    break;
                default:
                    break;
            }
            i++;
        }
        GlobalStatisticsMap::iterator it;
        it = globalStatisticsMap.find(m_name);
        if(it == globalStatisticsMap.end()){
            GlobalStatisticsList* list = new GlobalStatisticsList();
            globalStatisticsMap[m_name] = list;
        }
        globalStatisticsMap[m_name]->push_back(unit);
        unitData.clear();
    }
    return *this;
}

GlobalStatistics& GlobalStatistics::operator <<(double num) {
    GlobalStatisticsUnit::DataUnit unit;
    unit.type = GlobalStatisticsUnit::UNIT_TYPE_DOUBLE;
    unit.douData = num;
    unitData.push_back(unit);
    return *this;
}

GlobalStatistics& GlobalStatistics::changeName(string name) {
    GlobalStatisticsMap::iterator it;
    m_name = name;
    it = globalStatisticsMap.find(m_name);
    if(it == globalStatisticsMap.end()){
        GlobalStatisticsList* list = new GlobalStatisticsList();
        globalStatisticsMap[m_name] = list;
    }
    unitData.clear();
    return *this;
}

GlobalStatistics& GlobalStatistics::operator <<(string str) {
    GlobalStatisticsUnit::DataUnit unit;
    unit.type = GlobalStatisticsUnit::UNIT_TYPE_STRING;
    unit.strData = str;
    unitData.push_back(unit);
    return *this;
}

GlobalStatistics& GlobalStatistics::operator <<(int num) {
    GlobalStatisticsUnit::DataUnit unit;
    unit.type = GlobalStatisticsUnit::UNIT_TYPE_INT;
    unit.intData = num;
    unitData.push_back(unit);
    return *this;
}
GlobalStatistics& GlobalStatistics::operator <<(unsigned int num) {
    GlobalStatisticsUnit::DataUnit unit;
    unit.type = GlobalStatisticsUnit::UNIT_TYPE_INT;
    unit.intData = num;
    unitData.push_back(unit);
    return *this;
}

void GlobalStatistics::output(string name) {
    std::fstream fs;
    fs.open(name.c_str(), std::fstream::out);
    for(GlobalStatisticsMap::iterator it = globalStatisticsMap.begin(); it != globalStatisticsMap.end(); it++){
        for(GlobalStatisticsList::iterator lit = it->second->begin(); lit != it->second->end(); lit++){
            fs << it->first << "," << (*lit)->toString() << std::endl;
        }
    }
    fs.close();
}

GlobalStatistics& GlobalStatistics::get() {
    return *this;
}

void GlobalStatistics::eof() {
}
