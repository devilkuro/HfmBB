/*
 * Dijkstra.cpp
 *
 *  Created on: 2014-12-30
 *      Author: bobo
 */

#include "Dijkstra.h"

//默认构造函数
Dijkstra::Dijkstra() {
}
//构建临时路网
void Dijkstra::setRoadMap() {
	Node *road1 = new Node("1", 1.2);
	Node *road2 = new Node("2", 1.7);
	Node *road3 = new Node("3", 2.3);
	Node *road4 = new Node("4", 1.0);
	Node *road5 = new Node("5", 4.7);
	Node *road6 = new Node("6", 3.8);
	Node *road7 = new Node("7", 2.8);
	Node *road8 = new Node("8", 1.8);
	Node *road9 = new Node("9", 2.4);
	Node *road10 = new Node("10", 1.3);
	Node *road11 = new Node("11", 1.9);
	vector<Node*> temp1;
	temp1.push_back(road2);
	temp1.push_back(road3);
	roadMap[road1] = temp1;
	vector<Node*> temp2;
	temp2.push_back(road9);
	roadMap[road2] = temp2;
	vector<Node*> temp3;
	temp3.push_back(road4);
	temp3.push_back(road5);
	roadMap[road3] = temp3;
	vector<Node*> temp4;
	temp4.push_back(road7);
	roadMap[road4] = temp4;
	vector<Node*> temp5;
	temp5.push_back(road6);
	temp5.push_back(road8);
	roadMap[road5] = temp5;
	vector<Node*> temp6;
	temp6.push_back(road10);
	temp6.push_back(road11);
	roadMap[road6] = temp6;
	vector<Node*> temp7;
	temp7.push_back(road10);
	roadMap[road7] = temp7;
	vector<Node*> temp8;
	roadMap[road8] = temp8;
	vector<Node*> temp9;
	roadMap[road9] = temp9;
	vector<Node*> temp10;
	roadMap[road10] = temp10;
	vector<Node*> temp11;
	roadMap[road11] = temp11;
}
// 通过名称获取道路
Node* Dijkstra::getNode(string edgeName) {
	map<Node*, vector<Node*> >::iterator it;
	for (it = roadMap.begin(); it != roadMap.end(); it++) {
		Node *temp = it->first;
		if (temp->getRoadName() == edgeName) {
			return it->first;
			break;
		}
	}
	return NULL;
}
//初始化
void Dijkstra::init(Node* sNode) {
	closeList.push_back(sNode); // sNode 加入到closeList 其他加入到openList
	sNode->setShortestWeight(0.0);
	vector<Node*> nearests = roadMap[sNode];
	map<Node*, vector<Node*> >::iterator it;
	for (it = roadMap.begin(); it != roadMap.end(); it++) {
		Node *temp = it->first;
		if (temp != sNode) {
			openList.push_back(temp);
		}
	}
	for (it = roadMap.begin(); it != roadMap.end(); it++) {
		Node *temp = it->first;
		if (contains(nearests, temp)) {
			temp->setShortestWeight(temp->getRoadWeight());
		} else {
			temp->setShortestWeight(DB_max);
		}
		temp->setParentNode(sNode);
	}
}
// 核心函数
void Dijkstra::dijkstra(Node* sNode, Node* eNode) {
	int n = roadMap.size();
	for (int i = 0; i < n - 1; i++) {
		Node *nearest = getShortestPath();
		if (nearest == NULL) {
			break;
		}
		closeList.push_back(nearest);
		vector<Node*>::iterator vec_iter = openList.begin();
		for (; vec_iter != openList.end();) {
			if (*vec_iter == nearest) {
				vec_iter = openList.erase(vec_iter);
			} else {
				vec_iter++;
			}
		}
		vector<Node*> connectList = roadMap[nearest];
		for (unsigned int k = 0; k < openList.size(); k++) {
			Node *temp = openList[k];
			double shorestPath = nearest->getShortestWeight()
					+ temp->getRoadWeight();
			if (shorestPath < temp->getShortestWeight()
					&& (contains(connectList, temp))) {
				temp->setShortestWeight(shorestPath);
				temp->setParentNode(nearest);
			}
		}
	}
}
//打印最优路径
void Dijkstra::displayCost(Node* sNode, Node* eNode) {
	Node *p = eNode;
	vector<Node*> result;
	result.push_back(eNode);
	while (p != sNode) {
		p = p->getParentNode();
		result.push_back(p);
	}
	vector<Node*>::reverse_iterator r_it; //方向迭代器
	for (r_it = result.rbegin(); r_it != result.rend() - 1; r_it++) {
		cout << (*r_it)->getRoadName() + "->";
	}
	cout << (*r_it)->getRoadName() + "." << endl;
}
// 搜索函数
void Dijkstra::search(string startRoad, string endRoad) {
	Node *sNode = getNode(startRoad);
	Node *eNode = getNode(endRoad);
	init(sNode);
	dijkstra(sNode, eNode);
	displayCost(sNode, eNode);
}
// 获取最小代价的点
Node* Dijkstra::getShortestPath() {
	Node *res = NULL;
	double minWeight = DB_max;
	for (unsigned int i = 0; i < openList.size(); i++) {
		Node *temp = openList[i];
		if (temp->getShortestWeight() < minWeight) {
			res = temp;
			minWeight = temp->getShortestWeight();
		}
	}
	return res;
}

bool Dijkstra::contains(vector<Node*> vec, Node* res) {
	bool ret = false;
	vector<Node*>::iterator vec_iter;
	for (vec_iter = vec.begin(); vec_iter != vec.end(); vec_iter++) {
		if (*vec_iter == res) {
			ret = true;
			break;
		}
	}
	return ret;
}

//析构函数
Dijkstra::~Dijkstra() {
}

