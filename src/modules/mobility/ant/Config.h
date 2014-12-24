#ifndef   MY_H_Config       //���û�ж��������  
#define   MY_H_Config       //���������
#include <map>
#include<string>
#include<list>
#include "GlobalMapSystem.h"
using namespace std;

class Config {
public:
	static double ALPHA; //�������ӣ���Ϣ�ص���Ҫ�̶�
	static double BETA;   //�������ӣ����м�������Ҫ�̶�
	static double ROU; //��Ϣ�ز�������

	static int N_ANT_COUNT; //��������
	static int N_IT_COUNT; //��������
	static int N_CITY_COUNT; //��������

	static double DBQ; //�ܵ���Ϣ��
	static double DB_MAX; //һ����־����10��9�η�

	static double **g_Trial;    //�������м���Ϣ�أ����ǻ�����Ϣ��
	static double **g_Distance; //�������м����
	static map<int, map<int, double> > roadNetInfo;
	static map <string, int> roadMaps;
	/************************************************************************/
	/* ��ȡ����x������y�ľ��� */
	/************************************************************************/
	static void setDistance(int a, int b, double dis, double trial);
	/************************************************************************/
	/* ���ó���x������y�ľ���
	 ************************************************************************/
	static double getDistance(int a, int b);
	/**
	 *���ýֵ����ƺͽֵ�������Ӧ
	 */
    static map <string, int> convertStringToID(list<string> allEdges);
public:
    void init(GlobalMapSystem* gms);

	list<string> getAllEdges();
	list<string> getNextEdges(string edge);
	double getEdgeLength(string edge);
	int getVehicleNum(string edge);
	double getTravelTime(string edge);
	void setRoute(list<string> route);
	list<Coord> getEdgeShape(string edge);
private:
	GlobalMapSystem* gms;
};
#endif 
