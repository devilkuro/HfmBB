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

#include "AnnotationManager_Colorful.h"

Define_Module(AnnotationManager_Colorful)

AnnotationManager_Colorful::Line_Colorful* AnnotationManager_Colorful::drawLine_Colorful(Coord p1, Coord p2, std::string color, Group* group) {
    Line_Colorful* l = new Line_Colorful(p1, p2, color);
    l->group = group;

    annotations.push_back(l);

    if (par("draw"))
        show(l);

    return l;
}

void AnnotationManager_Colorful::setColor(AnnotationManager_Colorful::Annotation* annotation, std::string color) {
    // FIXME - just implement setting lines' color for now
    if (const Line_Colorful* l = dynamic_cast<const Line_Colorful*>(annotation)) {
        for (std::list<cModule*>::iterator it = l->dummyObjects.begin(); it != l->dummyObjects.end(); it++) {
            (*it)->getDisplayString().setTagArg("b", 4, color.c_str());
        }
    }
}
