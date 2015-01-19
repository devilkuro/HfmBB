#ifndef   MY_H_Util       //如果没有定义这个宏
#define   MY_H_Util       //定义这个宏
#define DB_max 10e9
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include "AntNode.h"
using namespace std;

template<typename T>
void printer(const T& val) {
	cout << val << " ";
}

//返回指定范围内的随机整数
bool contains(vector<AntNode*> vec, AntNode *res);
bool contains(list<AntNode*> li, AntNode *res);
//返回指定范围内的随机浮点数
double rnd(double dbLow, double dbUpper);
//返回浮点数四舍五入取整后的浮点数
double ROUND(double dbA);
int rnd(int nLow, int nUpper);
#endif
