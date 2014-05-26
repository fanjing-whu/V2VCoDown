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

#ifndef HORIZONTALMOBILITY_H_
#define HORIZONTALMOBILITY_H_

#include "MiXiMDefs.h"
#include "BaseMobility.h"
class HorizontalMobility: public BaseMobility {
public:
    HorizontalMobility();
    virtual ~HorizontalMobility();
protected:
    double acceleration;   ///< acceleration of linear motion
    double speed;
    double speedOffset;
    double angle;
    double maxSpeed,minSpeed;

    /** @brief always stores the last step for position display update */
    Coord stepTarget;

public:
    //Module_Class_Members( LinearMobility, BaseMobility, 0 );

    /** @brief Initializes mobility model parameters.*/
    virtual void initialize(int);

protected:
    /** @brief Move the host*/
    virtual void makeMove();

    virtual void fixIfHostGetsOutside();
};

#endif /* HORIZONTALMOBILITY_H_ */
