/*
 * BaseSeekRouteFunc.h
 *
 *  Created on: 2015Äê1ÔÂ19ÈÕ
 *      Author: Fanjing-LAB
 */

#ifndef BASESEEKROUTEFUNC_H_
#define BASESEEKROUTEFUNC_H_
#include <string>
#include <list>
#include "GlobalMapSystem.h"

using namespace std;
class BaseSeekRouteFunc {
public:
    list<string> seleted_route;

    BaseSeekRouteFunc();
    virtual ~BaseSeekRouteFunc();

    virtual void init(GlobalMapSystem* gms);

    virtual list<string> getAllEdges();
    virtual list<string> getNextEdges(string edge);
    virtual double getEdgeLength(string edge);
    virtual int getVehicleNum(string edge);
    virtual double getTravelTime(string edge);
    virtual void setRoute(list<string> route);
private:
    GlobalMapSystem* gms;
};

#endif /* BASESEEKROUTEFUNC_H_ */
