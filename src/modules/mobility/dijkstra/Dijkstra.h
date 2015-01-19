/*
 * Dijkstra.h
 *
 *  Created on: 2014-12-30
 *      Author: bobo
 */

#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_
#include "Node.h"
#include <map>
#include "../utils.h"
#include <vector>
#include<string>
#include <stdlib.h>
using namespace std;
class Dijkstra {
private:
	map<Node*, vector<Node*> > roadMap;
	vector<Node*> openList;
	vector<Node*> closeList;
	void displayCost(Node *sNode, Node *eNode); //计算代价函数
	void init(Node *sNode);
	Node* getShortestPath(); //获取最小代价的点
	Node* getNode(string edgeName); // 通过名称获取道路
public:
	Dijkstra();
	bool contains(vector<Node*> vec, Node *res);
	void setRoadMap(); //构建路网
	void search(string startRoad, string endRoad); //搜索函数
	void dijkstra(Node *sNode, Node *eNode); //核心函数
	virtual ~Dijkstra();
};

#endif /* DIJKSTRA_H_ */
