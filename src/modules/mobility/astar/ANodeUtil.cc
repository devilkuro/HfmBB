#include "ANodeUtil.h"
// 计算G,H,F值
// 计算G值
void countG(ANode *node, ANode *eNode) {
	double cost = node->getRoadWeight();
	if (node->getParentNode() == NULL) {
		node->setG(cost);
	} else {
		node->setG(node->getParentNode()->getG() + cost);
	}
}
// 计算H值
void countH(ANode *node, ANode *eNode) {
	node->setH(
			sqrt(
					pow(node->getxPos() - eNode->getxPos(), 2)
							+ pow(node->getyPos() - eNode->getyPos(), 2)));
}
// 计算F值
void countF(ANode *node) {
	node->setF(node->getG() + node->getH());
}
void count(ANode *node, ANode *eNode) {
	countG(node, eNode);
	countH(node, eNode);
	countF(node);
}
// 通过名称获取道路
ANode* getANode(map<ANode*, list<ANode*> > roadMap, string edgeName) {
	ANode *result = 0;
	for (map<ANode*, list<ANode*> >::iterator it = roadMap.begin();
			it != roadMap.end(); it++) {
		ANode *temp = it->first;
		if (temp->getRoadName() == edgeName) {
			result = it->first;
			break;
		}
	}
	return result;
}

bool contains(list<ANode*> li, ANode* res) {
	bool result = false;
	list<ANode*>::iterator iter;
	for (iter = li.begin(); iter != li.end(); iter++) {
		if (*iter == res) {
			result = true;
			break;
		}
	}
	return result;
}

ANode* getANodeElem(list<ANode*> li, int index) {
	ANode *result = NULL;
	list<ANode*>::iterator iter;
	int i = 0;
	for (iter = li.begin(); iter != li.end(); iter++) {
		if (i != index) {
			i++;
		} else {
			return *iter;
		}
	}
	return result;
}
bool compA(ANode *o1, ANode *o2) {
	if (o1->getF() > o2->getF()) {
		return false;
	} else if (o1->getF() == o2->getF()) {
		return false;
	} else {
		return true;
	}
}
