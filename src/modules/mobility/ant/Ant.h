#ifndef   ANT_H       //如果没有定义这个宏
#define   ANT_H       //定义这个宏

#include <vector>
#include <algorithm>
#include "Config.h"
#include "Util.h"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "mem.h"
using namespace std;

class CAnt
{
public:
	CAnt(void);
	CAnt(int startRoadNumber,int endRoadNumber);
	~CAnt(void);

public:

	int startRoadNumber;//起点街道
	int endRoadNumber; //终点街道
	int m_nCurRoadNumber; //当前所在街道

	vector<int> movedPath;//蚂蚁走过的路径
	double m_dbPathLength; //蚂蚁走过的路径长度

public:
	vector<int> setAllowRoad();
	int ChooseNextRoad();   //选择下一个城市
	void Init(int startRoadNumber,int endRoadNumber); //初始化
	void Search(int startRoadNumber,int endRoadNumber);			//搜索路径
	void CalPathLength();   //计算蚂蚁走过的路径长度
	list<string> displayPath();
};

#endif

