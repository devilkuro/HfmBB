#ifndef   MY_ANode_Util       //如果没有定义这个宏
#define   MY_ANode_Util       //定义这个宏

#include <iostream>
#include <math.h>
#include <cstdlib>
#include<string>
#include <map>
#include <list>
#include "ANode.h"
using std::string;
using std::list;
using std::map;
void count(ANode *node, ANode *eNode);
void countG(ANode *node, ANode *eNode);
void countH(ANode *node, ANode *eNode);
void countF(ANode *node);
bool contains(list<ANode*> list, ANode *res);
ANode* getANodeElem(list<ANode*> list,int index);
ANode* getANode(map<ANode*, list<ANode*> > roadMap, string edgeName);
bool compA(ANode *o1, ANode *o2);
#endif
