/*
 * ACOTest.h
 *
 *  Created on: 2015-1-5
 *      Author: bobo
 */

#ifndef ACOTEST_H_
#define ACOTEST_H_
#include <string>
#include "ACO.h"
#include "Ant.h"
#include "BaseSeekRouteFunc.h"
using namespace std;
class ACOTest: public BaseSeekRouteFunc{
private:
	Ant** ordAnts; // 普通蚂蚁数组
	Ant* bestAnt; // 定义一个蚂蚁变量，用来保存搜索过程中的最优结果
	Ant* badestAnt; // 保存最差蚂蚁

    Ant* dbestAnt; // 定义一个蚂蚁变量，用来保存搜索过程中的最优结果
    Ant* dbadestAnt; // 保存最差蚂蚁

public:
	list<AntNode*> route;
	ACOTest();
	virtual ~ACOTest();
	Ant* getBadestAnt();
	void setBadestAnt(Ant* badestAnt);
	Ant* getBestAnt();
	void setBestAnt(Ant* bestAnt);
	Ant** getOrdAnts();
	void setOrdAnts(Ant** ordAnts);
	void initData(string startRoad, string endRoad); // 初始化蚂蚁
	void reduceBestPathTrial(); // 跳出局部最优函数
	void updateBestTrial();	// 更新每次迭代最好位置上的蚂蚁信息素
	void updateBadestTrial(); // 更新每次迭代最差位置上的蚂蚁信息素
	void updateTrial(); // 更新环境信息素
	void setMaxMinTrial(); // 设置最小最大系统
	void search(string startRoad, string endRoad); // 开迭代始搜索
	void seekRoute(string startRoad, string endRoad);
	void init(GlobalMapSystem* gms);
};

#endif /* ACOTEST_H_ */
