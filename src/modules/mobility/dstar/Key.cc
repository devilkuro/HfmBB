/*
 * Key.cpp
 *
 *  Created on: 2015-1-4
 *      Author: bobo
 */

#include "Key.h"

namespace std {

Key::Key() {
	// TODO Auto-generated constructor stub
	key1 = 0.0;
	key2 = 0.0;
}

double Key::getKey1() {
	return key1;
}

void Key::setKey1(double key1) {
	this->key1 = key1;
}

double Key::getKey2() {
	return key2;
}

void Key::setKey2(double key2) {
	this->key2 = key2;
}

Key::~Key() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */
