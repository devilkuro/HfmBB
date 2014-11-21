#include "Ant.h"
using namespace std;

CAnt::CAnt(void) {

}
//构造函数
CAnt::CAnt(int startRoadNumber, int endRoadNumber) {
	this->startRoadNumber = startRoadNumber;
	this->endRoadNumber = endRoadNumber;
	//蚂蚁走过的路径长度设置为0
	m_dbPathLength = 0.0;
	m_nCurRoadNumber = startRoadNumber;
	//把出发城市保存入路径数组中
	this->movedPath.push_back(startRoadNumber);
}

//析构函数
CAnt::~CAnt(void) {
}

vector<int> CAnt::setAllowRoad() {
	int preRoadNumber = -1;
	if (movedPath.size() > 2) {
		int index = movedPath.size() - 2;
		preRoadNumber = movedPath[index];
	} else {
		preRoadNumber = startRoadNumber;
	}
	map<int, double> temp = Config::roadNetInfo[m_nCurRoadNumber];
	vector<int> allowedRoad;
	map<int, double>::iterator temp_iter;
	for (temp_iter = temp.begin(); temp_iter != temp.end(); temp_iter++) {
		if (Config::g_Trial[m_nCurRoadNumber][temp_iter->first] != 0.0) {
			allowedRoad.push_back(temp_iter->first);
		}
	}
	return allowedRoad;
}

list<string> CAnt::displayPath() {
	list<string> route;
	map<int, string> reMap;
	for (map<string, int>::iterator it = Config::roadMaps.begin();
			it != Config::roadMaps.end(); it++) {
		reMap[it->second] = it->first;
	}
	for (vector<int>::iterator it = movedPath.begin(); it != movedPath.end();
			it++) {
		route.push_back(reMap[*it]);
	}
	return route;
}
//初始化函数，蚂蚁搜索前调用
void CAnt::Init(int startRoadNumber, int endRoadNumber) {
	this->startRoadNumber = startRoadNumber;
	this->endRoadNumber = endRoadNumber;
	m_dbPathLength = 0.0;

//随机选择一个出发城市
	m_nCurRoadNumber = startRoadNumber;

//把出发城市保存入路径数组中
	this->movedPath.push_back(startRoadNumber);
}

//选择下一个城市
//返回值 为城市编号
int CAnt::ChooseNextRoad() {
	int nSelectedRoad = -1; //返回结果，先暂时把其设置为-1
	double dbTotal = 0.0;
	double *prob = new double[Config::N_CITY_COUNT]; //保存各个城市被选中的概率
	vector<int> allowedRoad = setAllowRoad();
	int preRoadNumber = -1;
	if (movedPath.size() > 2) {
		int index = movedPath.size() - 2;
		preRoadNumber = movedPath[index];
	} else {
		preRoadNumber = startRoadNumber;
	}

	while ((allowedRoad.size() == 0)
			|| (allowedRoad.size() == 1 && contains(movedPath, allowedRoad[0]))) { //走到死胡同 进行回溯
		preRoadNumber = movedPath[movedPath.size() - 2];
		Config::g_Trial[preRoadNumber][m_nCurRoadNumber] = 0.0;
		m_nCurRoadNumber = preRoadNumber;
		movedPath.pop_back();
		allowedRoad = setAllowRoad();
	}
	for (int i = 0; i != allowedRoad.size(); i++) {
		int nextRoadNum = allowedRoad[i];
		if (!contains(movedPath, nextRoadNum)) {
			double a = Config::g_Trial[m_nCurRoadNumber][nextRoadNum];
			double b = Config::getDistance(m_nCurRoadNumber, nextRoadNum);
			double x = pow(a, Config::ALPHA);
			double y = pow(1.0 / b, Config::BETA);
			prob[nextRoadNum] = x * y;
			dbTotal = dbTotal + prob[nextRoadNum];
		} else {
			prob[nextRoadNum] = 0.0;
		}
	}
	double dbTemp = 0.0;
	if (dbTotal > 0.0) {
		dbTemp = rnd(0.0, dbTotal);
		for (int i = 0; i != allowedRoad.size(); i++) {
			int nextRoadNum = allowedRoad[i];
			if (!contains(movedPath, nextRoadNum)) {
				dbTemp = dbTemp - prob[nextRoadNum];
				if (dbTemp < 0.0) {
					nSelectedRoad = nextRoadNum;
					break;
				}
			}
		}
	}
//返回结果，就是城市的编号
	return nSelectedRoad;
}
//蚂蚁进行搜索一次
void CAnt::Search(int startRoadNumber, int endRoadNumber) {

	this->Init(startRoadNumber, endRoadNumber); //蚂蚁搜索前，先初始化
	if (startRoadNumber == endRoadNumber) {
		return;
	}
	int nRoadNo = -1;
	while (nRoadNo != endRoadNumber) {
		nRoadNo = ChooseNextRoad();
		if (nRoadNo > 0) {
			movedPath.push_back(nRoadNo);
			m_nCurRoadNumber = nRoadNo;
		}
	}
//完成搜索后计算走过的路径长度
	CalPathLength();
}

//计算蚂蚁走过的路径长度
void CAnt::CalPathLength() {

	m_dbPathLength = 0.0; //先把路径长度置0
	int m = 0;
	int n = 0;
	for (int i = 1; i != movedPath.size(); i++) {
		m = movedPath[i];
		n = movedPath[i - 1];
		m_dbPathLength += Config::getDistance(n, m);
	}
}

