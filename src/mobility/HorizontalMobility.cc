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
        angle = par("angle").doubleValue();
        speed = par("speed").doubleValue();
        speedOffset = par("speedOffset").doubleValue();
        ASSERT2(speed>speedOffset,"speed must bigger than speedOffset. Check the INI file.");
        maxSpeed = speed+speedOffset;
        minSpeed = speed-speedOffset;
        move.setSpeed(speed);

        acceleration = par("acceleration");
        gcu = getGCU();
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
    Coord dummy = Coord(playgroundSizeX()-move.getStartPos().x,playgroundSizeY()/2);

    if(handleIfOutside(WRAP, stepTarget, dummy, dummy, angle)){
        gcu->disconnectAll();
    }
}

void HorizontalMobility::makeMove()
{
    debugEV << "start makeMove " << move.info() << endl;

    move.setStart(stepTarget, simTime());
    double nextSpeed = move.getSpeed()+uniform(-1.0,1.0)*acceleration*SIMTIME_DBL(updateInterval);
    if(nextSpeed>maxSpeed)
        nextSpeed = maxSpeed;
    if(nextSpeed<minSpeed)
        nextSpeed = minSpeed;
    stepTarget.x = (move.getStartPos().x + move.getSpeed() * cos(PI * angle / 180) * SIMTIME_DBL(updateInterval));
    stepTarget.y = (move.getStartPos().y + move.getSpeed() * sin(PI * angle / 180) * SIMTIME_DBL(updateInterval));

    move.setDirectionByTarget(stepTarget);

    debugEV << "new stepTarget: " << stepTarget.info() << endl;

    // accelerate
    move.setSpeed(nextSpeed);

    fixIfHostGetsOutside();

    gcu->setCurrentPostion(move.getStartPos());
}


GlobalControlUnit* HorizontalMobility::getGCU() {
    return FindModule<GlobalControlUnit*>::findSubModule(this->getParentModule());
}
