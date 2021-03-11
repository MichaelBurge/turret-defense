// object.cpp: implementation of the object class.
//
//////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <deque>
using namespace std;
#include "sprite.h"
#include "object.h"
#include "math.h"
#include "input.h"
extern input gameinput;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


object::~object()
{

}
void object::Process(void) {

	if (callback) {
		SIZE ret;
		ret = callback(this,Message_Position,extra);
		x +=ret.cx;
		y +=ret.cy;
	}
}
void object::Draw(unsigned int screenx,unsigned int screeny) {
	if (callback&&objectsprite.GetCurrent()>=objectsprite.GetPictureCount())
		callback(this,Message_EndOfSprite,extra);
	if (x+objectsprite.GetWidth() > x && x < screenx+screenwidth&&//If object is within screen...
		y+objectsprite.GetHeight()> y && y < screeny+screenheight)
		objectsprite.Draw(x-screenx,y-screeny);//Draw object on screen
}