#ifndef   MY_H_Util       //���û�ж��������
#define   MY_H_Util       //���������
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

//����ָ����Χ�ڵ��������
bool contains(vector<AntNode*> vec, AntNode *res);
bool contains(list<AntNode*> li, AntNode *res);
//����ָ����Χ�ڵ����������
double rnd(double dbLow, double dbUpper);
//���ظ�������������ȡ����ĸ�����
double ROUND(double dbA);
int rnd(int nLow, int nUpper);
#endif
