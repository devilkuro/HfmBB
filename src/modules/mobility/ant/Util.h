#ifndef   MY_H_Util       //如果没有定义这个宏  
#define   MY_H_Util       //定义这个宏

#include <vector>
#include <iostream>
#include <math.h>
#include <cstdlib>
using namespace std;

template<typename T>
void printer(const T& val)
{
	cout << val << " ";
}

//返回指定范围内的随机整数

bool contains(vector<int> vec,int res);
//返回指定范围内的随机浮点数
double rnd(double dbLow,double dbUpper);
//返回浮点数四舍五入取整后的浮点数
double ROUND(double dbA);
#endif
