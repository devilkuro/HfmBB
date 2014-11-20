#include "Config.h"
double Config::ALPHA = 1.0; //�������ӣ���Ϣ�ص���Ҫ�̶�
double Config::BETA = 2.0;   //�������ӣ����м�������Ҫ�̶�
double Config::ROU = 0.5; //��Ϣ�ز�������

int Config::N_ANT_COUNT = 5; //��������
int Config::N_IT_COUNT = 10; //��������
int Config::N_CITY_COUNT = 7; //��������

double Config::DBQ = 100.0; //�ܵ���Ϣ��
double Config::DB_MAX = 10e9; //һ����־����10��9�η�
double** Config::g_Distance = 0;
double** Config::g_Trial = 0;
map<int, map<int, double> > Config::roadNetInfo;
map<string, int> Config::roadMaps; //���ýֵ�������ֵ�ID���Ӧ

void Config::setDistance(int a, int b, double dis, double trial) {
	map<int, double> temp = roadNetInfo[a];
	temp[b] = dis;
	roadNetInfo[a] = temp;
	g_Trial[a][b] = trial;
}

double Config::getDistance(int a, int b) {
	map<int, double> temp = roadNetInfo[a];
	double dis = temp[b];
	return dis;
}

map<string, int> Config::convertStringToID(list<string> allEdges) {
	map<string, int> roadMaps;
	list<string>::iterator iter;
	int i = 1;
	for (iter = allEdges.begin(); iter != allEdges.end(); iter++) {
		roadMaps[*iter] = i;
		i++;
	}
	return roadMaps;
}

void Config::init(GlobalMapSystem* gms) {
    this->gms = gms;
	roadMaps  = convertStringToID(
			getAllEdges());
}

list<string> Config::getAllEdges() {
	return gms->getAllEdges();
}

list<string> Config::getNextEdges(string edge) {
	return gms->getNextEdges(edge);
}

double Config::getEdgeLength(string edge) {
    return gms->getEdgeLength(edge);
}

int Config::getVehicleNum(string edge) {
    return 0;
}

double Config::getTravelTime(string edge) {
    return 0;
}

void Config::setRoute(list<string> route) {
}
