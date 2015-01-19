/*
 * ANode.h
 *
 *  Created on: 2015-1-4
 *      Author: bobo
 */

#ifndef ANODE_H_
#define ANODE_H_
#include <string>
using namespace std;
class ANode {
private:
	string roadName;
	double xPos;
	double yPos;
	double roadWeight;
	ANode *parentNode;
	double g;
	double h;
	double f;
public:
	ANode();
	ANode(string roadName, double xPos, double yPos, double roadWeight,
			ANode *parentNode);
	string getRoadName();
	void setRoadName(string roadName);
	double getxPos();
	void setxPos(double xPos);
	double getyPos();
	void setyPos(double yPos);
	double getRoadWeight();
	void setRoadWeight(double roadWeight);
	double getG();
	void setG(double g);
	double getH();
	void setH(double h);
	double getF();
	void setF(double f);
	virtual ~ANode();
	ANode* getParentNode();
	void setParentNode(ANode* parentNode);
};

#endif /* ANODE_H_ */
