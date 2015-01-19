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
	void displayCost(Node *sNode, Node *eNode); //������ۺ���
	void init(Node *sNode);
	Node* getShortestPath(); //��ȡ��С���۵ĵ�
	Node* getNode(string edgeName); // ͨ�����ƻ�ȡ��·
public:
	Dijkstra();
	bool contains(vector<Node*> vec, Node *res);
	void setRoadMap(); //����·��
	void search(string startRoad, string endRoad); //��������
	void dijkstra(Node *sNode, Node *eNode); //���ĺ���
	virtual ~Dijkstra();
};

#endif /* DIJKSTRA_H_ */
