/*
 * ACOTest.cpp
 *
 *  Created on: 2015-1-5
 *      Author: bobo
 */

#include "ACOTest.h"

// 构造函数
ACOTest::ACOTest() {
	// TODO Auto-generated constructor stub
	ordAnts = new Ant*[ACO::N_ANT_COUNT];
	this->badestAnt = new Ant();
	this->bestAnt = new Ant();
}

ACOTest::~ACOTest() {
	// TODO Auto-generated destructor stub
	for (int i = 0; i < ACO::N_ANT_COUNT; i++) {
		delete ordAnts[i];
	}
	delete ordAnts;
}

// 初始化蚂蚁
void ACOTest::initData(string startRoad, string endRoad) {
	ACO aco;
	aco.setRoadMap(0.1);
	AntNode *sAntNode = aco.getAntNode(startRoad);
	AntNode *eAntNode = aco.getAntNode(endRoad);
	AntNode *curAntNode = sAntNode;
	for (int i = 0; i < ACO::N_ANT_COUNT; i++) {
		Ant *temp = new Ant(sAntNode, eAntNode, curAntNode);
		ordAnts[i] = temp;
	}
	bestAnt->setMovedPathLength(ACO::DB_MAX); // 初始化最好的蚂蚁的数据
	badestAnt->setMovedPathLength(ACO::DB_MIN); // 初始化最差的蚂蚁的数据
}
// 跳出局部最优函数
void ACOTest::reduceBestPathTrial() {
	for (int i = 0; i < ACO::N_ANT_COUNT; i++) {
		for (unsigned int j = 0; j != ordAnts[i]->getMovedPath().size(); j++) {
			if (ordAnts[i]->getMovedPathLength()
					== bestAnt->getMovedPathLength()) {
				AntNode *temp = ordAnts[i]->getMovedPath()[j];
				temp->setPheromone(temp->getPheromone() * 0.5);
			} else if (ordAnts[i]->getMovedPathLength()
					> bestAnt->getMovedPathLength()) {
				AntNode *temp = ordAnts[i]->getMovedPath()[j];
				temp->setPheromone(temp->getPheromone() * 0.1);
			}
		}
	}
	map<AntNode*, list<AntNode*> >::iterator iter;
	for (iter = ACO::roadMap.begin(); iter != ACO::roadMap.end(); iter++) {
		AntNode *key = iter->first;
		if (key->getCount() == 0 && key->getPheromone() != 0.0) {
			key->setPheromone(key->getPheromone() * 5);
		}
	}
	setMaxMinTrial();
}
// 更新每次迭代最差位置上的蚂蚁信息素
void ACOTest::updateBadestTrial() {
	vector<AntNode*>::iterator iter;

	for (unsigned int i = 0; i < badestAnt->getMovedPath().size(); ++i) {
		AntNode *badestTemp = badestAnt->getMovedPath().at(i);
		badestTemp->setPheromone(badestTemp->getPheromone() * ACO::ROU);
	}
	setMaxMinTrial();
}
// 更新环境信息素
void ACOTest::updateTrial() {
	map<AntNode*, double> tempTrial;
	for (int i = 0; i < ACO::N_ANT_COUNT; i++) {
		vector<AntNode*>::iterator iter;
		for (iter = ordAnts[i]->getMovedPath().begin();
				iter != ordAnts[i]->getMovedPath().end(); iter++) {
			AntNode* temp = *iter;
			double pheromone = ACO::DBQ / ordAnts[i]->getMovedPathLength();
			map<AntNode*, double>::iterator iter = tempTrial.find(temp);
			if (iter != tempTrial.end()) {
				double tempPheromone = tempTrial[temp];
				tempTrial[temp] = tempPheromone + pheromone;
			} else {
				tempTrial[temp] = pheromone;
			}
		}
	}
	map<AntNode*, list<AntNode*> >::iterator iter;
	for (iter = ACO::roadMap.begin(); iter != ACO::roadMap.end(); iter++) {
		AntNode *key = iter->first;
		map<AntNode*, double>::iterator it = tempTrial.find(key);
		if (it != tempTrial.end()) {
			double preTrial = key->getPheromone() * ACO::ROU;
			double curTrial = tempTrial[key];
			tempTrial[key] = preTrial + curTrial;
		} else {
			double curTrial = key->getPheromone() * ACO::ROU;
			tempTrial[key] = curTrial;
		}
		if (key->getPheromone() != 0.0) { // 最小最大信息素系统
			if (key->getPheromone() < 0.1) {
				key->setPheromone(0.1);
			} else if (key->getPheromone() > 1.0) {
				key->setPheromone(1.0);
			}
		}
	}
}
// 设置最小最大系统
void ACOTest::setMaxMinTrial() {
	map<AntNode*, list<AntNode*> >::iterator iter;
	for (iter = ACO::roadMap.begin(); iter != ACO::roadMap.end(); iter++) {
		AntNode *key = iter->first;
		if (key->getPheromone() != 0.0) {
			if (key->getPheromone() < 0.1) {
				key->setPheromone(0.1);
			} else if (key->getPheromone() > 1.0) {
				key->setPheromone(1.0);
			}
		}
	}
}
// 更新每次迭代最好位置上的蚂蚁信息素
void ACOTest::updateBestTrial() {
	vector<AntNode*>::iterator iter;
	for (unsigned int i = 0; i < bestAnt->getMovedPath().size(); ++i) {
		AntNode *bestTemp = bestAnt->getMovedPath().at(i);
		double pheromone = ACO::DBQ / bestAnt->getMovedPathLength();
		bestTemp->setPheromone(bestTemp->getPheromone() + pheromone);
	}

	setMaxMinTrial();
}

