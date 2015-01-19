#include "DNodeUtil.h"

// 改变道路权值
void setRoadWeight(DNode* node, int flag) {
	double p = 0.7;
	if (flag == 0) {
		node->setRoadWeight(node->getRoadWeight() * p);
	} else if (flag == 1) {
		node->setRoadWeight(node->getRoadWeight() * (1 + p));
	}
}
// 计算H值
void countH(DNode* node, DNode* sNode) {
	node->setH(
			sqrt(
					pow(node->getxPos() - sNode->getxPos(), 2)
							+ pow(node->getyPos() - sNode->getyPos(), 2)));
}
// 计算key值
void setKey(DNode* node) {
	Key *key = new Key();
	double key1 = min(node->getG(), node->getRhs()) + node->getH();
	double key2 = min(node->getG(), node->getRhs());
	key->setKey1(key1);
	key->setKey2(key2);
	node->setKey(key);
}
// 节点比较类
bool compD(DNode* o1, DNode* o2) {
	double o1key1 = o1->getKey()->getKey1();
	double o1key2 = o1->getKey()->getKey2();
	double o2key1 = o2->getKey()->getKey1();
	double o2key2 = o2->getKey()->getKey2();
	if (o1key1 < o2key1 || (o1key1 == o2key1 && o1key2 <= o2key2)) {
		return true;
	} else if (o1key1 == o2key1 && o1key2 == o2key2) {
		return false;
	} else {
		return false;
	}
}

DNode* getDNodeElem(list<DNode*> li, int index) {
	DNode *result = NULL;
	list<DNode*>::iterator iter;
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

bool contains(list<DNode*> li, DNode* res) {
	bool result = false;
	list<DNode*>::iterator iter;
	for (iter = li.begin(); iter != li.end(); iter++) {
		if (*iter == res) {
			result = true;
			break;
		}
	}
	return result;
}
