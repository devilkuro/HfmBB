#ifndef   MY_H_Config       //如果没有定义这个宏  
#define   MY_H_Config       //定义这个宏
#include <map>
#include<string>
#include<list>
#include "GlobalMapSystem.h"
using namespace std;

class Config {
public:
	static double ALPHA; //启发因子，信息素的重要程度
	static double BETA;   //期望因子，城市间距离的重要程度
	static double ROU; //信息素残留参数

	static int N_ANT_COUNT; //蚂蚁数量
	static int N_IT_COUNT; //迭代次数
	static int N_CITY_COUNT; //城市数量

	static double DBQ; //总的信息素
	static double DB_MAX; //一个标志数，10的9次方

	static double **g_Trial;    //两两城市间信息素，就是环境信息素
	static double **g_Distance; //两两城市间距离
	static map<int, map<int, double> > roadNetInfo;
	static map <string, int> roadMaps;
	/************************************************************************/
	/* 获取城市x到城市y的距离 */
	/************************************************************************/
	static void setDistance(int a, int b, double dis, double trial);
	/************************************************************************/
	/* 设置城市x到城市y的距离
	 ************************************************************************/
	static double getDistance(int a, int b);
	/**
	 *设置街道名称和街道编号相对应
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