Ant* ACOTest::getBadestAnt() {
	return this->badestAnt;
}

void ACOTest::setBadestAnt(Ant* badestAnt) {
	this->badestAnt = badestAnt;
}

Ant* ACOTest::getBestAnt() {
	return this->bestAnt;

}

void ACOTest::setBestAnt(Ant* bestAnt) {
	this->bestAnt = bestAnt;
}

Ant** ACOTest::getOrdAnts() {
	return this->ordAnts;
}

void ACOTest::setOrdAnts(Ant** ordAnts) {
	this->ordAnts = ordAnts;
}

// 开迭代始搜索
void ACOTest::search(string startRoad, string endRoad) {
	int continuousTime = 0; // 定义计数器 判断连续多少次最优路径没有更新
	double p = 0.1; // 连续迭代超过总迭代的百分之三十 则判断为陷入局部最优
	for (int i = 0; i != ACO::N_IT_COUNT; i++) {
		for (int j = 0; j < ACO::N_ANT_COUNT; j++) {
			ordAnts[j]->move(); // 核心函数 蚂蚁开始移动搜索
			if (ordAnts[0]->getMovedPath().size() == 0) {
				cout << "No way to the road!" << endl;
				return;
			}
		}
		Ant *localBestAnt;
		for (int j = 0; j < ACO::N_ANT_COUNT; j++) {
			localBestAnt = ordAnts[0];
			if (ordAnts[j]->getMovedPathLength()
					< localBestAnt->getMovedPathLength()) {
				localBestAnt = ordAnts[j];
			}
			if (badestAnt->getMovedPathLength() < ordAnts[j] // 找出一次迭代中的最差蚂蚁
			->getMovedPathLength()) {
				badestAnt = ordAnts[j]; // 克隆一次迭代中最差蚂蚁
			}
		}
		if (localBestAnt->getMovedPathLength()
				< bestAnt->getMovedPathLength()) {
			bestAnt = localBestAnt;
			continuousTime = 0;
		} else {
			continuousTime++;
		}
		if (continuousTime >= p * ACO::N_IT_COUNT) { // 如果经历这么多次迭代没有更新最优
			reduceBestPathTrial(); // 减少当前这条路径的信息素
		}
		int count = (int) (ACO::N_IT_COUNT * ACO::P * 5);
		if (i < count) {
			updateBestTrial(); // 更新最优路径信息素
			updateBadestTrial(); // 更新最差路径信息素
		} else if (i >= count) {
			updateTrial(); // 更新信息素
		}
	}
	route = bestAnt->displayPath();  //打印最优路径
}

