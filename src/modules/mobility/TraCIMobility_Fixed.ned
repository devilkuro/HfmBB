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

package smartcity.flowreserved.modules.mobility;
import org.mixim.base.modules.BaseMobility;

//
// TODO auto-generated module
//
simple TraCIMobility_Fixed extends BaseMobility
{
    parameters:
		@class(TraCIMobility_Fixed);
        @display("i=block/cogwheel");
        bool debug = default(false);  // debug switch
        double antennaPositionOffset @unit("m") = default(0.0m); // position offset of the antenna of the front of the car
        int accidentCount = default(0);  // number of accidents
        double accidentStart @unit("s") = default(uniform(30s,60s));  // time until first accident, relative to departure time
        volatile double accidentDuration @unit("s") = default(uniform(30s,60s));  // duration of accident
        volatile double accidentInterval @unit("s") = default(uniform(30s,60s));  // time between accidents
}
