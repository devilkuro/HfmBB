/*
 * ACO.h
 *
 *  Created on: 2015-1-4
 *      Author: bobo
 */

#ifndef ACO_H_
#define ACO_H_
#include <map>
#include<string>
#include "AntNode.h"
#include <list>
#include <stdlib.h>
#include <BaseSeekRouteFunc.h>
using namespace std;

class ACO :public BaseSeekRouteFunc{
public:
	static map<AntNode*, list<AntNode*> > roadMap; //路网邻接表
	static double ALPHA; // 启发因子，信息素的重要程度
	static double BETA; // 期望因子，城市间距离的重要程度
	static double ROU; // 信息素残留参数
	static double P; // 伪概率事件
	static int N_ANT_COUNT; // 蚂蚁数量
	static int N_IT_COUNT; // 迭代次数
	static double DBQ; // 总的信息量
	static double DB_MAX; //一个标志数，10的9次方
	static double DB_MIN;
	void setRoadMap(double pre_trial);
	AntNode* getAntNode(string edgeName); // 通过名称获取道路
	ACO();
	list<string> getAllEdges();
	list<string> getNextEdges(string edge);
	double getEdgeLength(string edge);
	int getVehicleNum(string edge);
	double getTravelTime(string edge);
	void setRoute(list<string> route);
	virtual ~ACO();
};
#endif /* ACO_H_ */
