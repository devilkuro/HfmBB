/*
 * DStar.cpp
 *
 *  Created on: 2015-1-8
 *      Author: bobo
 */

#include "DStar.h"
DStar::DStar() {

}
// 构造地图map
void DStar::setRoadMap() {
	DNode *road1 = new DNode("1", 1.0, 1.0, 1.2, NULL);
	DNode *road2 = new DNode("2", 1.0, 2.0, 1.7, NULL);
	DNode *road3 = new DNode("3", 2.0, 2.0, 2.3, NULL);
	DNode *road4 = new DNode("4", 3.0, 2.0, 4.0, NULL);
	DNode *road5 = new DNode("5", 2.0, 3.0, 4.7, NULL);
	DNode *road6 = new DNode("6", 3.0, 4.0, 1.8, NULL);
	DNode *road7 = new DNode("7", 4.0, 2.0, 2.8, NULL);
	DNode *road8 = new DNode("8", 2.0, 4.0, 1.8, NULL);
	DNode *road9 = new DNode("9", 1.0, 3.0, 2.4, NULL);
	DNode *road10 = new DNode("10", 4.0, 4.0, 1.3, NULL);
	DNode *road11 = new DNode("11", 3.0, 5.0, 1.9, NULL);
	list<DNode*> temp1;
	temp1.push_back(road2);
	temp1.push_back(road3);
	roadMap[road1] = temp1;
	list<DNode*> temp2;
	temp2.push_back(road9);
	roadMap[road2] = temp2;
	list<DNode*> temp3;
	temp3.push_back(road4);
	temp3.push_back(road5);
	roadMap[road3] = temp3;
	list<DNode*> temp4;
	temp4.push_back(road7);
	roadMap[road4] = temp4;
	list<DNode*> temp5;
	temp5.push_back(road6);
	temp5.push_back(road8);
	roadMap[road5] = temp5;
	list<DNode*> temp6;
	temp6.push_back(road10);
	temp6.push_back(road11);
	roadMap[road6] = temp6;
	list<DNode*> temp7;
	temp7.push_back(road10);
	roadMap[road7] = temp7;
	list<DNode*> temp8;
	roadMap[road8] = temp8;
	list<DNode*> temp9;
	roadMap[road9] = temp9;
	list<DNode*> temp10;
	roadMap[road10] = temp10;
	list<DNode*> temp11;
	roadMap[road11] = temp11;
	setReverseRoadMap();
	initRoadMap();
}
// 初始化路网
void DStar::initRoadMap() {
	double maxValue = DB_max;
	map<DNode*, list<DNode*> >::iterator it;
	for (it = roadMap.begin(); it != roadMap.end(); it++) {
		DNode *key = it->first;
		key->setG(maxValue);
		key->setRhs(maxValue);
	}
}
// 通过名称获取道路
DNode* DStar::getDNode(string edgeName) {
	DNode *result = NULL;
	map<DNode*, list<DNode*> >::iterator it;
	for (it = roadMap.begin(); it != roadMap.end(); it++) {
		DNode *key = it->first;
		if (key->getRoadName() == edgeName) {
			result = it->first;
			break;
		}
	}
	return result;
}
/**
 * 开始搜索最优路径初始化
 * @param startRoad
 * @param endRoad
 */
