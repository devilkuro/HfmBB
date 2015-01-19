/*
 * DNode.h
 *
 *  Created on: 2015-1-4
 *      Author: bobo
 */

#ifndef DNODE_H_
#define DNODE_H_
#include<string>
#include "Key.h"
using namespace std;
class DNode {
private:
	string roadName;
	double xPos;
	double yPos;
	double roadWeight;
	DNode *parentNode;
	double rhs;
	double h;
	double g;
	Key *key;
public:
	DNode();
	DNode(string roadName, double xPos, double yPos, double roadWeight,
			DNode *parentNode);
	Key* getKey();
	void setKey(Key *key);
	string getRoadName();
	void setRoadName(string roadName);
	double getxPos();
	void setxPos(double xPos);
	double getyPos();
	void setyPos(double yPos);
	double getRoadWeight();
	void setRoadWeight(double roadWeight);
	double getRhs();
	void setRhs(double rhs);
	double getH();
	void setH(double h);
	double getG();
	void setG(double g);
	DNode* getParentNode();
	void setParentNode(DNode* parentNode);
	virtual ~DNode();
};

#endif /* DNODE_H_ */
