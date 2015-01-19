#ifndef   MY_DNode_Util       //如果没有定义这个宏
#define   MY_DNode_Util       //定义这个宏
#include <iostream>
#include <math.h>
#include <cstdlib>
#include<string>
#include <map>
#include <list>
#include "DNode.h"
#include "Key.h"
using std::string;
using std::list;
using std::map;
void setRoadWeight(DNode *node, int flag);
void countH(DNode *node, DNode *sNode);
void setKey(DNode *node);
bool compD(DNode *o1, DNode *o2);
DNode* getDNodeElem(list<DNode*> list, int index);
bool contains(list<DNode*> li, DNode *res);
#endif
