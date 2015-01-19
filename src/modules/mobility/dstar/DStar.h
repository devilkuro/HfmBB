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
	map<DNode*, list<DNode*> > roadMap; // ��ǰ�ֵ�������ͨ�Ľֵ�map
	map<DNode*, list<DNode*> > reverseRoadMap; // ǰ�򼯺�
	list<DNode*> openList; // �����б�
	list<DNode*> closeList; // �ر��б�
	list<DNode*> uPriority; // ���ȶ���
	list<DNode*> key_node;
	void initRoadMap(); // ��ʼ��·��
	list<DNode*> get_reverse_node(DNode *node);//��ȡǰ�򼯺�
	void initRoad(DNode *sNode, DNode *eNode); //��ʼ��������·����ʼ��
	void search(DNode *eNode, DNode *sNode); //��һ�׶ι滮������·��
	void updateRoad(DNode *paramNode, DNode *eNode); //��·�������仯ʱ ����rhs
	void display(DNode *sNode, DNode *eNode); //��ӡһ�ι滮�������·��
	double getDistance(DNode *node, DNode *eNode); // ��ȡ�ֵ��ĳ���
	void getPath(list<DNode*> *resultList, DNode *node);
	DNode* getDNode(string edgeName); // ͨ�����ƻ�ȡ��·
public:
	DStar();
	void setRoadMap(); // �����ͼmap
	void setReverseRoadMap(); // ���췴��·��map
	void move(string startRoad, string endRoad); //��ʼ��������·���ƶ�
	virtual ~DStar();
};

#endif /* DSTAR_H_ */
