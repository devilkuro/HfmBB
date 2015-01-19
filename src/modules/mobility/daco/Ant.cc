/*
 * Ant.cpp
 *
 *  Created on: 2015-1-4
 *      Author: bobo
 */

#include "Ant.h"

Ant::Ant() {
	this->startRoad = NULL;
	this->endRoad = NULL;
	this->curRoad = NULL;
	movedPathLength = 0.0;
}

AntNode* Ant::getStartRoad() {
	return this->startRoad;
}

void Ant::setStartRoad(AntNode *startRoad) {
	this->startRoad = startRoad;
}

AntNode* Ant::getEndRoad() {
	return this->endRoad;
}

void Ant::setEndRoad(AntNode *endRoad) {
	this->endRoad = endRoad;
}

AntNode* Ant::getCurRoad() {
	return this->curRoad;
}

void Ant::setCurRoad(AntNode *curRoad) {
	this->curRoad = curRoad;
}

double Ant::getMovedPathLength() {
	return this->movedPathLength;
}

void Ant::setMovedPathLength(double movedPathLength) {
	this->movedPathLength = movedPathLength;
}

vector<AntNode*> Ant::getMovedPath() {
	return this->movedPath;
}

void Ant::setMovedPath(vector<AntNode*> movedPath) {
	this->movedPath = movedPath;
}
//���캯��
Ant::Ant(AntNode *startRoad, AntNode *endRoad, AntNode *curRoad) {
	this->startRoad = startRoad;
	this->endRoad = endRoad;
	this->curRoad = startRoad;
	this->movedPathLength = startRoad->getRoadWeight();
	movedPath.push_back(startRoad);
	startRoad->setCount(startRoad->getCount() + 1);
}
//���Ͻ�������һ��
void Ant::move() {
	if (curRoad->getRoadName() == endRoad->getRoadName()) {
		return;
	}
	while (true) {
		curRoad = chooseNextRoad();
		if (curRoad != NULL) {
			curRoad->setCount(curRoad->getCount() + 1);
			movedPath.push_back(curRoad);
			if (curRoad->getRoadName() == endRoad->getRoadName()) {
				checkmovedPath();  //����·��
				break;
			}
		} else {
			return;
		}
	}
	calPathLength(); // ���������߹��ĵ�·�Ļ���
}
// ѡ����һ���ֵ�
AntNode* Ant::chooseNextRoad() {
	AntNode *selectedRoad;
	vector<AntNode*> allowedRoad = setAllowedRoad();
	AntNode *preRoad; // �߹���ǰһ���ֵ�
	while (allowedRoad.size() == 0) {
		if (movedPath.size() > 2) {
			int index = movedPath.size() - 2;
			preRoad = movedPath[index];
		} else {
			preRoad = startRoad;
		}
		curRoad->setPheromone(0.0);
		curRoad = preRoad;
		movedPath.pop_back();
		allowedRoad = setAllowedRoad();
		if (allowedRoad.size() == 0 && curRoad == startRoad) {
			movedPath.clear();
			return NULL;
		}
	}
	int size = allowedRoad.size();
	double * prob = new double[size];
	double maxProb = ACO::DB_MIN;
	AntNode *maxProbRoad = NULL;
	double dbTotal = 0.0;
	for (unsigned int i = 0; i != allowedRoad.size(); i++) {
		AntNode *nextRoad = allowedRoad[i];
		if (!contains(movedPath, nextRoad)) {
			double p = 0.1;
			double a = nextRoad->getPheromone();
			double b = countH(nextRoad, this->endRoad) * (1 - p)
					+ p * nextRoad->getRoadWeight();
			if (b == 0.0) {
				selectedRoad = nextRoad;
				return selectedRoad;
			}
			double x = pow(a, ACO::ALPHA);
			double y = pow(1.0 / b, ACO::BETA);
			prob[i] = x * y;
			dbTotal += prob[i];
			if (prob[i] > maxProb) {
				maxProbRoad = allowedRoad[i];
			}
		} else {
			nextRoad->setPheromone(0.0);
		}
	}
	double p = rnd(0.0, 1.0); // α�����¼�ѡ���·
	if ((p > ACO::P) && p < (1.0 - ACO::P)) { // �����������ѡ��
		double dbTemp = 0.0;
		if (dbTotal > 0.0) {
			dbTemp = rnd(0.0, dbTotal);
			for (unsigned int i = 0; i != allowedRoad.size(); i++) {
				AntNode *nextRoad = allowedRoad[i];
				dbTemp = dbTemp - prob[i];
				if (dbTemp < 0.0) { // ����ֹͣת��������·�ڣ�ֱ������ѭ��
					selectedRoad = nextRoad;
					break;
				}
			}
		}
	} else if (p <= ACO::P) {
		selectedRoad = maxProbRoad;
	} else {
		int upIndex = allowedRoad.size() - 1;
		int rndIndex = rnd(0, upIndex);
		AntNode *nextRoad = allowedRoad[rndIndex];
		selectedRoad = nextRoad;
	}
	delete prob;
	return selectedRoad;
}
// ���������߹���·������
void Ant::calPathLength() {
	movedPathLength = 0.0; //�Ȱ�·��������0
	for (unsigned int i = 0; i != movedPath.size(); i++) {
		AntNode *temp = movedPath[i];
		movedPathLength += temp->getRoadWeight();
	}
}
// ���Ͽ���ѡ��Ľֵ�
vector<AntNode*> Ant::setAllowedRoad() {
	list<AntNode*> allowedRoad = ACO::roadMap[curRoad];
	vector<AntNode*> result;
	for (list<AntNode*>::iterator it = allowedRoad.begin();
			it != allowedRoad.end(); it++) {
		AntNode *temp = *it;
		if (temp->getPheromone() != 0.0 && !contains(movedPath, temp)) {
			result.push_back(*it);
		}
	}
	return result;
}
// ����ڵ㵽�յ��ŷʽ����
double Ant::countH(AntNode *node, AntNode *eNode) {
	double res = 0.0;
	res = sqrt(
			pow(node->getxPos() - eNode->getxPos(), 2)
					+ pow(node->getyPos() - eNode->getyPos(), 2));
	return res;
}
// ��������Ӧ��ֵ
double Ant::countThreshold(int iter) {
	double num = pow(iter, 2);
	return 1 - pow(exp(1), -num);
}

list<AntNode*> Ant::displayPath() {
	double totalCost = 0.0;
	list<AntNode*> route;
	vector<AntNode*>::iterator iter;
	for (iter = movedPath.begin(); iter != movedPath.end(); iter++) {
		route.push_back(*iter);
		AntNode *temp = *iter;
		totalCost += temp->getRoadWeight();
	}
	cout << "cost:" << totalCost << endl;
	return route;
}

void Ant::checkmovedPath() {
	for (unsigned int i = 0; i < movedPath.size(); i++) {
		for (unsigned int j = i + 2; j < movedPath.size(); j++) {
			AntNode *node_i = movedPath.at(i);
			AntNode *node_j = movedPath.at(j);
			if (contains(ACO::roadMap[node_i], node_j)) {
				int index_i = i;
				int index_j = j;
				int count = index_j - index_i - 1;
				while (count > 0) {
					vector<AntNode*>::iterator it = movedPath.begin()
							+ (index_i + 1);
					movedPath.erase(it);
					count--;
				}
			}
		}
	}
}

Ant::~Ant() {

}
