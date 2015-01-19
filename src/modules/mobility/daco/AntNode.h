/*
 * AntNode.h
 *
 *  Created on: 2014-12-30
 *      Author: bobo
 */

#ifndef ANTNODE_H_
#define ANTNODE_H_
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;
class AntNode {
private:
	string roadName; //道路名称
	int count; // 记录街道被蚂蚁走过的次数
	double xPos; // 道路横坐标
	double yPos; // 道路纵坐标
	double roadWeight; // 道路权值
	double pheromone; // 道路信息素
public:
	AntNode();
	AntNode(string roadName, double roadWeight, double xPos, double yPos,
			double pheromone);
	int getCount();
	void setCount(int count);
	double getxPos();
	void setxPos(double xPos);
	double getyPos();
	void setyPos(double yPos);
	string getRoadName();
	void setRoadName(string roadName);
	double getRoadWeight();
	void setRoadWeight(double roadWeight);
	double getPheromone();
	void setPheromone(double pheromone);
	virtual ~AntNode();
};

#endif /* ANTNODE_H_ */
