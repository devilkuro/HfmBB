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

list<BaseSeekRouteFunc::BSRF_Coord> BaseSeekRouteFunc::getShapeByEdge(string edge) {
    list<Coord> shape = this->gms->getEdgeShape(edge);
    list<BSRF_Coord> result;
    for(list<Coord>::iterator it = shape.begin(); it!=shape.end();it++){
        BSRF_Coord pos;
        pos.x = it->x;
        pos.y = it->y;
        result.push_back(pos);
    }
    return result;
}

void BaseSeekRouteFunc::seekRoute(string startRoad, string endRoad) {
    // do nothing in base
}

void BaseSeekRouteFunc::setRoute(list<string> route) {
    this->seleted_route = route;
}
