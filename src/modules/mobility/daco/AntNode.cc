/*
 * AntNode.cpp
 *
 *  Created on: 2014-12-30
 *      Author: bobo
 */

#include "AntNode.h"

AntNode::AntNode() {
	roadName = "noRoad";
	count = 0;
	xPos = 0.0;
	yPos = 0.0;
	roadWeight = 0.0;
	pheromone = 0.0;
}

AntNode::AntNode(string roadName, double roadWeight, double xPos, double yPos,
		double pheromone) {
	this->roadName = roadName;
	this->roadWeight = roadWeight;
	this->xPos = xPos;
	this->yPos = yPos;
	this->pheromone = pheromone;
	this->count = 0;
}

int AntNode::getCount() {
	return count;
}

void AntNode::setCount(int count) {
	this->count = count;
}

double AntNode::getxPos() {
	return this->xPos;
}

void AntNode::setxPos(double xPos) {
	this->xPos = xPos;
}

double AntNode::getyPos() {
	return this->yPos;
}

void AntNode::setyPos(double yPos) {
	this->yPos = yPos;
}

string AntNode::getRoadName() {
	return this->roadName;
}

void AntNode::setRoadName(string roadName) {
	this->roadName = roadName;
}

double AntNode::getRoadWeight() {
	return this->roadWeight;
}

void AntNode::setRoadWeight(double roadWeight) {
	this->roadWeight = roadWeight;
}

double AntNode::getPheromone() {
	return this->pheromone;
}

void AntNode::setPheromone(double pheromone) {
	this->pheromone = pheromone;
}

AntNode::~AntNode() {
}

