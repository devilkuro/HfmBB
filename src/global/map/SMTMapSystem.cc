//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "SMTMapSystem.h"
Define_Module(SMTMapSystem)
SMTMapSystem::SMTMapSystem() {
}

SMTMapSystem::~SMTMapSystem() {

}

void SMTMapSystem::initialize(int stage) {
    GlobalMapSystem::initialize(stage);
    if(stage == 1){
        list<string> allEgetList = getAllEdges();
        // 生成主要道路列表
        for(list<string>::iterator it = allEgetList.begin();it!=allEgetList.end();it++){
            if((*it)[0]!=':'){
                normalEdgeList.push_back(*it);
            }
        }
        // 生成主要道路连接拓扑
        ganerateMapTopology();
        // todo 改为由xml读入？感觉有点蛋疼啊，先写静态的吧。
        string xmlname = "matrixlog";
        queueMap["2/4to2/2_0"] = SMTCarInfoQueue("2/4to2/2_0",xmlname,577.30,5);
        // todo
    }
}

void SMTMapSystem::ganerateMapTopology() {
    // todo 暂时什么都没做，之后需要改为由xml录入
}
