/*
 * DStar.h
 *
 *  Created on: 2015-1-8
 *      Author: bobo
 */

#ifndef DSTAR_H_
#define DSTAR_H_
#include "DNode.h"
#include <map>
#include <string>
#include <list>
#include "../utils.h"
#include "DNodeUtil.h"
using namespace std;

class DStar {
private:
	map<DNode*, list<DNode*> > roadMap; // 当前街道可以连通的街道map
	map<DNode*, list<DNode*> > reverseRoadMap; // 前向集合
	list<DNode*> openList; // 开启列表
	list<DNode*> closeList; // 关闭列表
	list<DNode*> uPriority; // 优先队列
	list<DNode*> key_node;
	void initRoadMap(); // 初始化路网
	list<DNode*> get_reverse_node(DNode *node);//获取前向集合
	void initRoad(DNode *sNode, DNode *eNode); //开始搜索最优路径初始化
	void search(DNode *eNode, DNode *sNode); //第一阶段规划出最优路径
	void updateRoad(DNode *paramNode, DNode *eNode); //当路况发生变化时 更新rhs
	void display(DNode *sNode, DNode *eNode); //打印一次规划后的最优路径
	double getDistance(DNode *node, DNode *eNode); // 获取街道的长度
	void getPath(list<DNode*> *resultList, DNode *node);
	DNode* getDNode(string edgeName); // 通过名称获取道路
public:
	DStar();
	void setRoadMap(); // 构造地图map
	void setReverseRoadMap(); // 构造反向路网map
	void move(string startRoad, string endRoad); //开始沿着最优路径移动
	virtual ~DStar();
};

#endif /* DSTAR_H_ */
