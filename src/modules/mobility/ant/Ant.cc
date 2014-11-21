#include "Ant.h"
using namespace std;

CAnt::CAnt(void) {

}
//���캯��
CAnt::CAnt(int startRoadNumber, int endRoadNumber) {
	this->startRoadNumber = startRoadNumber;
	this->endRoadNumber = endRoadNumber;
	//�����߹���·����������Ϊ0
	m_dbPathLength = 0.0;
	m_nCurRoadNumber = startRoadNumber;
	//�ѳ������б�����·��������
	this->movedPath.push_back(startRoadNumber);
}

//��������
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
//��ʼ����������������ǰ����
void CAnt::Init(int startRoadNumber, int endRoadNumber) {
	this->startRoadNumber = startRoadNumber;
	this->endRoadNumber = endRoadNumber;
	m_dbPathLength = 0.0;

//���ѡ��һ����������
	m_nCurRoadNumber = startRoadNumber;

//�ѳ������б�����·��������
	this->movedPath.push_back(startRoadNumber);
}

//ѡ����һ������
//����ֵ Ϊ���б��
int CAnt::ChooseNextRoad() {
	int nSelectedRoad = -1; //���ؽ��������ʱ��������Ϊ-1
	double dbTotal = 0.0;
	double *prob = new double[Config::N_CITY_COUNT]; //����������б�ѡ�еĸ���
	vector<int> allowedRoad = setAllowRoad();
	int preRoadNumber = -1;
	if (movedPath.size() > 2) {
		int index = movedPath.size() - 2;
		preRoadNumber = movedPath[index];
	} else {
		preRoadNumber = startRoadNumber;
	}

	while ((allowedRoad.size() == 0)
			|| (allowedRoad.size() == 1 && contains(movedPath, allowedRoad[0]))) { //�ߵ�����ͬ ���л���
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
//���ؽ�������ǳ��еı��
	return nSelectedRoad;
}
//���Ͻ�������һ��
void CAnt::Search(int startRoadNumber, int endRoadNumber) {

	this->Init(startRoadNumber, endRoadNumber); //��������ǰ���ȳ�ʼ��
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
//�������������߹���·������
	CalPathLength();
}

//���������߹���·������
void CAnt::CalPathLength() {

	m_dbPathLength = 0.0; //�Ȱ�·��������0
	int m = 0;
	int n = 0;
	for (int i = 1; i != movedPath.size(); i++) {
		m = movedPath[i];
		n = movedPath[i - 1];
		m_dbPathLength += Config::getDistance(n, m);
	}
}

