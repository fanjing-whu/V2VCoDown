//
// Author: Emin Ilker Cetinbas (niw3_at_yahoo_d0t_com)
// Copyright (C) 2005 Emin Ilker Cetinbas
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

package simpleveins.mobility;

import org.mixim.base.modules.BaseMobility;

//
// This is a linear mobility model with speed, angle and acceleration parameters.
// Angle only changes when the mobile node hits a wall: then it reflects
// off the wall at the same angle.
//
// NOTE: Does not yet support 3-dimensional movement.
// @see ConnectionManager
// @author Emin Ilker Cetinbas
//
simple HorizontalMobility extends BaseMobility
{
    parameters:
        @class(HorizontalMobility);
        bool debug = default(false); // debug switch
        double angle = default(180);
        double speed @unit(mps) = default(uniform(25mps,33.333mps)); // speed of the host (in m/s)
        double acceleration @unit(mpss) = default(uniform(0.139mpss,0.278mpss)); // acceleration of linear motion (m/s2)
        double speedOffset @unit(mps) = default(uniform(1.389mps,2.778mps));
        double updateInterval @unit(s) = default(0.1s); // time interval to update the hosts position (seconds)
}

