/*
 * BaseSeekRouteFunc.cc
 *
 *  Created on: 2015Äê1ÔÂ19ÈÕ
 *      Author: Fanjing-LAB
 */

#include <BaseSeekRouteFunc.h>

BaseSeekRouteFunc::BaseSeekRouteFunc() {
    this->gms = NULL;
}

BaseSeekRouteFunc::~BaseSeekRouteFunc() {
    // do nothing here
}

list<string> BaseSeekRouteFunc::getAllEdges() {
    return gms->getAllEdges();
}

list<string> BaseSeekRouteFunc::getNextEdges(string edge) {
    return gms->getNextEdges(edge);
}

double BaseSeekRouteFunc::getEdgeLength(string edge) {
    return gms->getEdgeLength(edge);
}

int BaseSeekRouteFunc::getVehicleNum(string edge) {
    return gms->getVehicleNumByEdge(edge);
}

double BaseSeekRouteFunc::getTravelTime(string edge) {
    return gms->getAvgTravelTimeByEdge(edge);
}

void BaseSeekRouteFunc::init(GlobalMapSystem* gms) {
    this->gms = gms;
    // todo build, search.
}

void BaseSeekRouteFunc::setRoute(list<string> route) {
    this->seleted_route = route;
}
