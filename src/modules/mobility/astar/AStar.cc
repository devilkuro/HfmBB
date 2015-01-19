/*
 * AStar.cpp
 *
 *  Created on: 2015-1-4
 *      Author: bobo
 */

#include "AStar.h"

AStar::AStar() {
}
//构建临时地图
void AStar::setRoadMap() {
	ANode *road1 = new ANode("1", 1.0, 1.0, 1.2, NULL);
	ANode *road2 = new ANode("2", 1.0, 2.0, 1.7, NULL);
	ANode *road3 = new ANode("3", 2.0, 2.0, 2.3, NULL);
	ANode *road4 = new ANode("4", 3.0, 2.0, 1.0, NULL);
	ANode *road5 = new ANode("5", 2.0, 3.0, 4.7, NULL);
	ANode *road6 = new ANode("6", 3.0, 4.0, 3.8, NULL);
	ANode *road7 = new ANode("7", 4.0, 2.0, 2.8, NULL);
	ANode *road8 = new ANode("8", 2.0, 4.0, 1.8, NULL);
	ANode *road9 = new ANode("9", 1.0, 3.0, 2.4, NULL);
	ANode *road10 = new ANode("10", 4.0, 4.0, 1.3, NULL);
	ANode *road11 = new ANode("11", 3.0, 5.0, 1.9, NULL);
	list<ANode*> temp1;
	temp1.push_back(road2);
	temp1.push_back(road3);
	roadMap[road1] = temp1;
	list<ANode*> temp2;
	temp2.push_back(road9);
	roadMap[road2] = temp2;
	list<ANode*> temp3;
	temp3.push_back(road4);
	temp3.push_back(road5);
	//temp3.push_back(road10);
	roadMap[road3] = temp3;
	list<ANode*> temp4;
	temp4.push_back(road7);
	roadMap[road4] = temp4;
	list<ANode*> temp5;
	temp5.push_back(road6);
	temp5.push_back(road8);
	roadMap[road5] = temp5;
	list<ANode*> temp6;
	temp6.push_back(road10);
	temp6.push_back(road11);
	roadMap[road6] = temp6;
	list<ANode*> temp7;
	temp7.push_back(road10);
	roadMap[road7] = temp7;
	list<ANode*> temp8;
	roadMap[road8] = temp8;
	list<ANode*> temp9;
	roadMap[road9] = temp9;
	list<ANode*> temp10;
	roadMap[road10] = temp10;
	list<ANode*> temp11;
	roadMap[road11] = temp11;
}
list<ANode*>* AStar::search(ANode *sNode, ANode *eNode) {
	list<ANode*> *result = new list<ANode*>();
	bool isFind = false;
	ANode *node = NULL;
	while (openList.size() > 0) {
		// 取出开启列表中最低F值，即第一个存储的值的F为最低的
		node = openList.front();
		// 判断是否找到目标点
		if (node->getRoadName() == eNode->getRoadName()) {
			isFind = true;
			break;
		}
		list<ANode*> allowedRoad = roadMap[node];
		for (unsigned int i = 0; i < closeList.size(); i++) {
			if (contains(allowedRoad, getANodeElem(closeList, i))) {
				allowedRoad.remove(getANodeElem(closeList, i));
			}
		}
		for (unsigned int i = 0; i < allowedRoad.size(); i++) {
			ANode *temp = getANodeElem(allowedRoad, i);
			double cost = temp->getRoadWeight();
			checkPath(temp, node, eNode, cost);
		}
		// 从开启列表中删除
		// 添加到关闭列表中
		ANode *lastElem = openList.front();
		closeList.push_back(lastElem);
		openList.pop_front();
		openList.sort(compA);		// 开启列表中排序，把F值最低的放到最底端
	}
	if (isFind) {
		getPath(result, node);
		display(*result);
	}
	return result;
}
// 集合中是否包含某个元素(-1：没有找到，否则返回所在的索引)
int AStar::isListContains(list<ANode*> li, double x, double y) {
	list<ANode*>::iterator iter;
	int i = 0;
	for (iter = li.begin(); iter != li.end(); iter++) {
		ANode *temp = *iter;
		if (temp->getxPos() == x && temp->getyPos() == y) {
			return i;
		} else {
			i++;
		}
	}
	return -1;
}
// 查询此路是否能走通
bool AStar::checkPath(ANode *node, ANode *parentNode, ANode *eNode,
		double cost) {
	list<ANode*> allowedRoad = roadMap[node];
	if (allowedRoad.size() == 0
			&& node->getRoadName() != eNode->getRoadName()) { //还需要改
		closeList.push_back(node);
		return false;
	}
	// 查找开启列表中是否存在
	int index = -1;
	if ((index = isListContains(openList, node->getxPos(), node->getyPos()))
			!= -1) {
		// G值是否更小，即是否更新G，F值
		if ((parentNode->getG() + cost)
				< getANodeElem(openList, index)->getG()) {
			node->setParentNode(parentNode);
			countG(node, eNode);
			countF(node);
			int i = 0;
			for (list<ANode*>::iterator it = openList.begin();
					it != openList.begin(); it++) {
				if (i == index) {
					openList.insert(it, 1, node);
					break;
				} else {
					i++;
				}
			}
		}
	} else {
		// 添加到开启列表中
		node->setParentNode(parentNode);
		count(node, eNode);
		openList.push_back(node);
	}
	return true;
}
// 从终点往返回到起点
void AStar::getPath(list<ANode*> *resultList, ANode *node) {
	if (node->getParentNode() != NULL) {
		getPath(resultList, node->getParentNode());
	}
	resultList->push_back(node);
}

void AStar::display(list<ANode*> resultList) {
	double totalCost = 0.0;
	list<ANode*>::iterator iter;
	for (iter = resultList.begin(); iter != resultList.end(); iter++) {
		double temp = (*iter)->getRoadWeight();
		totalCost += temp;
		cout << (*iter)->getRoadName() << " ";
	}
	cout << endl;
	cout << "cost:" << totalCost << endl;
}
//A*开始搜索
void AStar::search(string startRoad, string endRoad) {
	ANode *sNode = getANode(roadMap, startRoad);
	ANode *eNode = getANode(roadMap, endRoad);
	openList.push_back(sNode);
	list<ANode*>* rst = search(sNode, eNode);
	delete rst;
}
//析构函数
AStar::~AStar() {
}

