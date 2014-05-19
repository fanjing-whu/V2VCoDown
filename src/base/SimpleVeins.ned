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

package simpleveins.base;

import org.mixim.base.connectionManager.ConnectionManager;
import org.mixim.base.modules.BaseWorldUtility;;
import simpleveins.globalModuls.GlobalStatistics;
import simpleveins.car.Car;
//
// TODO auto-generated type
//
network SimpleVeins
{
    parameters:
        double playgroundSizeX @unit(m); // x size of the area the nodes are in (in meters)
        double playgroundSizeY @unit(m); // y size of the area the nodes are in (in meters)
        double playgroundSizeZ @unit(m); // z size of the area the nodes are in (in meters)

        @display("bgb=$playgroundSizeX,$playgroundSizeY,white;bgp=0,0");

    submodules:
        car[numHosts]: Car {
            @display("p=137,95");
        }
        connectionManager: ConnectionManager {
            parameters:
                @display("p=201,0;i=abstract/multicast;is=s");
        }
        world: BaseWorldUtility {
            parameters:
                playgroundSizeX = playgroundSizeX;
                playgroundSizeY = playgroundSizeY;
                playgroundSizeZ = playgroundSizeZ;
                @display("p=280,0;i=misc/globe;is=s");
        }
        globalStatistics: GlobalStatistics {
            parameters:
                @display("p=201,0;i=block/blackboard;is=s");
        }
    connections allowunconnected:
}
