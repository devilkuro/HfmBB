/*
 * DNode.cpp
 *
 *  Created on: 2015-1-4
 *      Author: bobo
 */

#include "DNode.h"

DNode::DNode() {
	// TODO Auto-generated constructor stub
	this->roadName = "noRoad";
	xPos = 0.0;
	yPos = 0.0;
	roadWeight = 0.0;
	this->parentNode = NULL;
	rhs = 0.0;
	h = 0;
	g = 0;
	this->key = NULL;
}

DNode::DNode(string roadName, double xPos, double yPos, double roadWeight,
		DNode *parentNode) {
	this->roadName = roadName;
	this->xPos = xPos;
	this->yPos = yPos;
	this->roadWeight = roadWeight;
	this->parentNode = parentNode;
	rhs = 0.0;
	h = 0;
	g = 0;
	this->key = NULL;
}

Key* DNode::getKey() {
	return this->key;
}

void DNode::setKey(Key *key) {
	this->key = key;
}

string DNode::getRoadName() {
	return this->roadName;
}

void DNode::setRoadName(string roadName) {
	this->roadName = roadName;
}

double DNode::getxPos() {
	return this->xPos;
}

void DNode::setxPos(double xPos) {
	this->xPos = xPos;
}

double DNode::getyPos() {
	return this->yPos;
}

void DNode::setyPos(double yPos) {
	this->yPos = yPos;
}

double DNode::getRoadWeight() {
	return this->roadWeight;
}

void DNode::setRoadWeight(double roadWeight) {
	this->roadWeight = roadWeight;
}

double DNode::getRhs() {
	return this->rhs;
}

void DNode::setRhs(double rhs) {
	this->rhs = rhs;
}

double DNode::getH() {
	return this->h;
}

void DNode::setH(double h) {
	this->h = h;
}

double DNode::getG() {
	return this->g;
}

void DNode::setG(double g) {
	this->g = g;
}

DNode::~DNode() {
	// TODO Auto-generated destructor stub
}

DNode* DNode::getParentNode() {
	return this->parentNode;
}

void DNode::setParentNode(DNode* parentNode) {
	this->parentNode = parentNode;
}

