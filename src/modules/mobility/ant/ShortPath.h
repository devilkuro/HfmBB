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
	CAnt** m_cAntAry;	//��������
	CAnt* m_cBestAnt;				//����һ�����ϱ����������������������е����Ž��
									//�����ϲ�����������ֻ�������������Ž��
	list<string> route;
	int startPos;		//��ʼλ��
	int endPos;			//�յ�λ��
public:
	void InitData(Config* cfg);	//��ʼ������
	void Search();		//��ʼ����
	void UpdateTrial(); //���»�����Ϣ��
};
#endif
