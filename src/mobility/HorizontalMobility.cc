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

#include "HorizontalMobility.h"

#include <FWMath.h>

Define_Module(HorizontalMobility);
HorizontalMobility::HorizontalMobility() {
    // TODO Auto-generated constructor stub

}

HorizontalMobility::~HorizontalMobility() {
    // TODO Auto-generated destructor stub
}

void HorizontalMobility::initialize(int stage)
{
    BaseMobility::initialize(stage);

    debugEV << "initializing HorizontalMobility stage " << stage << endl;

    if (stage == 0){
        moveDir = par("moveTowardsRight").boolValue()?1.0:-1.0;
        move.setSpeed(par("speed").doubleValue());
        acceleration = par("acceleration");
    }
    else if(stage == 1){
        stepTarget = move.getStartPos();

        if(!world->use2D()) {
            opp_warning("This mobility module does not yet support 3 dimensional movement."\
                        "Movements will probably be incorrect.");
        }
        if(!world->useTorus()) {
            opp_warning("You are not using a torus (parameter \"useTorus\" in"\
                        "BaseWorldUtility module) playground but this mobility"\
                        "module uses WRAP as border policy.");
        }
    }
}


void HorizontalMobility::fixIfHostGetsOutside()
{
    Coord dummy = Coord(playgroundSizeX(),playgroundSizeX()/2);
    handleIfOutside(WRAP, stepTarget, dummy, dummy, angle);
}


/**
 * Move the host if the destination is not reached yet. Otherwise
 * calculate a new random position
 */
void HorizontalMobility::makeMove()
{
    debugEV << "start makeMove " << move.info() << endl;

    move.setStart(stepTarget, simTime());
    double nextSpeed = move.getSpeed()+uniform(-1.0,1.0)*acceleration*SIMTIME_DBL(updateInterval);
    stepTarget.x = (move.getStartPos().x + moveDir*(move.getSpeed() + nextSpeed)*SIMTIME_DBL(updateInterval)/2);
    stepTarget.y = move.getStartPos().y;

    move.setDirectionByTarget(stepTarget);

    debugEV << "new stepTarget: " << stepTarget.info() << endl;

    // accelerate
    move.setSpeed(move.getSpeed() + acceleration * SIMTIME_DBL(updateInterval));

    fixIfHostGetsOutside();
}
