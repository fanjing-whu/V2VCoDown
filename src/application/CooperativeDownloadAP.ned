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

package simpleveins.application;

import org.mixim.base.modules.IBaseApplLayer;
//
// TODO auto-generated module
//
simple CooperativeDownloadAP like IBaseApplLayer
{
    
    parameters:
    	int  headerLength @unit("bit");   // length of the application message header (in bits)
    	double frameInterval @unit("s") = default(0.1s);
    	double APframeSize @unit("kb") = default(150kb);
    	double maxDALength @unit("m") = default(8000m);
    	double maxAPRadius @unit("m") = default(400m);
    	double minCarSpeed @unit("mps") = default(15mps);
    	double APDLSpeed @unit("kbps") = default(1500kbps);
    	double CarDLSpeed @unit("kbps") = default(500kbps);
    	double carComRadius @unit("m") = default(250m);
    	// TODO add from above.
    gates:
    	input lowerLayerIn; // from network layer
        output lowerLayerOut; // to network layer
        input lowerControlIn; // control from network layer
        output lowerControlOut; // control to network layer        
}