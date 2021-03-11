// tank.cpp: implementation of the tank class.
//
//////////////////////////////////////////////////////////////////////
#include <deque>
using namespace std;
#include <windows.h>
#include "sprite.h"
#include "tileset.h"
#include "unit.h"
#include "draw.h"
#include "sprite.h"
#include "object.h"
#include "tank.h"
#include "engine.h"
extern engine gameengine;
extern draw drawing;

const D3DCOLOR tankcolor = D3DCOLOR_ARGB(255,255,128,128);

//////////////////////////////////////////////////////////////////////
// Destruction
//////////////////////////////////////////////////////////////////////

tank::~tank()
{

}

void tank::Process(void) {
	if (steps) {
		steps--;
		point = MoveInDirection(point.x,point.y,direction,speed);

	}

}

void tank::Render(void) {
	if (selected)
		drawing.SetModulation(tankcolor);

	if (facingdirection<4)
		graphics[facingdirection].Draw(point.x,point.y);
	else
		throw "Invalid facing direction";
	drawing.SetModulation(0xFFFFFFFF);
}