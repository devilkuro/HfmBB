/*
 * GlobalStaticsUnit.h
 *
 *  Created on: 1405051354
 *      Author: Fanjing
 */

#ifndef GLOBALSTATISTICSUNIT_H_
#define GLOBALSTATISTICSUNIT_H_

#include <string>
using namespace std;
/*
 *
 */
class GlobalStatisticsUnit {
public:
    GlobalStatisticsUnit(int size);
    virtual ~GlobalStatisticsUnit();
public:
    enum UnitType {
        UNIT_TYPE_ERROR = -1,
        UNIT_TYPE_INT = 0,
        UNIT_TYPE_DOUBLE,
        UNIT_TYPE_STRING
    };
    struct DataUnit {
        UnitType type;
        union {
            int intData;
            double douData;
        };
        string strData;
        string toString();
    };
public:
    int getDataType(int index);
    void setData(double data, int index);
    void setData(int data, int index);
    void setData(string data, int index);
    int getSize() const;
    string toString();

private:
    int size;       // read only, it must be set at initialize phase.
    DataUnit *data;   // statistics data
};

#endif /* GLOBALSTATISTICSUNIT_H_ */
