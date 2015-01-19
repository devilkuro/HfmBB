/*
 * ACO.h
 *
 *  Created on: 2015-1-4
 *      Author: bobo
 */

#ifndef ACO_H_
#define ACO_H_
#include <map>
#include<string>
#include "AntNode.h"
#include <list>
#include <stdlib.h>
#include <BaseSeekRouteFunc.h>
using namespace std;

class ACO :public BaseSeekRouteFunc{
public:
	static map<AntNode*, list<AntNode*> > roadMap; //·���ڽӱ�
	static double ALPHA; // �������ӣ���Ϣ�ص���Ҫ�̶�
	static double BETA; // �������ӣ����м�������Ҫ�̶�
	static double ROU; // ��Ϣ�ز�������
	static double P; // α�����¼�
	static int N_ANT_COUNT; // ��������
	static int N_IT_COUNT; // ��������
	static double DBQ; // �ܵ���Ϣ��
	static double DB_MAX; //һ����־����10��9�η�
	static double DB_MIN;
	void setRoadMap(double pre_trial);
	AntNode* getAntNode(string edgeName); // ͨ�����ƻ�ȡ��·
	ACO();
	list<string> getAllEdges();
	list<string> getNextEdges(string edge);
	double getEdgeLength(string edge);
	int getVehicleNum(string edge);
	double getTravelTime(string edge);
	void setRoute(list<string> route);
	virtual ~ACO();
};
#endif /* ACO_H_ */
