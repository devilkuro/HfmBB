#ifndef SHORT_PATH_H
#define SHORT_PATH_H

#include "Ant.h"
#include "Config.h"
//#include "Config.h"

class CShortPath
{
public:
	CShortPath(void);
	~CShortPath(void);

public:
	CAnt** m_cAntAry;	//蚂蚁数组
	CAnt* m_cBestAnt;				//定义一个蚂蚁变量，用来保存搜索过程中的最优结果
									//该蚂蚁不参与搜索，只是用来保存最优结果
	list<string> route;
	int startPos;		//起始位置
	int endPos;			//终点位置
public:
	void InitData(Config* cfg);	//初始化数据
	void Search();		//开始搜索
	void UpdateTrial(); //更新环境信息素
};
#endif
