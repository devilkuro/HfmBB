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

#ifndef SMTMOBILITY_H_
#define SMTMOBILITY_H_

#include "TraCIMobility_Fixed.h"

class SMTMobility : public TraCIMobility_Fixed {
public:
    SMTMobility();
    virtual ~SMTMobility();

protected:
    // overload these function in different mobility
    virtual void processAfterRouting();
    virtual void statisticAtFinish();
    virtual void processAtRouting();
    virtual void processWhenChangeRoad();
    virtual void processWhenInitializingRoad();
    virtual void processWhenNextPosition();

};

#endif /* SMTMOBILITY_H_ */
