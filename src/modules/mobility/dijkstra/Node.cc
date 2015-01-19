/*
 * Node.cpp
 *
 *  Created on: 2014-12-30
 *      Author: bobo
 */

#include "Node.h"

string Node::getRoadName() {
	return roadName;
}

void Node::setRoadName(string roadName) {
	this->roadName = roadName;
}

double Node::getRoadWeight() {
	return roadWeight;
}

void Node::setRoadWeight(double roadWeight) {
	this->roadWeight = roadWeight;
}

double Node::getShortestWeight() {
	return shortestWeight;
}

Node::Node(string roadName, double roadWeight) {
	this->roadName = roadName;
	this->roadWeight = roadWeight;
	this->shortestWeight = 10e9;
	this->parentNode = NULL;
}

Node::Node() {
	this->roadName = "noName";
	this->roadWeight = 0.0;
	this->shortestWeight = 0.0;
	this->parentNode = NULL;
}

void Node::setShortestWeight(double shortestWeight) {
	this->shortestWeight = shortestWeight;
}
Node::~Node() {
}

Node* Node::getParentNode() {
	return this->parentNode;
}

void Node::setParentNode(Node* parentNode) {
	this->parentNode = parentNode;
}
