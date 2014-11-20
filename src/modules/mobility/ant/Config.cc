#include "Config.h"
double Config::ALPHA = 1.0; //启发因子，信息素的重要程度
double Config::BETA = 2.0;   //期望因子，城市间距离的重要程度
double Config::ROU = 0.5; //信息素残留参数

int Config::N_ANT_COUNT = 5; //蚂蚁数量
int Config::N_IT_COUNT = 10; //迭代次数
int Config::N_CITY_COUNT = 7; //城市数量

double Config::DBQ = 100.0; //总的信息素
double Config::DB_MAX = 10e9; //一个标志数，10的9次方
double** Config::g_Distance = 0;
double** Config::g_Trial = 0;
map<int, map<int, double> > Config::roadNetInfo;
map<string, int> Config::roadMaps; //设置街道名称与街道ID相对应

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
