/*
 * GlobalStaticsUnit.cc
 *
 *  Created on: 1405051354
 *      Author: Fanjing
 */

#include "GlobalStatisticsUnit.h"
#include <sstream>

GlobalStatisticsUnit::GlobalStatisticsUnit(int size) {
    this->size = size;
    data = new DataUnit[size];
}

GlobalStatisticsUnit::~GlobalStatisticsUnit() {
    size = 0;
    delete(data);
    data = NULL;
}

void GlobalStatisticsUnit::setData(double data, int index) {
    if (index >= 0 && index < size) {
        this->data[index].type = UNIT_TYPE_DOUBLE;
        this->data[index].douData = data;
    }
}


int GlobalStatisticsUnit::getSize() const {
    return size;
}

void GlobalStatisticsUnit::setData(int data, int index) {
    if (index >= 0 && index < size) {
        this->data[index].type = UNIT_TYPE_INT;
        this->data[index].intData = data;
    }
}

void GlobalStatisticsUnit::setData(unsigned int data, int index) {
    if (index >= 0 && index < size) {
        this->data[index].type = UNIT_TYPE_UINT;
        this->data[index].uintData = data;
    }
}

void GlobalStatisticsUnit::setData(string data, int index) {
    if (index >= 0 && index < size) {
        this->data[index].type = UNIT_TYPE_STRING;
        this->data[index].strData = data;
    }
}

int GlobalStatisticsUnit::getDataType(int index) {
    if (index >= 0 && index < size) {
        return data[index].type;
    }else{
        return UNIT_TYPE_ERROR;
    }
}

string GlobalStatisticsUnit::toString() {
    stringstream ss;
    for(int i = 0;i<size;i++){
        if(i!=0)
            ss<<",";
        ss<<data[i].toString();
    }
    return ss.str();
}

string GlobalStatisticsUnit::DataUnit::toString() {
    stringstream ss;
    switch (type) {
        case UNIT_TYPE_INT:
            ss<<intData;
            break;
        case UNIT_TYPE_UINT:
            ss<<intData;
            break;
        case UNIT_TYPE_DOUBLE:
            ss<<douData;
            break;
        case UNIT_TYPE_STRING:
            ss<<strData;
            break;
        default:
            break;
    }
    return ss.str();
}
