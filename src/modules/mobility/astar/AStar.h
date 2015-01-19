/*
 * AStar.h
 *
 *  Created on: 2015-1-4
 *      Author: bobo
 */

#ifndef ASTAR_H_
#define ASTAR_H_
#include <string>
#include <map>
#include <list>
#include "ANode.h"
#include "ANodeUtil.h"
using namespace std;
class AStar {
private:
	map<ANode*, list<ANode*> > roadMap; // 当前街道可以连通的街道map
	list<ANode*> openList; // 开启列表
	list<ANode*> closeList; // 关闭列表
public:
	AStar();
	list<ANode*>* search(ANode *sNode, ANode *eNode);
	void search(string startRoad, string endRoad);
	void setRoadMap();
	int isListContains(list<ANode*> list, double x, double y);
	bool checkPath(ANode *node, ANode *parentNode, ANode *eNode, double cost);
	void getPath(list<ANode*> *resultList, ANode *node);
	void display(list<ANode*> resultList);
	virtual ~AStar();
};

#endif /* ASTAR_H_ */
