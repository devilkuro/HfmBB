/*
 * Node.h
 *
 *  Created on: 2014-12-30
 *      Author: bobo
 */

#ifndef NODE_H_
#define NODE_H_
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;
class Node {
private:
	string roadName;
	double roadWeight;
	double shortestWeight;
	Node *parentNode;
public:
	Node();
	Node(string roadName, double roadWeight);
	virtual ~Node();
	string getRoadName();
	void setRoadName(string roadName);
	double getRoadWeight();
	void setRoadWeight(double roadWeight);
	double getShortestWeight();
	void setShortestWeight(double shortestWeight);

	Node* getParentNode();

    void setParentNode(Node* parentNode) ;
};

#endif /* NODE_H_ */
