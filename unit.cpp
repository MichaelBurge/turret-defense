// unit.cpp: implementation of the unit class.
//
//////////////////////////////////////////////////////////////////////
#include "draw.h"
#include "sprite.h"
#include "unit.h"
#include "math.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

unit::unit() {
	speed = 0.0;
	direction = 0;
	steps = 0;
	player = 0;
}

unit::~unit() {
}

void unit::Render(void) {
	graphic.Draw(point.x,point.y);
}

void unit::Process(void) {
	if (steps) {
		steps--;
		point = MoveInDirection(point.x,point.y,direction,speed);
	}

}

void unit::Move(POINT& destination) {
	if (moves&&!steps) {
		moves--;//Subtract a move from the available moves
		//Get the direction between current location and destination
		direction = GetDirection(point.x,point.y,destination.x,destination.y);
		//Number of processings to get to destination = Distance to destination / speed of unit
		steps = DistanceToPoint(point.x,point.y,destination.x,destination.y)/speed;
	}
}