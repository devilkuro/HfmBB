/*
 * ACOTest.cpp
 *
 *  Created on: 2015-1-5
 *      Author: bobo
 */

#include "ACOTest.h"

// ���캯��
ACOTest::ACOTest() {
    // TODO Auto-generated constructor stub
    ordAnts = new Ant*[ACO::N_ANT_COUNT];
    this->badestAnt = new Ant();
    this->bestAnt = new Ant();
    this->dbadestAnt = badestAnt;
    this->dbestAnt = bestAnt;
    for (int var = 0; var < ACO::N_ANT_COUNT; ++var) {
        ordAnts[var] = NULL;
    }
}

ACOTest::~ACOTest() {
    // TODO Auto-generated destructor stub
    for(int i = 0; i < ACO::N_ANT_COUNT; i++){
        if (ordAnts[i] == NULL) {
            delete ordAnts[i];
        }
    }
    delete dbadestAnt;
    delete dbestAnt;
    delete ordAnts;
}

// ��ʼ������
void ACOTest::initData(string startRoad, string endRoad) {
    ACO aco;
    aco.setRoadMap(0.1);
    AntNode *sAntNode = aco.getAntNode(startRoad);
    AntNode *eAntNode = aco.getAntNode(endRoad);
    AntNode *curAntNode = sAntNode;
    for(int i = 0; i < ACO::N_ANT_COUNT; i++){
        Ant *temp = new Ant(sAntNode, eAntNode, curAntNode);
        ordAnts[i] = temp;
    }
    bestAnt->setMovedPathLength(ACO::DB_MAX); // ��ʼ����õ����ϵ�����
    badestAnt->setMovedPathLength(ACO::DB_MIN); // ��ʼ���������ϵ�����
}
// �����ֲ����ź���
void ACOTest::reduceBestPathTrial() {
    for(int i = 0; i < ACO::N_ANT_COUNT; i++){
        for(unsigned int j = 0; j != ordAnts[i]->getMovedPath().size(); j++){
            if(ordAnts[i]->getMovedPathLength() == bestAnt->getMovedPathLength()){
                AntNode *temp = ordAnts[i]->getMovedPath()[j];
                temp->setPheromone(temp->getPheromone() * 0.5);
            }else if(ordAnts[i]->getMovedPathLength() > bestAnt->getMovedPathLength()){
                AntNode *temp = ordAnts[i]->getMovedPath()[j];
                temp->setPheromone(temp->getPheromone() * 0.1);
            }
        }
    }
    map<AntNode*, list<AntNode*> >::iterator iter;
    for(iter = ACO::roadMap.begin(); iter != ACO::roadMap.end(); iter++){
        AntNode *key = iter->first;
        if(key->getCount() == 0 && key->getPheromone() != 0.0){
            key->setPheromone(key->getPheromone() * 5);
        }
    }
    setMaxMinTrial();
}
// ����ÿ�ε������λ���ϵ�������Ϣ��
void ACOTest::updateBadestTrial() {
    vector<AntNode*>::iterator iter;

    for(unsigned int i = 0; i < badestAnt->getMovedPath().size(); ++i){
        AntNode *badestTemp = badestAnt->getMovedPath().at(i);
        badestTemp->setPheromone(badestTemp->getPheromone() * ACO::ROU);
    }
    setMaxMinTrial();
}
// ���»�����Ϣ��
void ACOTest::updateTrial() {
    map<AntNode*, double> tempTrial;
    for(int i = 0; i < ACO::N_ANT_COUNT; i++){
        vector<AntNode*>::iterator iter;
        for(iter = ordAnts[i]->getMovedPath().begin(); iter != ordAnts[i]->getMovedPath().end(); iter++){
            AntNode* temp = *iter;
            double pheromone = ACO::DBQ / ordAnts[i]->getMovedPathLength();
            map<AntNode*, double>::iterator iter = tempTrial.find(temp);
            if(iter != tempTrial.end()){
                double tempPheromone = tempTrial[temp];
                tempTrial[temp] = tempPheromone + pheromone;
            }else{
                tempTrial[temp] = pheromone;
            }
        }
    }
    map<AntNode*, list<AntNode*> >::iterator iter;
    for(iter = ACO::roadMap.begin(); iter != ACO::roadMap.end(); iter++){
        AntNode *key = iter->first;
        map<AntNode*, double>::iterator it = tempTrial.find(key);
        if(it != tempTrial.end()){
            double preTrial = key->getPheromone() * ACO::ROU;
            double curTrial = tempTrial[key];
            tempTrial[key] = preTrial + curTrial;
        }else{
            double curTrial = key->getPheromone() * ACO::ROU;
            tempTrial[key] = curTrial;
        }
        if(key->getPheromone() != 0.0){ // ��С�����Ϣ��ϵͳ
            if(key->getPheromone() < 0.1){
                key->setPheromone(0.1);
            }else if(key->getPheromone() > 1.0){
                key->setPheromone(1.0);
            }
        }
    }
}
// ������С���ϵͳ
void ACOTest::setMaxMinTrial() {
    map<AntNode*, list<AntNode*> >::iterator iter;
    for(iter = ACO::roadMap.begin(); iter != ACO::roadMap.end(); iter++){
        AntNode *key = iter->first;
        if(key->getPheromone() != 0.0){
            if(key->getPheromone() < 0.1){
                key->setPheromone(0.1);
            }else if(key->getPheromone() > 1.0){
                key->setPheromone(1.0);
            }
        }
    }
}
// ����ÿ�ε������λ���ϵ�������Ϣ��
void ACOTest::updateBestTrial() {
    vector<AntNode*>::iterator iter;
    for(unsigned int i = 0; i < bestAnt->getMovedPath().size(); ++i){
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

// ������ʼ����
void ACOTest::search(string startRoad, string endRoad) {
    int continuousTime = 0; // ��������� �ж��������ٴ�����·��û�и���
    double p = 0.1; // �������������ܵ����İٷ�֮��ʮ ���ж�Ϊ����ֲ�����
    for(int i = 0; i != ACO::N_IT_COUNT; i++){
        for(int j = 0; j < ACO::N_ANT_COUNT; j++){
            ordAnts[j]->move(); // ���ĺ��� ���Ͽ�ʼ�ƶ�����
            if(ordAnts[0]->getMovedPath().size() == 0){
                cout << "No way to the road!" << endl;
                return;
            }
        }
        Ant *localBestAnt;
        localBestAnt = ordAnts[0];
        for(int j = 0; j < ACO::N_ANT_COUNT; j++){
            if(ordAnts[j]->getMovedPathLength() < localBestAnt->getMovedPathLength()){
                localBestAnt = ordAnts[j];
            }
            if(badestAnt->getMovedPathLength() < ordAnts[j] // �ҳ�һ�ε����е��������
            ->getMovedPathLength()){
                badestAnt = ordAnts[j]; // ��¡һ�ε������������
            }
        }
        if(localBestAnt->getMovedPathLength() < bestAnt->getMovedPathLength()){
            bestAnt = localBestAnt;
            continuousTime = 0;
        }else{
            continuousTime++;
        }
        if(continuousTime >= p * ACO::N_IT_COUNT){ // ���������ô��ε���û�и�������
            reduceBestPathTrial(); // ���ٵ�ǰ����·������Ϣ��
        }
        int count = (int) (ACO::N_IT_COUNT * ACO::P * 5);
        if(i < count){
            updateBestTrial(); // ��������·����Ϣ��
            updateBadestTrial(); // �������·����Ϣ��
        }else if(i >= count){
            updateTrial(); // ������Ϣ��
        }
    }
    route = bestAnt->displayPath();  //��ӡ����·��
}

void ACOTest::seekRoute(string startRoad, string endRoad) {
    {
        ACO aco;
        AntNode *sAntNode = aco.getAntNode(startRoad);
        AntNode *eAntNode = aco.getAntNode(endRoad);
        AntNode *curAntNode = sAntNode;
        for(int i = 0; i < ACO::N_ANT_COUNT; i++){
            Ant *temp = new Ant(sAntNode, eAntNode, curAntNode);
            ordAnts[i] = temp;
        }
        bestAnt->setMovedPathLength(ACO::DB_MAX); // ��ʼ����õ����ϵ�����
        badestAnt->setMovedPathLength(ACO::DB_MIN); // ��ʼ���������ϵ�����
    }
    {
        // search
        search(startRoad, endRoad);
    }
    {
        list<string> route;
        vector<AntNode*>::iterator iter;
        for(iter = bestAnt->movedPath.begin(); iter != bestAnt->movedPath.end(); iter++){
            route.push_back((*iter)->getRoadName());
        }
        setRoute(route);
    }
}

void ACOTest::init(GlobalMapSystem* gms) {

    this->gms = gms;
    {
        // initData
        ACO aco;
        //aco.setRoadMap(0.1);
        {
            // set road map
            list<string> edges = getAllEdges();
            for(list<string>::iterator it = edges.begin(); it != edges.end(); ++it){
                double weight = getTravelTime(*it);
                list<BSRF_Coord> posList = getShapeByEdge(*it);
                list<BSRF_Coord>::iterator posit = posList.end();
                posit--;
                BSRF_Coord pos = *(posit);
                double x = pos.x;
                double y = pos.y;
                if(weight == 0){
                    weight = getEdgeLength(*it) / 14;
                }
                AntNode *road = new AntNode(*it, weight, x, y, 0.1);
                list<AntNode*> outEdges;
                aco.roadMap[road] = outEdges;
            }
            for(list<string>::iterator it = edges.begin(); it != edges.end(); ++it){
                list<AntNode*> outEdges;
                list<string> outEdgesStr = getNextEdges(*it);
                for(list<string>::iterator edgeit = outEdgesStr.begin(); edgeit != outEdgesStr.end(); ++edgeit){
                    outEdges.push_back(aco.getAntNode(*edgeit));
                }
                aco.roadMap[aco.getAntNode(*it)] = outEdges;
            }
        }
    }
}
