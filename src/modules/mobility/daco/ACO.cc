/*
 * ACO.cpp
 *
 *  Created on: 2015-1-4
 *      Author: bobo
 */

#include "ACO.h"

double ACO::ALPHA = 1.0;
double ACO::BETA = 2.0;
double ACO::ROU = 0.5;
double ACO::P = 0.2;
double ACO::DB_MAX = 10e9; //一个标志数，10的9次方
double ACO::DB_MIN = 2.2250738585072014e-308;
int ACO::N_ANT_COUNT = 10;
int ACO::N_IT_COUNT = 100;
double ACO::DBQ = 100.0;
map<AntNode*, list<AntNode*> > ACO::roadMap;
ACO::ACO() {
}

void ACO::setRoadMap(double pre_trial) {
	AntNode *road1 = new AntNode("1", 0.1, 1.0, 1.0, pre_trial);
	AntNode *road2 = new AntNode("2", 0.2, 1.0, 2.0, pre_trial);
	AntNode *road3 = new AntNode("3", 0.2, 2.0, 1.0, pre_trial);
	AntNode *road4 = new AntNode("4", 0.2, 4.0, 1.0, pre_trial);
	AntNode *road5 = new AntNode("5", 0.2, 2.0, 3.0, pre_trial);
	AntNode *road6 = new AntNode("6", 0.2, 3.0, 2.0, pre_trial);
	AntNode *road7 = new AntNode("7", 0.2, 5.0, 2.0, pre_trial);
	AntNode *road8 = new AntNode("8", 0.2, 1.0, 4.0, pre_trial);
	AntNode *road9 = new AntNode("9", 0.2, 3.0, 4.0, pre_trial);
	AntNode *road10 = new AntNode("10", 0.2, 5.0, 4.0, pre_trial);
	list<AntNode*> temp1;
	temp1.push_back(road2);
	temp1.push_back(road3);
	roadMap[road1] = temp1;
	list<AntNode*> temp2;
	temp2.push_back(road8);
	roadMap[road2] = temp2;
	list<AntNode*> temp3;
	temp3.push_back(road4);
	temp3.push_back(road5);
	temp3.push_back(road6);
	roadMap[road3] = temp3;
	list<AntNode*> temp4;
	temp4.push_back(road7);
	roadMap[road4] = temp4;
	list<AntNode*> temp5;
	temp5.push_back(road9);
	roadMap[road5] = temp5;
	list<AntNode*> temp6;
	temp6.push_back(road10);
	roadMap[road6] = temp6;
	list<AntNode*> temp7;
	roadMap[road7] = temp7;
	list<AntNode*> temp8;
	roadMap[road8] = temp8;
	list<AntNode*> temp9;
	temp9.push_back(road10);
	roadMap[road9] = temp9;
	list<AntNode*> temp10;
	roadMap[road10] = temp10;
}
// 通过名称获取道路
AntNode* ACO::getAntNode(string edgeName) {
	AntNode *result;
	map<AntNode*, list<AntNode*> >::iterator iter;
	for (iter = roadMap.begin(); iter != roadMap.end(); iter++) {
		AntNode *temp = iter->first;
		if (temp->getRoadName() == edgeName) {
			result = iter->first;
			break;
		}
	}
	return result;
}
//获取所有路网的边
list<string> ACO::getAllEdges() {
	list<string> allEdges;
	allEdges.push_back("1");
	allEdges.push_back("2");
	allEdges.push_back("3");
	allEdges.push_back("4");
	allEdges.push_back("5");
	allEdges.push_back("6");
	allEdges.push_back("7");
	allEdges.push_back("8");
	allEdges.push_back("9");
	allEdges.push_back("10");
	allEdges.push_back("11");
	return allEdges;
}

list<string> ACO::getNextEdges(string edge) {
	list<string> edges;
	if (edge == "1") {
		edges.push_back("2");
	} else if (edge == "2") {
		edges.push_back("3");
	} else if (edge == "5") {
		edges.push_back("6");
		edges.push_back("7");
		edges.push_back("8");
	} else if (edge == "8") {
		edges.push_back("9");
		edges.push_back("10");
		edges.push_back("11");
	}
	return edges;
}

double ACO::getEdgeLength(string edge) {
	double array[] = { 3.0, 4.0, 2.0, 1.0, 4.0, 2.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
	return array[atoi(edge.c_str()) - 1];
}
//未实现
int ACO::getVehicleNum(string edge) {
	return 0;
}
//未实现
double ACO::getTravelTime(string edge) {
	return 0.0;
}
//未实现
void ACO::setRoute(list<string> route) {
}

ACO::~ACO() {

}
