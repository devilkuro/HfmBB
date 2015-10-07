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

#ifndef CARFLOWGENERATOR_H_
#define CARFLOWGENERATOR_H_
#include <string>
#include <list>
#include <iostream>
#include "tinyxml2.h"
#include "SMTCarInfo.h"
#include "StringHelper.h"

namespace Fanjing {

using namespace std;
using namespace tinyxml2;

class CarFlowGenerator {
public:
    CarFlowGenerator();
    virtual ~CarFlowGenerator();

    bool addODCar(string id, string origin, string destination, double time, string vtype);
    bool addLoopCar(string id, list<string> loop, double time, string vtype);

    list<string> getAllCars();
    string getRouteTypeOfCar(string id);
    string getOriginOfODCar(string id);
    string getDestinationOfODCar(string id);
    list<string> getLoopOfLoopCar(string id);
    string getCarTypeOFCar(string id);
    double getDepartTimeOfCar(string id);

    list<string> switchRouteToRoadList(string route);
    string switchRoadListToRoute(list<string> roadlist);

    bool setXMLPath(string path);
    bool loadXML(string path);
    void clear(bool save = false);
    void save(string path = "");

    // other
    void setPrecisionOfTime(int precision);
protected:
    XMLElement* root;
    XMLDocument* doc;
    string path;
    bool notSaved;

    int precisionOfTime;

    XMLElement* seekChildElementByAttribute(string name, string value);
    list<string> splitStringToWordsList(string str);
    void finish();
};

class CarInfo{

};

} /* namespace Fanjing */
#endif /* CARFLOWGENERATOR_H_ */
