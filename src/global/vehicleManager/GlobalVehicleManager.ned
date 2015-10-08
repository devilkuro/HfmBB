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

package smartcity.flowreserved.global.vehicleManager;

//
// TODO auto-generated module
//
simple GlobalVehicleManager
{
    parameters:
        @class(GlobalVehicleManager);
        
        int carNumLimit = default(2000);	// the max number of generated cars
        string carFlowXMLPath = default("");	// the file of the car 
        string carVTypeXMLPath = default("");	// the file includes vehicle types
        bool generateNewXMLFile = default(true);	// if true, will generate new xml file at initializing phase.
        double maxCarFlowRate = default(0.6);	// the max car flow rate for each spawn point. the unit is num per second
        // since the traffic light only allow the cars through the road cross in 1/4 time for non-right direction
        // the maxCarFlowRate should not beyond the max throughput too much.
        // the max throughput can be calculate by the average speed divided by safe distance.
        // So it uses 12mps/5m*(1/4)=0.6 cars per second here
        double minCarFlowRate = default(0);
        double carSpawnJudgeInterval @unit(s) = default(0.1s);	// the judge interval to decide to spawn one car or not (for each possible spawn position)
        double carSpawnPeriod @unit(s) = default(2h);	// since the distribution of the car generating rate is sine distribution this value may be twice of the rush hours.
        double carSpawnOffset @unit(s) = default(15min);	// the actual spwan time of each car can before or after the set time within this value.
        // carSpawnOffset may move into the generator and bind to each car.
}
