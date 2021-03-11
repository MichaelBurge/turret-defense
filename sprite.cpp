// sprite.cpp: implementation of the sprite class.
//
//////////////////////////////////////////////////////////////////////
#include "main.h"
#include "sprite.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern draw drawing;

sprite::sprite(void) {
	width=0;
	height=0;
	current=0;
}
sprite::~sprite()
{

}


void sprite::AddPicture(const char *file) {
	pictures.push_back(drawing.LoadSurface(file));
}
void sprite::AddPicture(int picture) {
	pictures.push_back(picture);
}
HRESULT sprite::Draw(unsigned int x,unsigned int y) {
	if (pictures.size()) {
		if (current >= pictures.size())
			current=0;
		RECT rect = {0,0,width,height};
		return drawing.DrawPicture(x,y,pictures[current++],&rect);
	}
	return NULL;
}

