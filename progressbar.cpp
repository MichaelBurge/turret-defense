// progressbar.cpp: implementation of the progressbar class.
//
//////////////////////////////////////////////////////////////////////

#include "progressbar.h"
#include "draw.h"
extern draw drawing;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

progressbar::progressbar()
{
	max = 0;
	step = 0;
	active = true;
	increment = 0;
	backcolor = D3DCOLOR_ARGB(255,255,255,255);
	frontcolor= D3DCOLOR_ARGB(255,0,0,0);
	vertical = false;
}

progressbar::~progressbar()
{

}

void progressbar::HandleMessages(void) {
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
		case CPROGRESSBAR_SETMAX:
			max = messages[0].parameter1;
			break;
		case CPROGRESSBAR_STEP:
			step += increment;
			break;
		case CPROGRESSBAR_SETINCREMENT:
			increment = messages[0].parameter1;
			break;
		case CPROGRESSBAR_RESET:
			step = 0;
			break;
		case CPROGRESSBAR_GETFINISHED:
			*(reinterpret_cast<bool*>(messages[0].parameter1)) = step >= max;
			break;
		case CPROGRESSBAR_GETSTEP:
			*(reinterpret_cast<int*>(messages[0].parameter1)) = step;
			break;
		case CPROGRESSBAR_GETINCREMENT:
			*(reinterpret_cast<int*>(messages[0].parameter1)) = increment;
			break;
		case CPROGRESSBAR_GETMAX:
			*(reinterpret_cast<int*>(messages[0].parameter1)) = max;
			break;
		case CPROGRESSBAR_GETBACKCOLOR:
			*(reinterpret_cast<D3DCOLOR*>(messages[0].parameter1)) = backcolor;
			break;
		case CPROGRESSBAR_SETBACKCOLOR:
			backcolor = static_cast<D3DCOLOR>(messages[0].parameter1);
			break;
		case CPROGRESSBAR_GETFRONTCOLOR:
			*(reinterpret_cast<D3DCOLOR*>(messages[0].parameter1)) = frontcolor;
			break;
		case CPROGRESSBAR_SETFRONTCOLOR:
			frontcolor = static_cast<D3DCOLOR>(messages[0].parameter1);
			break;
		case CPROGRESSBAR_SETVERTICAL:
			vertical = messages[0].parameter1;
			break;
		}
		messages.pop_front();
	}

}

void progressbar::Process(void) {
	HandleMessages();
	if (active) {
		//If progress bar is only partially progressed...
		if (step < max) {
			//Draw background color of progress bar
			drawing.DrawColor(xpos(),ypos(),xpos()+width,ypos()+height,backcolor);
			//Calculate percentage of completion
			double percentage = (double)step/(double)max;
			//Draw the progress vertically or horizontally, depending on settings.
			if (vertical)
				drawing.DrawColor(xpos(),ypos(),xpos()+width,ypos()+(height*percentage),frontcolor);
			else
				drawing.DrawColor(xpos(),ypos(),xpos()+(width*percentage),ypos()+height,frontcolor);
		//Otherwise, draw the progress bar with the front color
		} else {
			drawing.DrawColor(xpos(),ypos(),xpos()+width,ypos()+height,frontcolor);
		}
	}

}