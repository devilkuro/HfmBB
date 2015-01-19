/*
 * Ant.h
 *
 *  Created on: 2015-1-4
 *      Author: bobo
 */

#ifndef ANT_H_
#define ANT_H_
#include <string>
#include <vector>
#include <math.h>
#include "AntNode.h"
#include "../utils.h"
#include "ACO.h"
using namespace std;

class Ant {
private:
	AntNode* startRoad; //���ֵ�
	AntNode* endRoad; //�յ�ֵ�
	AntNode* curRoad; // ��ǰ���ڽֵ�
	double movedPathLength; // �����߹���·������
public:
    vector<AntNode*> movedPath; // �����Ѿ��߹���·��
	AntNode* getStartRoad();
	void setStartRoad(AntNode *startRoad);
	AntNode* getEndRoad();
	void setEndRoad(AntNode *endRoad);
	AntNode* getCurRoad();
	void setCurRoad(AntNode *curRoad);
	double getMovedPathLength();
	void setMovedPathLength(double movedPathLength);
	vector<AntNode*> getMovedPath();
	void setMovedPath(vector<AntNode*> movedPath);
	Ant();
	Ant(AntNode *startRoad, AntNode *endRoad, AntNode *curRoad);
	void move();
	AntNode* chooseNextRoad();
	void calPathLength();
	vector<AntNode*> setAllowedRoad();
	double countH(AntNode *node, AntNode *eNode);
	double countThreshold(int iter);
	list<AntNode*> displayPath();
	void checkmovedPath();
	virtual ~Ant();
};

#endif /* ANT_H_ */
