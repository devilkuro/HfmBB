/*
 * ACOUtil.cpp
 *
 *  Created on: 2015-1-4
 *      Author: bobo
 */

#include "ACOUtil.h"
#include "AntNode.h"
bool contains(vector<AntNode*> vec, AntNode *res) {
	bool ret = false;
	vector<AntNode*>::iterator vec_iter;
	for (vec_iter = vec.begin(); vec_iter != vec.end(); vec_iter++) {
		if (*vec_iter == res) {
			ret = true;
			break;
		}
	}
	return ret;
}

double rnd(double dbLow, double dbUpper) {
	double dbTemp = rand() / ((double) RAND_MAX + 1.0);
	return dbLow + dbTemp * (dbUpper - dbLow);
}

bool contains(list<AntNode*> li, AntNode* res) {
	bool ret = false;
	list<AntNode*>::iterator vec_iter;
	for (vec_iter = li.begin(); vec_iter != li.end(); vec_iter++) {
		if (*vec_iter == res) {
			ret = true;
			break;
		}
	}
	return ret;
}

int rnd(int nLow, int nUpper) {
	return nLow + (nUpper - nLow) * rand() / (RAND_MAX + 1);
}
double ROUND(double dbA) {
	return (double) ((int) (dbA + 0.5));
}
