/*
 * ACOTest.h
 *
 *  Created on: 2015-1-5
 *      Author: bobo
 */

#ifndef ACOTEST_H_
#define ACOTEST_H_
#include <string>
#include "ACO.h"
#include "Ant.h"
#include "BaseSeekRouteFunc.h"
using namespace std;
class ACOTest: public BaseSeekRouteFunc{
private:
	Ant** ordAnts; // ��ͨ��������
	Ant* bestAnt; // ����һ�����ϱ����������������������е����Ž��
	Ant* badestAnt; // �����������

    Ant* dbestAnt; // ����һ�����ϱ����������������������е����Ž��
    Ant* dbadestAnt; // �����������

public:
	list<AntNode*> route;
	ACOTest();
	virtual ~ACOTest();
	Ant* getBadestAnt();
	void setBadestAnt(Ant* badestAnt);
	Ant* getBestAnt();
	void setBestAnt(Ant* bestAnt);
	Ant** getOrdAnts();
	void setOrdAnts(Ant** ordAnts);
	void initData(string startRoad, string endRoad); // ��ʼ������
	void reduceBestPathTrial(); // �����ֲ����ź���
	void updateBestTrial();	// ����ÿ�ε������λ���ϵ�������Ϣ��
	void updateBadestTrial(); // ����ÿ�ε������λ���ϵ�������Ϣ��
	void updateTrial(); // ���»�����Ϣ��
	void setMaxMinTrial(); // ������С���ϵͳ
	void search(string startRoad, string endRoad); // ������ʼ����
	void seekRoute(string startRoad, string endRoad);
	void init(GlobalMapSystem* gms);
};

#endif /* ACOTEST_H_ */
