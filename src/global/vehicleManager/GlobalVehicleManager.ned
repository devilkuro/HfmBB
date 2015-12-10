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
  		bool disableSinFix = default(false);
  		bool enableBurst= default(false);
  		int nBurstNum= default(10);
        string carFlowXMLPath = default("");	// the file of the car 
        string carVTypeXMLPath = default("");	// the file includes vehicle types
        bool generateNewXMLFile = default(true);	// if true, will generate new xml file at initializing phase.
        double maxCarFlowRate = default(0.5);	// the max car flow rate for each spawn point. the unit is num per second
        // since the traffic light only allow the cars through the road cross in 1/4 time for non-right direction
        // the maxCarFlowRate should not beyond the max throughput too much.
        // the max throughput can be calculate by the average speed divided by safe distance.
        // So it uses 12mps/5m*(1/4)=0.6 cars per second here
        double minCarFlowRate = default(0);
        double maxFreeCarFlowRate = default(0.2);
        double minFreeCarFlowRate = default(0.1);
        double carSpawnJudgeInterval @unit(s) = default(1s);	// the judge interval to decide to spawn one car or not (for each possible spawn position)
        double carSpawnPeriod @unit(s) = default(2h);	// since the distribution of the car generating rate is sine distribution this value may be twice of the rush hours.
        double carSpawnOffset @unit(s) = default(15min);	// the actual spwan time of each car can before or after the set time within this value.
        // carSpawnOffset may move into the generator and bind to each car.
		bool useCarSpawnOffset = default(true);
		double carSpawnStartTime @unit(s) = default(16min);	// the target car flow will generate after this time (in xml file).
        double carSpawnTimeLimit @unit(s) = default(carSpawnPeriod*3);	// the max time from starting generating cars to ending it.
        int carNumLimit = default(carSpawnPeriod/1s);	// the max number of generated cars (default 3 spawn points)
        double reverseCarFlowRate = default(0.5);
}
