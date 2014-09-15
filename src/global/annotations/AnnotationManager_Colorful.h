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

#ifndef __SMARTCITYFLOWRESERVED_ANNOTATIONMANAGER_COLORFUL_H_
#define __SMARTCITYFLOWRESERVED_ANNOTATIONMANAGER_COLORFUL_H_

#include <omnetpp.h>
#include "AnnotationManager.h"
/**
 *
 */
class AnnotationManager_Colorful : public AnnotationManager {
public:
    class Line_Colorful : public AnnotationManager::Line {
    public:
        Line_Colorful(Coord p1, Coord p2, std::string color) :
                Line(p1, p2, color) {
        }
        virtual ~Line_Colorful() {
        }
        virtual void setColor(std::string color);
    protected:
        friend class AnnotationManager;
        friend class AnnotationManager_Colorful;

    };

    AnnotationManager_Colorful::Line_Colorful* drawLine_Colorful(Coord p1, Coord p2, std::string color,
            Group* group = 0);
};
class AnnotationManager_ColorfulAccess {
public:
    AnnotationManager_Colorful* getIfExists() {
        return FindModule<AnnotationManager_Colorful*>::findGlobalModule();
    }
    ;
};
#endif
