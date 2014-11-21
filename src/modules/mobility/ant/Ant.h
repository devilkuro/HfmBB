#ifndef   ANT_H       //���û�ж��������
#define   ANT_H       //���������

#include <vector>
#include <algorithm>
#include "Config.h"
#include "Util.h"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "mem.h"
using namespace std;

class CAnt
{
public:
	CAnt(void);
	CAnt(int startRoadNumber,int endRoadNumber);
	~CAnt(void);

public:

	int startRoadNumber;//���ֵ�
	int endRoadNumber; //�յ�ֵ�
	int m_nCurRoadNumber; //��ǰ���ڽֵ�

	vector<int> movedPath;//�����߹���·��
	double m_dbPathLength; //�����߹���·������

public:
	vector<int> setAllowRoad();
	int ChooseNextRoad();   //ѡ����һ������
	void Init(int startRoadNumber,int endRoadNumber); //��ʼ��
	void Search(int startRoadNumber,int endRoadNumber);			//����·��
	void CalPathLength();   //���������߹���·������
	list<string> displayPath();
};

#endif

