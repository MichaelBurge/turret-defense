// projectile.cpp: implementation of the projectile class.
//
//////////////////////////////////////////////////////////////////////

#include "projectile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

projectile::projectile()
{
	callback = NULL;
	x = 0;
	y = 0;
	speed = 0.0;
	exploding = false;
	time = 0;
	active = false;
}

projectile::~projectile()
{

}


void projectile::Process(void) {
	if (active &&!exploding) {
		POINT temp = MoveInDirection(x,y,GetDirection(x,y,targetx,targety),speed);
		x = temp.x;
		y = temp.y;
		if (DistanceToPoint(x,y,targetx,targety)<4.0) {
			callback(TargetReached,*this,extra);
			exploding = true;
		}
	} else if (active) {
		if (!(time--)) {
			callback(Exploded,*this,extra);
			active = false;
		}
	}
}

void projectile::Render(void) {
	if (active) {
		if (exploding) 
			explosion.Draw(x,y);
		else 
			picture.Draw(x,y);
	}
}