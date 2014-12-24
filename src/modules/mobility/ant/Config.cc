#include "Config.h"
#include "statistics/GlobalStatistics.h"
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
    for(iter = allEdges.begin(); iter != allEdges.end(); iter++){
        roadMaps[*iter] = i;
        i++;
    }
    return roadMaps;
}

void Config::init(GlobalMapSystem* gms) {
    this->gms = gms;
    GlobalStatistics* gs = GlobalStatisticsAccess().get();
    roadMaps = convertStringToID(getAllEdges());
    gs->changeName("edgeName");
    for(map<string, int>::iterator it = roadMaps.begin(); it != roadMaps.end(); it++){
        gs->get() << (it->first);
    }
    gs->get() << gs->endl;

    gs->output("con-edge.txt");
    gs->changeName("edgeLength");
    for(map<string, int>::iterator it = roadMaps.begin(); it != roadMaps.end(); it++){
        gs->get() << it->first << getEdgeLength(it->first);
    }
    gs->get() << gs->endl;
    gs->output("con-length.txt");
    for(map<string, int>::iterator it = roadMaps.begin(); it != roadMaps.end(); it++){
        gs->changeName(it->first);
        list<string> edges = getNextEdges(it->first);
        for(list<string>::iterator lit = edges.begin(); lit != edges.end(); lit++){
            gs->get() << *lit;
        }
        gs->get() << gs->endl;
    }
    for(map<string, int>::iterator it = roadMaps.begin(); it != roadMaps.end(); it++){
        gs->changeName("shape,"+it->first);
        list<Coord> coords =getEdgeShape(it->first);
        for(list<Coord>::iterator lit = coords.begin(); lit != coords.end(); lit++){
            gs->get() << (*lit).x<<(*lit).y;
        }
        gs->get() << gs->endl;
    }
    gs->output("con-connect.txt");
    N_CITY_COUNT = roadMaps.size() + 1;
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

list<Coord> Config::getEdgeShape(string edge) {
    return gms->getEdgeShape(edge);
}
