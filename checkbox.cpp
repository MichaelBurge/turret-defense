// checkbox.cpp: implementation of the checkbox class.
//
//////////////////////////////////////////////////////////////////////

#include "main.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern input gameinput;
extern draw drawing;
checkbox::checkbox()
{
	onpicture=0;offpicture=0;
	fine=false;selected=false;
	active=true;
}

checkbox::~checkbox()
{

}
void checkbox::HandleMessages(void) {
	int size = messages.size();
	while (size--) {
		switch (messages[0].message) {
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
		case CONTROL_ACTIVATE:
			active = messages[0].parameter1>0;
			break;
		case CCHECKBOX_SETPICTURE:
			onpicture = messages[0].parameter1;
			offpicture= messages[0].parameter2;
			break;
		default:
			break;//Do nothing for default
		}
		messages.pop_front();
	}
}
void checkbox::Process(void) {
	RECT rect = {0,0,width,height};
	HandleMessages();
	if (active) {
		bool within = gameinput.mousex > xpos() && gameinput.mousex < xpos()+width &&gameinput.mousey > ypos() && gameinput.mousey < ypos()+height;
		if (gameinput.leftclicked&&within&&fine) {selected=!selected;fine=false;callback(selected);gameinput.leftclicked=false;}
		if (!gameinput.leftclicked) fine=true;
		drawing.DrawPicture(x,y,selected?onpicture:offpicture,&rect);
	}
}