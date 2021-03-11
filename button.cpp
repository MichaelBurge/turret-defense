// button.cpp: implementation of the button class.
//
//////////////////////////////////////////////////////////////////////
#include <deque>
using namespace std;
#include "main.h"
#include "draw.h"
#include "button.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern input gameinput;
extern draw drawing;
button::button()
{
active=true;
x=0;
y=0;
parent=NULL;
bool solid=false;
}

button::~button()
{

}
char *button::Save(void) {
	return NULL;
}
char *button::Load(void) {
	return NULL;
}

void button::HandleMessages(void) {
	int size = messages.size();
	while (size--) {
		switch (messages[0].message) {
		case CBUTTON_SETUP:
			uppicture=messages[0].parameter1;
			break;
		case CBUTTON_SETDOWN:
			downpicture=messages[0].parameter1;
			break;
		case CONTROL_ACTIVATE:
			active = messages[0].parameter1>0;
			break;
		case CBUTTON_SETCALLBACK:
			callback=(void (__cdecl *)(bool,void *))messages[0].parameter1;
			break;
		case CONTROL_SETPOS:
			if	(messages[0].parameter1>0&&messages[0].parameter1<drawing.width&&(!parent||
				(messages[0].parameter1>parent->xpos()&&messages[0].parameter1<parent->xpos()+parent->GetWidth())))
				x = messages[0].parameter1;
			if	(messages[0].parameter2>0&&messages[0].parameter2<drawing.height&&(!parent||
				(messages[0].parameter2>parent->ypos()&&messages[0].parameter2<parent->ypos()+parent->GetHeight())))
				y = messages[0].parameter2;
			break;
		case CONTROL_SETSIZE:
			width=messages[0].parameter1;
			height=messages[0].parameter2;
			break;
		case CBUTTON_GETDOWN:
			*(bool*)messages[0].parameter1=pressed;
			break;
		case CBUTTON_SETEXTRA:
			extra = (void*)messages[0].parameter1;
			break;
		case CBUTTON_SOLID:
			solidity=messages[0].parameter1;
		default:
			break;//Do nothing for default
		}
		messages.pop_front();
	}
}
void button::Process() {
	RECT rect = {0,0,width,height};
	HandleMessages();
	if (active) {
		bool within = gameinput.mousex > xpos() && gameinput.mousex < xpos()+width &&gameinput.mousey > ypos() && gameinput.mousey < ypos()+height;
		if (within&&gameinput.leftclicked) 
			pressed = true;
		drawing.DrawPicture(xpos(),ypos(),pressed?downpicture:uppicture,&rect);
		if (within&&!gameinput.leftclicked&&pressed&&callback) {
			callback(true,extra);
			pressed=false;
		}
		else if (within&&callback)
			callback(false,extra);
	}
}