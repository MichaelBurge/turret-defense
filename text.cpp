// text.cpp: implementation of the text class.
//
//////////////////////////////////////////////////////////////////////

#include "main.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern draw drawing;
text::text()
{
	textcolor=RGB(192,192,192);
	active=true;
	parent=NULL;
	string=NULL;
}

text::~text()
{

}

void text::HandleMessages(void) {
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
		case CTEXT_SETTEXT:
			if (string) delete [] string;
			string = new char[strlen((char*)messages[0].parameter1)+1];
			strcpy(string,(char*)messages[0].parameter1);
			break;
		case CTEXT_SETCOLOR:
			textcolor=messages[0].parameter1;
			break;
		default:
			break;//Do nothing for default
		}
		messages.pop_front();
	}
}

void text::Process(void) {
	HandleMessages();
	if (active&&string) drawing.DrawText(string,xpos(),ypos(),textcolor);
}