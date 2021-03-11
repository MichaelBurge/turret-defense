// radio.cpp: implementation of the radio class.
//
//////////////////////////////////////////////////////////////////////

#include "main.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern draw drawing;
extern input gameinput;
radio::radio()
{
	selected=-1;
	unselectedpic=-1;
	selectedpic=-1;
	active=true;
}

radio::~radio()
{

}

void radio::HandleMessages(void) {
	int size = messages.size();
	while (size--) {
		switch (messages[0].message) {
		case CONTROL_ACTIVATE:
			active = messages[0].parameter1>0;
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
		case CRADIO_ADDBUTTON:
			{
				POINT p = {messages[0].parameter1,messages[0].parameter2};
				buttons.push_back(p);
			}
			break;
		case CRADIO_SETPICTURES:
			selectedpic=messages[0].parameter1;
			unselectedpic=messages[0].parameter2;
			break;
		case CRADIO_GETSELECTED:
			*((int*)messages[0].parameter1)=selected;
			break;
		default:
			break;//Do nothing for default
		}
		messages.pop_front();
	}
}
void radio::Process() {
	HandleMessages();
	if (active) {
		RECT rect={0,0,width,height};//Get rect for size of pic and loop through buttons, drawing them
		for (int temp=0;temp<buttons.size();temp++)
			if (gameinput.leftclicked&&(xpos()+buttons[temp].x)<gameinput.mousex&&(xpos()+buttons[temp].x+width)>(gameinput.mousex) &&
				(ypos()+buttons[temp].y)<gameinput.mousey&&(ypos()+buttons[temp].y+height)>(gameinput.mousey)) selected=temp;
		for (temp=0;temp<buttons.size();temp++)
			if (selected==temp) drawing.DrawPicture(xpos()+buttons[temp].x,ypos()+buttons[temp].y,selectedpic,&rect);
			else drawing.DrawPicture(xpos()+buttons[temp].x,ypos()+buttons[temp].y,unselectedpic,&rect);
	}
}