void DStar::initRoad(DNode *sNode,DNode *eNode) {
	countH(eNode, sNode);
	eNode->setRhs(0.0);
	setKey(eNode);
	uPriority.push_back(eNode);
}
// 第一阶段规划出最优路径
void DStar::search(DNode* eNode, DNode* sNode) {
	DNode *firstNode = uPriority.front();
	openList.push_back(firstNode);
	uPriority.pop_front();
	bool isFind = false;
	DNode *node = NULL;
	while (openList.size() > 0) {
		node = openList.front();
		if (node->getG() > node->getRhs()) {
			node->setG(node->getRhs());
			if (node->getRoadName() == sNode->getRoadName()) {
				openList.pop_front();
				isFind = true;
				break;
			}
			list<DNode*> allowedRoad = get_reverse_node(node);
			for (unsigned int i = 0; i < closeList.size(); i++) {
				DNode *temp = getDNodeElem(closeList, i);
				if (contains(allowedRoad, temp)) {
					allowedRoad.remove(temp);
				}
			}
			for (unsigned int i = 0; i < allowedRoad.size(); i++) {
				DNode *temp = getDNodeElem(allowedRoad, i);
				countH(temp, sNode);
				updateRoad(temp, eNode);
			}
		} else {
			node->setG(DB_max);
			updateRoad(node, eNode);
			list<DNode*> allowedRoad = get_reverse_node(node);
			for (unsigned int i = 0; i < closeList.size(); i++) {
				DNode *temp = getDNodeElem(closeList, i);
				if (contains(allowedRoad, temp)) {
					allowedRoad.remove(temp);
				}
			}
			for (unsigned int i = 0; i < allowedRoad.size(); i++) {
				DNode *temp = getDNodeElem(allowedRoad, i);
				countH(temp, sNode);
				updateRoad(temp, eNode);
			}
		}
		DNode *o_last = openList.front();
		closeList.push_back(o_last);
		openList.pop_front();
		uPriority.sort(compD);
		DNode *u_last = uPriority.front();
		openList.push_back(u_last);
		uPriority.pop_front();
	}
	if (isFind) {
		display(sNode, eNode);
	}
}
//当路况发生变化时 更新rhs
void DStar::updateRoad(DNode* paramNode, DNode* eNode) {
	DNode *localRoad = NULL;
	if ((paramNode->getRoadName() != eNode->getRoadName())) {
		paramNode->setRhs(DB_max);
		list<DNode*> allowedRoad = roadMap[paramNode];
		double min;
		for (unsigned int i = 0; i < allowedRoad.size(); i++) {
			if (getDNodeElem(allowedRoad, i)->getG() == DB_max) {
				min = DB_max;
			} else {
				min = getDNodeElem(allowedRoad, i)->getG()
						+ getDNodeElem(allowedRoad, i)->getRoadWeight();
			}
			if (paramNode->getRhs() <= min) {
				continue;
			}
			paramNode->setRhs(min);
			localRoad = getDNodeElem(allowedRoad, i);
		}
	}
	paramNode->setParentNode(localRoad);
	if (paramNode->getG() != paramNode->getRhs()) {
		setKey(paramNode);
		uPriority.push_back(paramNode);
	}
}
//打印一次规划后的最优路径
void DStar::display(DNode* sNode, DNode* eNode) {
	double totalCost = 0.0;
	DNode *p = sNode;
	while (p != eNode) {
		totalCost += p->getRoadWeight();
		cout << p->getRoadName() + "->";
		p = p->getParentNode();
	}
	totalCost += p->getRoadWeight();
	cout << p->getRoadName() << "." << endl;
}

double DStar::getDistance(DNode* node, DNode* eNode) {
	return 0.0;
}

void DStar::getPath(list<DNode*>* resultList, DNode* node) {
}
void DStar::move(string startRoad, string endRoad) {
	DNode *sNode = getDNode(startRoad);
	DNode *eNode = getDNode(endRoad);
	initRoad(sNode, eNode);
	search(eNode, sNode);
}

void DStar::setReverseRoadMap() {
	map<DNode*, list<DNode*> >::iterator iter;
	for (iter = roadMap.begin(); iter != roadMap.end(); iter++) {
		DNode *key = iter->first;
		list<DNode*> reverseLink;
		map<DNode*, list<DNode*> >::iterator it;
		for (it = roadMap.begin(); it != roadMap.end(); it++) {
			DNode *r_key = it->first;
			list<DNode*> temp = roadMap[r_key];
			if (temp.size() != 0 && contains(temp, key)) {
				reverseLink.push_back(r_key);
			}
		}
		reverseRoadMap[key] = reverseLink;
	}
}

list<DNode*> DStar::get_reverse_node(DNode* node) {
	return reverseRoadMap[node];
}

DStar::~DStar() {
}

