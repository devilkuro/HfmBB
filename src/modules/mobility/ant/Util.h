#ifndef   MY_H_Util       //���û�ж��������  
#define   MY_H_Util       //���������

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

//����ָ����Χ�ڵ��������

bool contains(vector<int> vec,int res);
//����ָ����Χ�ڵ����������
double rnd(double dbLow,double dbUpper);
//���ظ�������������ȡ����ĸ�����
double ROUND(double dbA);
#endif
