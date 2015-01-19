/*
 * ANode.cpp
 *
 *  Created on: 2015-1-4
 *      Author: bobo
 */

#include "ANode.h"

ANode::ANode() {
	// TODO Auto-generated constructor stub
	this->roadName = "noRoad";
	this->xPos = 0.0;
	this->yPos = 0.0;
	this->roadWeight = 0.0;
	this->parentNode = NULL;
	this->g = 0.0;
	this->h = 0.0;
	this->f = 0.0;
}

ANode::ANode(string roadName, double xPos, double yPos, double roadWeight,
		ANode *parentNode) {
	this->roadName = roadName;
	this->xPos = xPos;
	this->yPos = yPos;
	this->roadWeight = roadWeight;
	this->parentNode = parentNode;
	this->g = 0.0;
	this->h = 0.0;
	this->f = 0.0;
}

string ANode::getRoadName() {
	return this->roadName;
}

void ANode::setRoadName(string roadName) {
	this->roadName = roadName;
}

double ANode::getxPos() {
	return this->xPos;
}

void ANode::setxPos(double xPos) {
	this->xPos = xPos;
}

double ANode::getyPos() {
	return this->yPos;
}

void ANode::setyPos(double yPos) {
	this->yPos = yPos;
}

double ANode::getRoadWeight() {
	return this->roadWeight;
}

void ANode::setRoadWeight(double roadWeight) {
	this->roadWeight = roadWeight;
}

double ANode::getG() {
	return this->g;
}

void ANode::setG(double g) {
	this->g = g;
}

double ANode::getH() {
	return this->h;
}

void ANode::setH(double h) {
	this->h = h;
}

double ANode::getF() {
	return this->f;
}

void ANode::setF(double f) {
	this->f = f;
}

ANode::~ANode() {
	// TODO Auto-generated destructor stub
}
ANode* ANode::getParentNode() {
	return this->parentNode;
}

void ANode::setParentNode(ANode* parentNode) {
	this->parentNode = parentNode;
}

