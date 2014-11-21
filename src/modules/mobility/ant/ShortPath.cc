#include "ShortPath.h"

CShortPath::CShortPath(void) {
    cfg = NULL;
    m_cBestAnt = NULL;
    endPos = 0;
    startPos = 0;
	m_cAntAry = new CAnt*[Config::N_CITY_COUNT];
}

CShortPath::~CShortPath(void) {
}

//��ʼ������
void CShortPath::InitData(Config* cfg) {
    this->cfg = cfg;
	//�����������м����
	m_cBestAnt = new CAnt();
	m_cBestAnt->m_dbPathLength = Config::DB_MAX;
	for (unsigned int i = 1; i <= cfg->roadMaps.size(); i++) {
		map<int, double> temp;
		Config::roadNetInfo[i] = temp;
	}

	Config::g_Distance = new double*[Config::N_CITY_COUNT];
	for (int i = 0; i < Config::N_CITY_COUNT; i++) //��ѭ����һά�����ɶ�ά
			{
		Config::g_Distance[i] = new double[Config::N_CITY_COUNT];
	}

	Config::g_Trial = new double*[Config::N_CITY_COUNT];
	for (int i = 0; i < Config::N_CITY_COUNT; i++) //��ѭ����һά�����ɶ�ά
			{
		Config::g_Trial[i] = new double[Config::N_CITY_COUNT];
	}
	/*setDistance(1, 2, 8.0, 0.1);
	 setDistance(1, 3, 5.0, 0.1);
	 setDistance(1, 4, 4.0, 0.1);
	 setDistance(2, 6, 7.0, 0.1);
	 setDistance(3, 2, 3.0, 0.1);
	 setDistance(3, 5, 5.0, 0.1);
	 setDistance(4, 3, 2.0, 0.1);
	 setDistance(5, 6, 6.0, 0.1);
	 setDistance(6, 1, 7.0, 0.1);
	 setDistance(6, 4, 5.0, 0.1);*/
//	Config::setDistance(1, 2, 8.0, 0.1);
//	Config::setDistance(1, 4, 5.0, 0.1);
//	Config::setDistance(2, 3, 4.0, 0.1);
//	Config::setDistance(4, 5, 7.0, 0.1);
//	Config::setDistance(4, 6, 7.0, 0.1);
//	Config::setDistance(3, 2, 3.0, 0.1);
	for (map<string, int>::iterator it = Config::roadMaps.begin(); it != Config::roadMaps.end(); it++) {
		string curEdge = it->first;
		list<string> edges = cfg->getNextEdges(curEdge);
		for (list<string>::iterator lit = edges.begin(); lit != edges.end(); lit++) {
			Config::setDistance(Config::roadMaps[curEdge],
					Config::roadMaps[*lit],
					cfg->getEdgeLength(*lit), 0.1);
		}
	}
}

//���»�����Ϣ��
void CShortPath::UpdateTrial() {
//��ʱ���飬�����ֻ�������������м������µ���Ϣ��
	double dbTempAry[Config::N_CITY_COUNT][Config::N_CITY_COUNT];
	memset(dbTempAry, 0, sizeof(dbTempAry)); //��ȫ������Ϊ0

//���������ӵ���Ϣ��,���浽��ʱ������
	int m = 0;
	int n = 0;
	for (int i = 0; i < Config::N_ANT_COUNT; i++) //����ÿֻ�������µ���Ϣ��
			{
		for (unsigned int j = 1; j != m_cAntAry[i]->movedPath.size(); j++) {
			m = m_cAntAry[i]->movedPath[j];
			n = m_cAntAry[i]->movedPath[j - 1];
			dbTempAry[n][m] = dbTempAry[n][m]
					+ Config::DBQ / m_cAntAry[i]->m_dbPathLength;
		}
	}

//==================================================================
//���»�����Ϣ��
	for (int i = 0; i < Config::N_CITY_COUNT; i++) {
		for (int j = 0; j < Config::N_CITY_COUNT; j++) {
			Config::g_Trial[i][j] = Config::g_Trial[i][j] * Config::ROU
					+ dbTempAry[i][j]; //���µĻ�����Ϣ�� = �������Ϣ�� + �����µ���Ϣ��
		}
	}

}

void CShortPath::Search(string start, string end) {

//�ڵ��������ڽ���ѭ��
	for (int i = 0; i < Config::N_IT_COUNT; i++) {
		//ÿֻ��������һ��
		for (int j = 0; j < Config::N_ANT_COUNT; j++) {
			m_cAntAry[j] = new CAnt();
			int istart = cfg->roadMaps[start];
			int iend = cfg->roadMaps[end];
			m_cAntAry[j]->Search(istart, iend);
		}

		//���»�����Ϣ��
		UpdateTrial();

		//������ѽ��
		for (int j = 0; j < Config::N_ANT_COUNT; j++) {
			if (m_cAntAry[j]->m_dbPathLength < m_cBestAnt->m_dbPathLength) {
				m_cBestAnt = m_cAntAry[j];
			} else {
				delete m_cAntAry[j];
			}
		}
	}
	route = m_cBestAnt->displayPath();
}
