// window.cpp: implementation of the window class.
//
//////////////////////////////////////////////////////////////////////

#include "main.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern draw drawing;
extern input gameinput;
extern logger debugger;
window::window() {
	parent=NULL;
	x=0;
	y=0;
	width=0;
	height=0;
	bgcolor=0;
	foregroundcolor=RGB(0,255,0);
	textcolor=RGB(255,255,255);
	titlebarcolor=RGB(40,40,40);
	titlebarcolor2=RGB(0,0,0);
	titlebarsize=20;
	selected=false;
	active=true;
	backgroundpicture=-1;
	close.SetParent(this);
	close.GiveMessage(CONTROL_SETPOS,0,0);
	close.GiveMessage(CONTROL_SETSIZE,16,16);
	close.GiveMessage(CONTROL_ACTIVATE,0,0);
	close.GiveMessage(CBUTTON_SETEXTRA,(int)this,0);
	close.GiveMessage(CBUTTON_SETCALLBACK,(int)windowclose,0);
	bool button=false;
}
window::~window()
{

}

void window::HandleMessages(void) {
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
			close.GiveMessage(CONTROL_SETPOS,width-24,0);
			break;
		case CWINDOW_SETTITLEBARSIZE:
			titlebarsize=messages[0].parameter1;
			break;
		case CWINDOW_SETTITLEBARCOLOR:
			titlebarcolor=messages[0].parameter1;
			break;
		case CWINDOW_SETBGCOLOR:
			bgcolor=messages[0].parameter1;
			break;
		case CWINDOW_SETFGCOLOR:
			foregroundcolor=messages[0].parameter1;
			break;
		case CWINDOW_SETTEXTCOLOR:
			textcolor=messages[0].parameter1;
			break;
		case CONTROL_ACTIVATE:
			active=messages[0].parameter1>0;
			break;
		case CWINDOW_SETBACKGROUND:
			backgroundpicture=messages[0].parameter1;
			break;
		case CWINDOW_SETBACKGROUNDSIZE:
			bgpicwidth=messages[0].parameter1;
			bgpicheight=messages[0].parameter2;
			break;
		case CWINDOW_SETCLOSEPICTURES:
			close.GiveMessage(CBUTTON_SETUP,messages[0].parameter1,0);
			close.GiveMessage(CBUTTON_SETDOWN,messages[0].parameter2,0);
			break;
		case CWINDOW_SETCLOSECALLBACK:
			callback = (void (_cdecl *)(void))messages[0].parameter1;
			break;
		case CWINDOW_TOGGLECLOSEBUTTON:
			button = messages[0].parameter1>0;
			close.GiveMessage(CONTROL_ACTIVATE,messages[0].parameter1,0);
			break;
		default:
			break;//Do nothing for default
		}
		messages.pop_front();
	}
}

void window::Process() {
	HandleMessages();
	if (active) {
		bool pressed=false;
		if (button) {
			
			close.GiveMessage(CBUTTON_GETDOWN,(int)&pressed,0);
			close.Process();
		}
		if (!pressed&&gameinput.leftclicked&&(selected||(gameinput.mousex>xpos()&&gameinput.mousex<xpos()+width&&
			gameinput.mousey>ypos()&&gameinput.mousey<ypos()+titlebarsize))) {
			x+=(x+gameinput.xdiff>0&&x+gameinput.xdiff+width<drawing.width )?gameinput.xdiff:(gameinput.mousex-=gameinput.xdiff,0);
			y+=(y+gameinput.ydiff>0&&y+gameinput.ydiff+height<drawing.height)?gameinput.ydiff:(gameinput.mousey-=gameinput.ydiff,0);
			selected=true;
			gameinput.leftclicked=false;
		} else selected=false;
		if (backgroundpicture<0)drawing.DrawColor(x,y,x+width,y+height,bgcolor);
		else drawing.DrawPictureRepeat(backgroundpicture,bgpicwidth,bgpicheight,xpos(),ypos(),xpos()+width,ypos()+height);
		drawing.DrawColor(x,y,x+width,y+titlebarsize,selected?titlebarcolor:titlebarcolor2);
		drawing.DrawLine(x,y+titlebarsize,x+width,y+titlebarsize,foregroundcolor);
		drawing.DrawLine(x,y,x,y+height,foregroundcolor);
		drawing.DrawLine(x,y+height,x+width,y+height,foregroundcolor);
		drawing.DrawLine(x+width,y+height,x+width,y,foregroundcolor);
		if (button) close.Process();
	}
}

void windowclose(bool pressed,void *data) {
	if (pressed) {
		window *target = static_cast<window*>(data);
		target->callback();
	}
}